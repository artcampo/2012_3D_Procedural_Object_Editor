#include "EditorMaterial.hpp"
#include "UI/MainWindow.hpp"

EditorMaterial::EditorMaterial()
{
  mProcShader = new ProcShader();
}

EditorMaterial::~EditorMaterial()
{

}



void EditorMaterial::loadFromFile(QFile &aFile)
{
    delete mProcShader;
    mProcShader = new ProcShader( aFile );
}

void EditorMaterial::saveToFile(QFile &aFile)
{
    mProcShader->save(aFile);
}

void EditorMaterial::exportToFile(QString& aName)
{  
  wchar_t path[2048];
  wcscpy_s ( path, (aName.toStdWString()).c_str() );
  mProcShader->exportToHlsl( (LPCWSTR)&path[0]);
}

void EditorMaterial::exportToFileDeferred(QString& aName, int aVertexLayout)
{
  wchar_t path[2048];
  wcscpy_s ( path, (aName.toStdWString()).c_str() );
  mProcShader->exportToHlslDeferred( (LPCWSTR)&path[0], aVertexLayout );
}

void EditorMaterial::updateStatusBarByTab( QString aName )
{
  GlobalEditor::gMainWindow->updateStatusBarByTab(aName);
}

void EditorMaterial::newFile()
{
  mFirstFreeUniqueIdSignal  = 0;
  mFirstFreeUniqueIdLayer   = 0;

  //  Create 1 signal, 1 layer for rgb
  std::string signalName = getSignalName( getUniqueIdSignal() );
  mProcShader->selectChannel( Channel::eChannel_rgba );
  mProcShader->createNewSignal ( signalName );
  mProcShader->modifiyLayer( mProcShader->getRootLayer(), signalName);
  //mProcShader->createNewLayer  ( signalName );

  //  Create 1 signal, 1 layer for rgb (far)
  signalName = getSignalName( getUniqueIdSignal() );
  mProcShader->selectChannel( Channel::eChannel_rgba_far );
  mProcShader->createNewSignal ( signalName );
  mProcShader->modifiyLayer( mProcShader->getRootLayer(), signalName);

  //1 layer for bump
  std::string signalBumpName = getSignalName( getUniqueIdSignal() );
  mProcShader->createNewSignal ( signalBumpName );
  mProcShader->selectChannel( Channel::eChannel_bump );
  mProcShader->modifiyLayer( mProcShader->getRootLayer(), signalBumpName );

  //back to default channel
  mProcShader->selectChannel( Channel::eChannel_rgba );

}

std::string EditorMaterial::getSignalName( int i )
{
  std::stringstream ret;
  ret << "Signal " << i;
  return ret.str();
}

std::string EditorMaterial::getLayerName( int i )
{
  std::stringstream ret;
  ret << "Layer " << i;
  return ret.str();
}

int EditorMaterial::getUniqueIdSignal()
{
  int ret = mFirstFreeUniqueIdSignal;
  ++mFirstFreeUniqueIdSignal;
  return ret;
}

int EditorMaterial::getUniqueIdLayer()
{
  int ret = mFirstFreeUniqueIdLayer;
  ++mFirstFreeUniqueIdLayer;
  return ret;
}

/*
  Mode list. Get all layers that correspond to a signal.
*/
std::vector<std::string>  EditorMaterial::getLayersSigNames_ModeList ()
{
  std::vector<std::string> ret;
  mProcShader->getLayersNames();

  std::string current = mProcShader->getRootLayer();

  bool finished = false;
  while (!finished)
  {
    Layer* l = mProcShader->getLayer(current);
    if ( l->getLayerType() == Layer::eLayerSignal)
    {
      ret.push_back( current );
      finished = true;
    }
    if ( l->getLayerType() == Layer::eLayerOp)
    {
      LayerOp* lo = dynamic_cast<LayerOp*> (l);
      ret.push_back( lo->getIthOperand(0) );
      current = lo->getIthOperand(1);
    }
  }

  return ret;
}


/*
  Mode list. Get all layers that correspond to an operation.
*/
std::vector<std::string>  EditorMaterial::getLayersOpNames_ModeList ()
{
  std::vector<std::string> ret;
  mProcShader->getLayersNames();

  std::string current = mProcShader->getRootLayer();

  bool finished = false;
  while (!finished)
  {
    Layer* l = mProcShader->getLayer(current);
    if ( l->getLayerType() == Layer::eLayerSignal)
    {
      finished = true;
    }
    if ( l->getLayerType() == Layer::eLayerOp)
    {
      LayerOp* lo = dynamic_cast<LayerOp*> (l);
      ret.push_back( current );
      current = lo->getIthOperand(1);
    }
  }

  return ret;
}


void EditorMaterial::createComposition ( int aNewOpId, float aBlendFactor, std::string& aFatherLayer )
{

  Layer* initialLayerSignal = mProcShader->getLayer( aFatherLayer );
  std::string oldParent = initialLayerSignal->parent();


  std::string signalName = getSignalName( getUniqueIdSignal() );
  mProcShader->createNewSignal ( signalName );

  std::string newLayerOperation = mProcShader->createNewLayer( signalName, oldParent );
  std::string newLayerSignal = mProcShader->createNewLayer( signalName, newLayerOperation );
  initialLayerSignal->setParent(newLayerOperation);

  mProcShader->modifiyLayer( newLayerOperation, aNewOpId, aBlendFactor, aFatherLayer, newLayerSignal );
  mProcShader->changePointedLayer( oldParent, aFatherLayer, newLayerOperation );
}

