#include "ObjectEditor.h"
#include "objeditorstate.h"
#include "UI/MainWindow.hpp"


ObjectEditor::ObjectEditor( )
{


  mDistanceThresholdSelectVertex  = 0.04f;
  mDistanceThresholdSelectGizmo   = 0.01f;

  mAdjustToGrid = false;


  mDefaultAlphaForBackgroundImage = 0.4f;

  mInhibitRedraw = false;
  mHasRenderModel = false;

  mRenderMode = ePhongMode;

  mObject = NULL;

  mOptionKnifeIsFirstPoint = false;

  init();
  initMisc();
  initEditorState();

}

ObjectEditor::~ObjectEditor()
{
  mBackgroundImageTexture.resize(0);

  delete mObject;
  for (int i = 0; i < mUndoStates.size();++i)
    delete mUndoStates[i];
  mUndoStates.clear();
}




void ObjectEditor::selectWorkView( QD3DWidgetObj* aView )
{

  if ( mWorkingView != aView )
  {
    mWorkingView = aView;
    updateWorkView();
  }

}



void ObjectEditor::updateWorkView()
{
  if ( !mInhibitRedraw )
    emit updateViews();
}

void ObjectEditor::initEditorState()
{
  for ( unsigned int i = 0 ; i < mUndoStates.size(); ++i )
    delete mUndoStates[i];

  mUndoStates.clear();
  mUndoPosition = 0;
  mRedoDone     = false;
}



void ObjectEditor::saveEditorState()
{  

  mRedoDone = false;
  int index;
  if (mUndoPosition < mUndoStates.size())
  {
    for ( unsigned int i = mUndoPosition + 1 ; i < mUndoStates.size(); ++i )
    {
      delete mUndoStates[i];
    }
    mUndoStates.resize( mUndoPosition + 1 );
    index = mUndoPosition;
  }
  else
  {
    index = mUndoStates.size();
    mUndoStates.resize( index + 1 );
  }
  //mObject->serialize();
  mInhibitRedraw = true;
  mUndoStates[index] = new ObjEditorState( *mObject );
  mInhibitRedraw = false;
  ++mUndoPosition;

}

void ObjectEditor::undoEditorState()
{

  if ( undoPossible())
  {
    if ( mUndoPosition == mUndoStates.size())
      mUndoPosition -= 2;
    else
      --mUndoPosition;


    delete mObject;
    mObject = mUndoStates[mUndoPosition]->restoreState();

    createGizmoT();
    updateViews();
  }

  mRedoDone = false;
}

void ObjectEditor::redoEditorState()
{

  if ( redoPossible())
  {   
    //TODO: block all the views
    delete mObject;
    if (!mRedoDone)
    {
      mObject = mUndoStates[mUndoPosition + 1]->restoreState();
      mUndoPosition += 2;
      mRedoDone = true;
    }
    else
    {
      mObject = mUndoStates[mUndoPosition]->restoreState();
      ++mUndoPosition;
    }
    createGizmoT();   //TODO: should iot be show afterwards?
    updateViews();
  }

}

bool ObjectEditor::undoPossible()
{
  return (mUndoPosition >0 && mUndoStates.size() > 1);
}

bool ObjectEditor::redoPossible()
{
  return (mUndoPosition < mUndoStates.size()) && (mUndoStates.size() > 0);
}


void ObjectEditor::loadFromFile(QString &aFile, int aMode)
{
  QFile file(aFile);

  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::critical(this, tr("Error"),
        tr("Could not open file for load"));
    return;
  } else {
    QDataStream stream(&file);
    mObject->load( stream, QCoreApplication::applicationDirPath() );
    file.close();

    QFileInfo info1(aFile);
    mFileLastKnowSize = info1.size();

    mObject->setMode( aMode );
    hideGizmo();
    initEditorState();
    saveEditorState();

    updateViews();

  }
}