void EditorMaterial::swapLayerSignal(std::string aLayer1, std::string aLayer2, bool aSwapCompositeOp )
{
  if (aSwapCompositeOp)
  {
    Layer* l1p = mProcShader->getLayer( aLayer1 );
    std::string layer1Op = l1p->parent();
    Layer* l2p = mProcShader->getLayer( aLayer2 );
    std::string layer2Op = l2p->parent();

    LayerOp* lop1 = dynamic_cast<LayerOp*> (mProcShader->getLayer( layer1Op ) );
    LayerOp* lop2 = dynamic_cast<LayerOp*> (mProcShader->getLayer( layer2Op ) );

    std::string op1Parent     = lop1->parent();
    std::string op1ChildLeft  = lop1->getIthOperand(0);
    std::string op1ChildRight = lop1->getIthOperand(1);
    std::string op2Parent     = lop2->parent();
    std::string op2ChildLeft  = lop2->getIthOperand(0);
    std::string op2ChildRight = lop2->getIthOperand(1);

    //  Treat appart the case when the descending of one layer
    //  is the other
    if ( op1Parent == layer2Op || op2Parent == layer1Op  )
    {
      if ( op1Parent == layer2Op )
      {
        std::string temp = layer2Op;
        layer2Op = layer1Op;
        layer1Op = temp;
        lop1 = dynamic_cast<LayerOp*> (mProcShader->getLayer( layer1Op ) );
        lop2 = dynamic_cast<LayerOp*> (mProcShader->getLayer( layer2Op ) );
        op1Parent     = lop1->parent();
        op1ChildRight = lop1->getIthOperand(1);
        op2Parent     = lop2->parent();
        op2ChildRight = lop2->getIthOperand(1);
      }

      mProcShader->changePointedLayer( op1Parent, layer1Op, layer2Op );
      lop2->setIthOperand( 1, layer1Op );
      mProcShader->getLayer( layer1Op )->setParent ( layer2Op );
      lop2->setParent( op1Parent );

      lop1->setIthOperand( 1, op2ChildRight  );
      mProcShader->getLayer( op2ChildRight )->setParent ( layer1Op );
      lop1->setParent( layer2Op );

    }
    else
    {
      mProcShader->changePointedLayer( op1Parent, layer1Op, layer2Op );
      mProcShader->changePointedLayer( op2Parent, layer2Op, layer1Op );
      lop1->setParent( op2Parent );
      lop2->setParent( op1Parent );

      lop1->setIthOperand( 1, op2ChildRight );
      lop2->setIthOperand( 1, op1ChildRight );

      mProcShader->getLayer( op2ChildRight )->setParent ( layer1Op );
      mProcShader->getLayer( op1ChildRight )->setParent ( layer2Op );
    }
  }
  else
  {

      //  TODO: CODE SUSPECTED NOT TO WORK (and it shouldn't probably anyways)
    //  do not swap composite operations, only LayerSignals
    Layer* l1p = mProcShader->getLayer( aLayer1 );
    std::string layer1Op = l1p->parent();
    Layer* l2p = mProcShader->getLayer( aLayer2 );
    std::string layer2Op = l2p->parent();

    LayerOp* lop1 = dynamic_cast<LayerOp*> (mProcShader->getLayer( layer1Op ) );
    LayerOp* lop2 = dynamic_cast<LayerOp*> (mProcShader->getLayer( layer2Op ) );

    std::string op1Parent     = lop1->parent();
    std::string op2Parent     = lop2->parent();

    mProcShader->changePointedLayer( op1Parent, aLayer1, aLayer2);
    l1p->setParent( layer2Op );

    mProcShader->changePointedLayer( op2Parent, aLayer2, aLayer1);
    l2p->setParent( layer1Op );


  }
}

void EditorMaterial::channelSave( QFile& aFile )
{
  QDataStream stream(&aFile);

  QString title ("Channel v0.1");
  stream << title;
  stream << QString::fromStdString( mProcShader->channelName() );

  mProcShader->save( stream );
}

void EditorMaterial::channelLoad(QFile& aFile)
{
  mProcShader->deleteAllLayers();

  QDataStream stream(&aFile);

  QString title, channelName;
  stream >> title;
  stream >> channelName;
  mProcShader->setChannelName( channelName.toStdString() );

  mProcShader->load( stream );
}

void EditorMaterial::deleteLastLayerSignal(std::string &aLayerOp, std::string& aLayerSignalDelete, std::string& aLayerSignalKeep  )
{
  Layer* lop = mProcShader->getLayer( aLayerOp );
  Layer* lsg = mProcShader->getLayer( aLayerSignalKeep );

  std::string layerOpParent = lop->parent();

  mProcShader->changePointedLayer( layerOpParent, aLayerOp, aLayerSignalKeep );
  lsg->setParent(layerOpParent);

  mProcShader->deleteLayer( aLayerOp );
  mProcShader->deleteLayer( aLayerSignalDelete );
}