void ObjectEditor::saveToFile(QString &aFile)
{
  QFile file(aFile);

  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::critical(this, tr("Error"),
        tr("Could not open file for save"));
    return;
  } else {
    QDataStream stream(&file);

    if ( stream.status() != QDataStream::Ok)
    {
      QMessageBox::critical(this, tr("Error"),
          tr("Error on stream creation."));
      return;
    }

    mObject->save( stream );

    if ( stream.status() != QDataStream::Ok)
    {
      QMessageBox::critical(this, tr("Error"),
          tr("Error on stream write."));
      return;
    }

    file.flush();
    mFileLastKnowSize = file.size();
    file.close();

    ofstream fileStd ("object.nbo", ofstream::out| ofstream::binary);
    mObject->save(fileStd);
    fileStd.close();

  }
}

void ObjectEditor::exportToFile(QString &aFile)
{

}


void ObjectEditor::setBackgroundImageFileName(QString &aFileName, float aAlpha, float aSizeMult )
{/*
  mBackgroundImageFileName = aFileName;

  mBackgroundImageTexture.resize(0);


  wchar_t fileName[4096];

  wcscpy_s ( fileName,  ((aFileName.toStdWString()).c_str()) );
   long bmps;
  BYTE* p = BMP::LoadBMP( &mBmpw,&mBmph,&bmps, (LPCTSTR)fileName );
  mBackgroundImageTexture.resize(mBmph*mBmpw*4);
  for (int i = 0; i < (mBmph*mBmpw);++i)
  {
    mBackgroundImageTexture[ 4*i + 0] = *(p + 3*i + 0);
    mBackgroundImageTexture[ 4*i + 1] = *(p + 3*i + 1);
    mBackgroundImageTexture[ 4*i + 2] = *(p + 3*i + 2);
    mBackgroundImageTexture[ 4*i + 3] = 255;
  }


  mBackgroundImageVertices = mWorkingView->computeBackgroundImageVertices( aSizeMult );

  //updateViews();
  */
}

void ObjectEditor::updateStatusBarByTab( QString aName )
{
  GlobalEditor::gMainWindow->updateStatusBarByTab(aName);
}


void ObjectEditor::muteBackgroundImage()
{
  /*
  GlobalEditor::gViewY->muteBackgroundImage();
  GlobalEditor::gViewX->muteBackgroundImage();
  GlobalEditor::gViewZ->muteBackgroundImage();
  GlobalEditor::gViewF->muteBackgroundImage();
  */
}

void ObjectEditor::setGridMultiplier(float aMult)
{
  /*
  GlobalEditor::gViewY->setGridScaleMultiplier(aMult);
  GlobalEditor::gViewX->setGridScaleMultiplier(aMult);
  GlobalEditor::gViewZ->setGridScaleMultiplier(aMult);
  GlobalEditor::gViewF->setGridScaleMultiplier(aMult);
  */
}

void ObjectEditor::newFile()
{
  //  object  

  mObject = new Mesh();

  mObject->setMode( eModeFace );
  mObject->newPrimitive( OpId::ePrimCube );


  mHasRenderModel = true;
  mFileName = QString("untitled.ngm");

  mFileLastKnowSize = 0;

  hideGizmo();

  init();
  initEditorState();
  saveEditorState();
}

bool ObjectEditor::checkEntity(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE)
{  
  //  Knife-op: at half
  bool selectPointAtMiddleOfEdge = false;
  if ( mMode == eModeEdge &&
       mOpMode == eOpModeKniveLine &&
       mOptionKnifeHalf )
  {
    selectPointAtMiddleOfEdge = true;
  }

  int knifeCut = mOptionKnifeHalfCut;

  if (knifeCut != 0 && mOptionKnifeIsFirstPoint)
  {
    if ( knifeCut == 1) knifeCut = 2;
    else if ( knifeCut == 2) knifeCut = 1;
  }

  Selection s = mObject->checkEntity ( aRayB, aRayE,
                                       distanceThreshold(),
                                       mWorkingView->app->mView,
                                       mWorkingView->mView->getCamPos(),
                                       mHighlightedSegmentPoint,
                                       selectPointAtMiddleOfEdge,
                                       knifeCut,
                                       mHighlightedPoint);



  //  update model only on change of highlighting
  /*
  if ( (s.idComponent != -1 || mHighlighted.idComponent == -1 )
     ||(s.idComponent == -1 || mHighlighted.idComponent != -1 )
     ||(s.typeEntity != mHighlighted.typeEntity )
     ||(s.idComponent != mHighlighted.idComponent ))
*/
  if ( (s.idComponent != -1 && mHighlighted.idComponent == -1 )
     ||(s.idComponent == -1 && mHighlighted.idComponent != -1 )
     ||(s.idComponent != mHighlighted.idComponent && mHighlighted.idComponent != -1 )
     ||(s.idEntity != mHighlighted.idEntity && mHighlighted.idComponent != -1 ))
  {
    mHighlighted = s;
    if ( mRenderMode != eProcTexMode )
      updateWorkView();
  }

  return (s.idComponent != -1);
}

bool ObjectEditor::checkGizmoT(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, bool& aUpdateGizmo )
{
  bool highlighted = false;

  if ( mAllowsGizmoT )
  {
    bool previousState = mGizmoT.hasHighlight();
    highlighted = mGizmoT.check( aRayB, aRayE, distanceThresholdSelectGizmo() );

    aUpdateGizmo = (previousState != highlighted);
  }

  return highlighted;
}

bool ObjectEditor::checkGizmoR(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, bool& aUpdateGizmo )
{
  bool highlighted = false;

  if ( mAllowsGizmoT )
  {
    bool previousState = mGizmoT.hasHighlight();
    highlighted = mGizmoT.check( aRayB, aRayE, distanceThresholdSelectGizmo() );

    aUpdateGizmo = (previousState != highlighted);
  }

  return highlighted;
}

bool ObjectEditor::checkGizmoS(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, bool& aUpdateGizmo )
{
  bool highlighted = false;

  if ( mAllowsGizmoS )
  {
    bool previousState = mGizmoS.hasHighlight();
    highlighted = mGizmoS.check( aRayB, aRayE, distanceThresholdSelectGizmo() );

    aUpdateGizmo = (previousState != highlighted);
  }

  return highlighted;
}

//TODO: each mode
float ObjectEditor::distanceThreshold()
{
  return mDistanceThresholdSelectVertex;
}


std::vector<EditableMeshModel*> ObjectEditor::convertObject( ID3D10Device* aDev, D3DXVECTOR3 aCamPos )
{
  return mObject->convert( aDev, aCamPos , &mColours );
}

std::vector<EditableMeshModelProcTex*> ObjectEditor::convertObjectProcTex( ID3D10Device* aDev, D3DXVECTOR3 aCamPos )
{
  return mObject->convertProcTex( aDev, aCamPos , &mColours );
}

void ObjectEditor::initMisc()
{
  mColours.obj      = D3DXCOLOR( 0.8f, 0.0f, 0.0f, 1.0f );
  mColours.objSel   = D3DXCOLOR( 0.8f, 0.0f, 0.0f, 1.0f );
  mColours.objHigh  = D3DXCOLOR( 0.8f, 0.0f, 0.0f, 1.0f );

  mColours.ver      = D3DXCOLOR( 0.8f, 0.0f, 0.0f, 1.0f );
  mColours.verSel   = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
  mColours.verHigh  = D3DXCOLOR( 0.0f, 0.0f, 0.8f, 1.0f );

  mColours.edg      = D3DXCOLOR( 0.8f, 0.0f, 0.0f, 1.0f );
  mColours.edgSel   = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
  mColours.edgHigh  = D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f );


  mColours.fac      = D3DXCOLOR( 0.78f, 0.78f, 0.9f, 0.2f );
  mColours.facSel   = D3DXCOLOR( 0.8f, 0.8f, 0.0f, 0.2f );
  mColours.facHigh  = D3DXCOLOR( 0.6f, 0.6f, 0.6f, 0.2f );

  mColours.gizmo[0] = D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f );
  mColours.gizmo[1] = D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f );
  mColours.gizmo[2] = D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f );

  mColours.gizmoHigh[0] = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
  mColours.gizmoHigh[1] = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
  mColours.gizmoHigh[2] = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );

  mColours.gizmoSel[0] = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
  mColours.gizmoSel[1] = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );
  mColours.gizmoSel[2] = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f );

}

void ObjectEditor::setMode(int aMode)
{
  if ( mMode != aMode )
  {
    if ( mOperationUnfinished )
      endOperation();

    unselectEntities();
    hideGizmo();

    mMode = aMode;
    mObject->setMode( aMode );
  }

}

void ObjectEditor::init()
{
  atLeastOneEntitySelected  = false;
  mOperationUnfinished      = false;
  mOperationOnGizmoT        = false;

  mAllowsSelect = true;

  mOperationStep = 10.0f;

  //TODO: delme
  initOptions();

}

void ObjectEditor::initOptions()
{
  mOptionKnifeHalf    = false;
  mOptionExtrudeMode  = eOpExtModeOffset;


}

bool ObjectEditor::allowsOperation()
{
  return atLeastOneEntitySelected;
}

bool ObjectEditor::allowsSelection()
{
  return mAllowsSelect;
}

bool ObjectEditor::allowsModifySelection()
{
  return (atLeastOneEntitySelected & mAllowsSelect);
}

void ObjectEditor::selectEntityHighlighted()
{
  // TODO: selection of object (!) (no array access)
  mObject->selectEntity( mHighlighted );
  atLeastOneEntitySelected = true;
}



void ObjectEditor::unselectEntities()
{
  mObject->unselectEntities();  
  atLeastOneEntitySelected = false;
  hideGizmo();
  updateWorkView();  
}

void ObjectEditor::selectAddEntityHighlighted()
{
  mObject->selectAddEntity( mHighlighted );
  updateWorkView();
}

void ObjectEditor::selectRemoveEntityHighlighted()
{
  atLeastOneEntitySelected = mObject->selectRemoveEntity( mHighlighted );
  updateWorkView();
}

void ObjectEditor::createGizmoT()
{
  if ( atLeastOneEntitySelected )
  {
    switch( mMode )
    {
    case eModeVertex:
      createGizmoTVertices();
      break;
    case eModeEdge:
      createGizmoTEdges();
      break;
    case eModeFace:
      createGizmoTFaces();
      break;
    case eModeObject:
      createGizmoTObject();
      break;
    }

    showGizmoT();
  }
  else
    hideGizmoT();
}

void ObjectEditor::createGizmoR()
{
  if ( atLeastOneEntitySelected )
  {
    switch( mMode )
    {
    case eModeVertex:
      createGizmoRVertices();
      break;
    case eModeEdge:
      createGizmoREdges();
      break;
    case eModeFace:
      createGizmoRFaces();
      break;
    case eModeObject:
      createGizmoRObject();
      break;
    }

    showGizmoR();
  }
  else
    hideGizmoR();
}

void ObjectEditor::createGizmoS()
{
  if ( atLeastOneEntitySelected )
  {
    switch( mMode )
    {
    case eModeVertex:
      createGizmoSVertices();
      break;
    case eModeEdge:
      createGizmoSEdges();
      break;
    case eModeFace:
      createGizmoSFaces();
    case eModeObject:
      createGizmoSObject();
      break;
    }

    showGizmoS();
  }
  else
    hideGizmoS();
}

GizmoTMeshModel * ObjectEditor::gizmoT( ID3D10Device* aDev, float aScale )
{
  mGizmoT.setScale( aScale );
  return mGizmoT.convert(aDev, &mColours);
}

GizmoTMeshModel * ObjectEditor::gizmoR( ID3D10Device* aDev, float aScale )
{
  mGizmoR.setScale( aScale );
  return mGizmoR.convert(aDev, &mColours);
}

GizmoTMeshModel * ObjectEditor::gizmoS( ID3D10Device* aDev, float aScale )
{
  mGizmoS.setScale( aScale );
  return mGizmoS.convert(aDev, &mColours);
}

void ObjectEditor::createGizmoTVertices()
{
  mGizmoT.mCenter = mObject->centerMassSelectedVertices();
  mGizmoT.mBase[0] = D3DXVECTOR3(1,0,0);
  mGizmoT.mBase[1] = D3DXVECTOR3(0,1,0);
  mGizmoT.mBase[2] = D3DXVECTOR3(0,0,1);  
}

void ObjectEditor::createGizmoRVertices()
{
  mGizmoR.mCenter = mObject->centerMassSelectedVertices();
  mGizmoR.mBase[0] = D3DXVECTOR3(1,0,0);
  mGizmoR.mBase[1] = D3DXVECTOR3(0,1,0);
  mGizmoR.mBase[2] = D3DXVECTOR3(0,0,1);
}

void ObjectEditor::createGizmoSVertices()
{
  mGizmoS.mCenter = mObject->centerMassSelectedVertices();
  mGizmoS.mBase[0] = D3DXVECTOR3(1,0,0);
  mGizmoS.mBase[1] = D3DXVECTOR3(0,1,0);
  mGizmoS.mBase[2] = D3DXVECTOR3(0,0,1);
}


float ObjectEditor::distanceThresholdSelectVertex()
{
  return mDistanceThresholdSelectVertex * mWorkingView->camScale();
}

float ObjectEditor::distanceThresholdSelectGizmo()
{
  return mDistanceThresholdSelectGizmo * mWorkingView->camScale();
}

void ObjectEditor::startOperation()
{  
  mInhibitRedraw = true;

  saveToFile( QString("Crash_Recovery_PreOp.ngm") );

  if (mOpMode == eOpModeKniveLine ||
      mOpMode == eOpModeTexture)
  {
    mOperationUnfinished = true;

    if ( mOpMode == eOpModeKniveLine)
    {
      //startOpKniveLine();
    }
    if ( mOpMode == eOpModeTexture )
    {
      startOpTexture();
    }

  }
  else
  {
    if ( hasSelectionOfEntities() )
    {
      mOperationUnfinished = true;
      if ( mOpMode == eOpModeExtrude && mMode == eModeFace )
      {
        startOpExtrude();
      }

      if ( mOpMode == eOpModeBevel && mMode == eModeVertex )
      {
        startOpBevelV();
      }

      if ( mOpMode == eOpModeBevel && mMode == eModeEdge )
      {
        startOpBevelE();
      }

      if ( mOpMode == eOpModeNormalScale && mMode == eModeFace )
      {
        startOpNorScale();
      }
    }
  }

  mInhibitRedraw = false;
}

void ObjectEditor::performOperation( float x, float y )
{  

  if ( mOpMode == eOpModeExtrude && mMode == eModeFace)
  {
    performOpExtrude(x,y);
    updateWorkView();
  }

  if ( mOpMode == eOpModeBevel && mMode == eModeVertex )
  {
    performOpBevelV(x,y);
    updateWorkView();
  }

  if ( mOpMode == eOpModeBevel && mMode == eModeEdge )
  {
    performOpBevelE(x,y);
    updateWorkView();
  }

  if ( mOpMode == eOpModeNormalScale && mMode == eModeFace )
  {
    performOpNorScale(x,y);
    updateWorkView();
  }


}

float ObjectEditor::operationValue()
{
  if ( mOpMode == eOpModeExtrude && mMode == eModeFace)
  {
    return mExtrudeValue;
  }

  if ( mOpMode == eOpModeBevel && mMode == eModeVertex )
  {
    return mBevelVValue;
  }

  if ( mOpMode == eOpModeBevel && mMode == eModeEdge )
  {
    return mBevelEValue;
  }

  if ( mOpMode == eOpModeNormalScale && mMode == eModeFace )
  {
    return mNorScaleValue;
  }
  return 0.0f;
}

void ObjectEditor::setModeOp(int aModeOp)
{
  if ( mOpMode != aModeOp )
  {
    if ( mOperationUnfinished )
      endOperation();

    mOpMode = aModeOp;
    if ( mOpMode == eOpModeSelection ||
         mOpMode == eOpModeKniveLine ||
         mOpMode == eOpModeTexture)
    {
      unselectEntities();
    }

    if ( mOpMode == eOpModeTexture )
      mRenderMode = eProcTexMode;
    else
      mRenderMode = ePhongMode;

    ///////////////////////////////////////////////
    //  Modes proopierties
    if ( mOpMode == eOpModeRotation )
    {
      mAllowsGizmoT = false;
      mAllowsGizmoR = true;
      mAllowsGizmoS = false;
    }
    if ( mOpMode == eOpModeScale )
    {
      mAllowsGizmoT = false;
      mAllowsGizmoR = false;
      mAllowsGizmoS = true;
      hideGizmoT();
    }
    if (mOpMode != eOpModeScale && mOpMode != eOpModeRotation )
    {
      hideGizmoS();
      hideGizmoR();
    }
    if ( mOpMode == eOpModeKniveLine || mOpMode == eOpModeTexture )
    {
      hideGizmo();
      mAllowsSelect = false;
      mAllowsGizmoT = false;
      mAllowsGizmoR = false;
      mAllowsGizmoS = false;
    }
    if ( mOpMode != eOpModeKniveLine &&
         mOpMode != eOpModeRotation &&
         mOpMode != eOpModeScale &&
         mOpMode != eOpModeTexture)
    {
      mAllowsSelect = true;
      mAllowsGizmoT = true;
      mAllowsGizmoR = false;
      mAllowsGizmoS = false;
    }
  }
  emit updateViews();
}

void ObjectEditor::hideGizmoT( bool aUpdateViews)
{
  mWorkingView->hideGizmoT();
  if (aUpdateViews)
    updateWorkView();
}

void ObjectEditor::showGizmoT()
{
  if (mAllowsGizmoT)
  {
    hideGizmoS();
    mWorkingView->showGizmoT();
    updateWorkView();
  }
}

void ObjectEditor::hideGizmoR( bool aUpdateViews )
{
  mWorkingView->hideGizmoR();
  if (aUpdateViews)
    updateWorkView();
}

void ObjectEditor::showGizmoR()
{
  if (mAllowsGizmoR)
  {
    mWorkingView->showGizmoR();
    updateWorkView();
  }
}

void ObjectEditor::hideGizmoS( bool aUpdateViews )
{
  mWorkingView->hideGizmoS();
  if (aUpdateViews)
    updateWorkView();
}

void ObjectEditor::showGizmoS()
{
  if (mAllowsGizmoS)
  {
    hideGizmoT();
    mWorkingView->showGizmoS();
    updateWorkView();
  }
}

void ObjectEditor::createGizmoTEdges()
{
  mGizmoT.mCenter  = mObject->centerMassSelectedEdges();
  if ( mObject->hasBasisOfSelection( mObject->getCurrentSelection() ) )
  {
    std::vector<D3DXVECTOR3> base = mObject->basisOfSelection( mObject->getCurrentSelection() );
    mGizmoT.mBase[0] = base[0];
    mGizmoT.mBase[1] = base[1];
    mGizmoT.mBase[2] = base[2];
  }
  else
  {
    mGizmoT.mBase[0] = D3DXVECTOR3(1,0,0);
    mGizmoT.mBase[1] = D3DXVECTOR3(0,1,0);
    mGizmoT.mBase[2] = D3DXVECTOR3(0,0,1);
  }

}

void ObjectEditor::createGizmoREdges()
{
  mGizmoR.mCenter  = mObject->centerMassSelectedEdges();
  mGizmoR.mBase[0] = D3DXVECTOR3(1,0,0);
  mGizmoR.mBase[1] = D3DXVECTOR3(0,1,0);
  mGizmoR.mBase[2] = D3DXVECTOR3(0,0,1);
}

void ObjectEditor::createGizmoSEdges()
{
  mGizmoS.mCenter  = mObject->centerMassSelectedEdges();
  mGizmoS.mBase[0] = D3DXVECTOR3(1,0,0);
  mGizmoS.mBase[1] = D3DXVECTOR3(0,1,0);
  mGizmoS.mBase[2] = D3DXVECTOR3(0,0,1);
}

void ObjectEditor::createGizmoTFaces()
{
  mGizmoT.mCenter = mObject->centerMassSelectedFaces();

  if ( mObject->hasBasisOfSelection( mObject->getCurrentSelection() ) )
  {
    std::vector<D3DXVECTOR3> base = mObject->basisOfSelection( mObject->getCurrentSelection() );
    mGizmoT.mBase[0] = base[0];
    mGizmoT.mBase[1] = base[1];
    mGizmoT.mBase[2] = base[2];
  }
  else
  {
    mGizmoT.mBase[0] = D3DXVECTOR3(1,0,0);
    mGizmoT.mBase[1] = D3DXVECTOR3(0,1,0);
    mGizmoT.mBase[2] = D3DXVECTOR3(0,0,1);
  }
}

void ObjectEditor::createGizmoRFaces()
{
  mGizmoR.mCenter = mObject->centerMassSelectedFaces();
  mGizmoR.mBase[0] = D3DXVECTOR3(1,0,0);
  mGizmoR.mBase[1] = D3DXVECTOR3(0,1,0);
  mGizmoR.mBase[2] = D3DXVECTOR3(0,0,1);
}

void ObjectEditor::createGizmoSFaces()
{
  mGizmoS.mCenter = mObject->centerMassSelectedFaces();
  mGizmoS.mBase[0] = D3DXVECTOR3(1,0,0);
  mGizmoS.mBase[1] = D3DXVECTOR3(0,1,0);
  mGizmoS.mBase[2] = D3DXVECTOR3(0,0,1);
}

int ObjectEditor::modeOp() const
{
  return mOpMode;
}

int ObjectEditor::mode() const
{
  return mMode;
}


D3DXVECTOR3 ObjectEditor::highlightedSegmentPoint()
{
  return mHighlightedSegmentPoint;
}

D3DXVECTOR3 ObjectEditor::highlightedPoint()
{
  return mHighlightedPoint;
}

ObjEd::Selection ObjectEditor::entityHighlighted()
{
  return mHighlighted;
}

std::vector<int> ObjectEditor::entityHighlightedDEBUG()
{
  std::vector<int> vec;
  if ( mHighlighted.typeEntity == eModeVertex)
    vec.push_back( mHighlighted.idEntity );
  else
    vec = mObject->DEBUGlistEntity( mHighlighted);

  return vec;
}


void ObjectEditor::endOperation()
{  
  if (!mOperationUnfinished)
    return;

  if ( mOpMode == eOpModeExtrude && mMode == eModeFace )
  {
    endOpExtrude();
  }

  if ( mOpMode == eOpModeBevel && mMode == eModeVertex )
  {
    endOpBevelV();
  }
  if ( mOpMode == eOpModeBevel && mMode == eModeEdge )
  {
    endOpBevelE();
  }

  if ( mOpMode == eOpModeKniveLine)
  {
    endOpKniveLine();
  }

  if ( mOpMode == eOpModeNormalScale && mMode == eModeFace )
  {
    endOpNorScale();
  }

  //unselectEntities();
  endOpGeneric();
}

MeshInfo ObjectEditor::infoMeshText()
{
  MeshInfo m = mObject->infoMeshText();
  m.lastKnowSize = mFileLastKnowSize;
  return m;
}

bool ObjectEditor::hasRenderModel()
{
  return mHasRenderModel;
}

bool ObjectEditor::allowsGizmoT()
{
  return mAllowsGizmoT;
}

bool ObjectEditor::allowsGizmoR()
{
  return mAllowsGizmoR;
}

bool ObjectEditor::allowsGizmoS()
{
  return mAllowsGizmoS;
}

void ObjectEditor::hideGizmo()
{
  hideGizmoT( false );
  hideGizmoR( false );
  hideGizmoS( false  );
}

void ObjectEditor::optionsOpKnifeHalf(bool v)
{
  mOptionKnifeHalf = v;
}

void ObjectEditor::optionsOpKnifeHalf( int c )
{
  mOptionKnifeHalfCut = c;
}

void ObjectEditor::optionsOpKnifeFirstPoint( bool v )
{
  mOptionKnifeIsFirstPoint = v;
}


void ObjectEditor::opExtOptions(int aMode)
{
  mOptionExtrudeMode = aMode;
}


void ObjectEditor::createGizmoTObject()
{
  mGizmoT.mCenter = mObject->centerMassSelectedObjects();
  mGizmoT.mBase[0] = D3DXVECTOR3(1,0,0);
  mGizmoT.mBase[1] = D3DXVECTOR3(0,1,0);
  mGizmoT.mBase[2] = D3DXVECTOR3(0,0,1);
}

void ObjectEditor::createGizmoRObject()
{
  mGizmoR.mCenter = mObject->centerMassSelectedObjects();
  mGizmoR.mBase[0] = D3DXVECTOR3(1,0,0);
  mGizmoR.mBase[1] = D3DXVECTOR3(0,1,0);
  mGizmoR.mBase[2] = D3DXVECTOR3(0,0,1);
}

void ObjectEditor::createGizmoSObject()
{
  mGizmoS.mCenter = mObject->centerMassSelectedObjects();
  mGizmoS.mBase[0] = D3DXVECTOR3(1,0,0);
  mGizmoS.mBase[1] = D3DXVECTOR3(0,1,0);
  mGizmoS.mBase[2] = D3DXVECTOR3(0,0,1);
}

void ObjectEditor::updateOperationOptions(Options::ExtrudeOptions &opEo, Options::ExtInnerOptions &opEi, Options::ExtMatrixOptions &opEm)
{
  mOptionsExtrude       = opEo;
  mOptionsExtrudeInner  = opEi;
  mOptionsExtrudeMatrix = opEm;
}

void ObjectEditor::addNewPrimitive()
{
  mObject->newPrimitive( OpId::ePrimCube );
}

bool ObjectEditor::isEntitySelected( const ObjEd::Selection& aEntity)
{
  return mObject->isEntitySelected( aEntity );
}

float ObjectEditor::stepOperation() const
{
  return mOperationStep;
}

int ObjectEditor::renderMode()
{
  return mRenderMode;
}

void ObjectEditor::setProcTex(const std::vector<ObjEd::ProcTex> &aProcTex)
{
  mObject->setProcTex( aProcTex );
  emit updateViews();
}

std::vector<ObjEd::ProcTex> ObjectEditor::procTex() const
{
  return mObject->procTex();
}

bool ObjectEditor::hasProcTex() const
{
  if ( mObject == NULL )
    return false;
  else
    return mObject->hasProcTex();
}

void ObjectEditor::newProcTex()
{
  mObject->newProcTex();
}

void ObjectEditor::setCurrentTex(const int aTexId)
{
  mProcTexId = aTexId;
}

void ObjectEditor::changeRenderMode()
{
  if (mRenderMode == eTransparentMode)
    mRenderMode = ePhongMode;
  else
    if (mRenderMode == ePhongMode)
      mRenderMode = eTransparentMode;

  if (mRenderMode == eProcTexMode)
    mRenderMode = eColTexMode;
  else
    if (mRenderMode == eColTexMode)
      mRenderMode = eProcTexMode;

}

bool ObjectEditor::hasSelectionOfEntities()
{
  std::vector<ObjEd::Selection> s = mObject->getCurrentSelection();
  return (!s.empty());
}

void ObjectEditor::endOpGeneric()
{
  saveEditorState();
  saveToFile( QString("Crash_Recovery.ngm") );
  mOperationUnfinished = false;
}

