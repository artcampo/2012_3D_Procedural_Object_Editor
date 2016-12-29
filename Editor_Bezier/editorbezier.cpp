#include "editorbezier.h"
#include "UI/MainWindow.hpp"

EditorBezier::EditorBezier()
{

  mBezSelectedColour    = D3DXCOLOR( 0.5f, 0.5f, 0.5f, 1.0f );
  mBezNotSelectedColour = D3DXCOLOR( 0.7f, 0.7f, 0.7f, 1.0f );
  mDistanceThresholdSelect =3.5f;
  mDistanceThresholdSelectOnDrag =1.5f;


  mAtLeastOnePointExists  = false;
  mOnePointIsSelected     = false;
  mTangentsBeingDisplayed = false;
  mTangentSelected        = false;

  mModeSelect = false;
  mModeCreate = false;
  mModeInsert = false;
  mModeMove   = false;
  mAdjustToGrid = false;

  mOptionWhenMovingPointTangentsShouldBeContinous = true;

  mWorkingView = NULL;

  mFirstFreeUniqueId  = 0;
  mUndoPosition       = 0;
  mDefaultAlphaForBackgroundImage = 0.4f;
}

EditorBezier::~EditorBezier()
{
  mBackgroundImageTexture.resize(0);
}

void EditorBezier::init()
{
  createNewSpline();
  createNewSpline();
  mSelectedSpline = 0;
  modeCreate();
}

void EditorBezier::setMovingPointTangentsForceContinous(const bool aValue)
{
  mOptionWhenMovingPointTangentsShouldBeContinous = aValue;
}


void EditorBezier::updateViews()
{
  //mBezModels[0] = new BezierModel( mBezSplines[0].getPoints(), mBezSelectedColour  );

  GlobalEditor::gViewX->eraseAllModels();
  GlobalEditor::gViewY->eraseAllModels();
  GlobalEditor::gViewZ->eraseAllModels();
  GlobalEditor::gViewF->eraseAllModels();

  bool setModels = false;
  for ( unsigned int i = 0; i < mBezSplines.size() ; ++i)
  {
    if (  mBezSplines[i].getNumPoints() >= 2 )
    {
      setModels = true;
      D3DXCOLOR colour;
      if (mBezSplines[i].isHighlighted())
        colour = mBezSelectedColour;
      else
        colour = mBezNotSelectedColour;

      GlobalEditor::gViewX->createModel( mBezSplines[i].getPoints(), mBezSplines[i].getCurve(), colour, i );
      GlobalEditor::gViewY->createModel( mBezSplines[i].getPoints(), mBezSplines[i].getCurve(), colour, i );
      GlobalEditor::gViewZ->createModel( mBezSplines[i].getPoints(), mBezSplines[i].getCurve(), colour, i );
      GlobalEditor::gViewF->createModel( mBezSplines[i].getPoints(), mBezSplines[i].getCurve(), colour, i );
    }
  }

  if (setModels)
  {
    GlobalEditor::gViewX->setModels();
    GlobalEditor::gViewY->setModels();
    GlobalEditor::gViewZ->setModels();
    GlobalEditor::gViewF->setModels();
  }
}

//  \param  true<=>point was selected
bool EditorBezier::selectPointIfNear(const D3DXVECTOR3 &aP)
{
  D3DXVECTOR3 maskedPoint = applyMask(aP, mWorkingView->getSelectionMask());
  bool pointSelected = false;
  if (allowsSelection())
  {
    for( unsigned int i = 0 ; !pointSelected && (i < mBezSplines.size()) ; ++i )
    {
      for( int j = 0 ; !pointSelected && (j < mBezSplines[i].getNumPoints()) ; ++j )
      {
        D3DXVECTOR3 p = mBezSplines[i].getPoint(j);
        p = applyMask(p, mWorkingView->getSelectionMask());

        float distance = D3DXVec3Length( &(p - maskedPoint));
        if ( distance < mDistanceThresholdSelect )
        {
          pointSelected = true;
          selectBezierCurveAndPoint( i, j );
        }
      }
    }
  }

  return pointSelected;
}

bool EditorBezier::hasPointNear(const D3DXVECTOR3 &aP)
{
  bool pointNear = false;
  D3DXVECTOR3 maskedPoint = applyMask(aP, mWorkingView->getSelectionMask());
  for( unsigned int i = 0 ; !pointNear && (i < mBezSplines.size()) ; ++i )
  {
    for( int j = 0 ; !pointNear && (j < mBezSplines[i].getNumPoints()) ; ++j )
    {
      D3DXVECTOR3 p = mBezSplines[i].getPoint(j);      
      p = applyMask(p, mWorkingView->getSelectionMask());
      float distance = D3DXVec3Length( &(p - maskedPoint));
      if ( distance < mDistanceThresholdSelect )
      {
        pointNear = true;
      }
    }
  }

  return pointNear;
}

void EditorBezier::selectBezierCurveAndPoint(int aBezIndex, int aPointIndex )
{
  mBezSplines[mSelectedSpline].setHighlighted(false);

  mOnePointIsSelected = true;
  mSelectedSpline = aBezIndex;
  mSelectedPoint  = aPointIndex;
  mBezSplines[mSelectedSpline].setHighlighted(true);
  showTangents();
  updateViews();
}

bool EditorBezier::hasPointSelected()
{
  return mOnePointIsSelected;
}

int EditorBezier::getIndexPointSelected()
{
  return mSelectedPoint;
}


bool EditorBezier::allowsAppend()
{
  return mModeCreate;
}

bool EditorBezier::allowsSelection()
{
  return (mAtLeastOnePointExists );
}

bool EditorBezier::allowsTangentDragging()
{
  return (mTangentsBeingDisplayed && mAtLeastOnePointExists && !mModeMove);
}


bool EditorBezier::allowsPointDragging()
{
  return (mOnePointIsSelected && mAtLeastOnePointExists &&
          (mModeSelect || mModeMove));
}

bool EditorBezier::selectedPointHasTangents()
{
    bool noTangents = true;
    if(!mAtLeastOnePointExists)
      noTangents = false;
    else
    {
        if(  mSelectedPoint == 0)
            noTangents = false;
        /*
        if(  mSelectedPoint == (mBezSplines[mSelectedSpline].getNumPoints() - 1 ))
            noTangents = false;
        */
    }
    return noTangents;
}

void EditorBezier::appendPoint( const D3DXVECTOR3& aP )
{
  if (allowsAppend())
  {
    mBezSplines[mSelectedSpline].appendPoint
          ( aP,
            mWorkingView->getUnitUmultScale() );
    mAtLeastOnePointExists = true;
    mOnePointIsSelected    = true;
    mSelectedPoint         = mBezSplines[mSelectedSpline].getNumPoints() - 1;
    mTangentSelected       = true;
    mTangentPrevSelected   = false;
    showTangents();
    updateViews();
    saveEditorState();

  }
}


void EditorBezier::dragSelectedPoint(const D3DXVECTOR3& aP)
{
  //  Drag single point
  if(allowsPointDragging() && !mModeMove)
  {
    if (mOptionWhenMovingPointTangentsShouldBeContinous &&
            selectedPointHasTangents()   )
    {
      mBezSplines[mSelectedSpline].setPointForceTangentContinuity( mSelectedPoint, aP );
    }
    else
    {
      mBezSplines[mSelectedSpline].setPoint( mSelectedPoint, aP );
    }

    if ( mTangentsBeingDisplayed )
    {
      showTangents();
    }
    updateViews();
  }

  //  Drag whole spline
  if(allowsPointDragging() && mModeMove)
  {

    D3DXVECTOR3 difference = aP - mBezSplines[mSelectedSpline].getPoint ( mSelectedPoint );
    mBezSplines[mSelectedSpline].translate( difference );


    if ( mTangentsBeingDisplayed )
    {
      showTangents();
    }
    updateViews();
  }
}

void EditorBezier::dragSelectedTangent(const D3DXVECTOR3& aP)
{
  if (allowsTangentDragging())
  {
    mBezSplines[mSelectedSpline].setTangent( mSelectedPoint, mTangentPrevSelected, aP );
    if ( mTangentsBeingDisplayed )
    {
      showTangents();
    }
    updateViews();
  }
}


void EditorBezier::dragSelectedTangentCorrectOpposite(const D3DXVECTOR3& aP)
{
  if (allowsTangentDragging())
  {
    mBezSplines[mSelectedSpline].setTangentCorrectOpposite( mSelectedPoint, mTangentPrevSelected, aP );
    if ( mTangentsBeingDisplayed )
    {
      showTangents();
    }
    updateViews();
  }
}

void EditorBezier::dragFinished()
{
  saveEditorState();
}

void EditorBezier::showTangents()
{  
  if( selectedPointHasTangents() )
  {
    mTangentsBeingDisplayed = true;
    mWorkingView->eraseTangentModel();
    mWorkingView->createTangentModel(
          mBezSplines[mSelectedSpline].getTangents( mSelectedPoint ),
          D3DXCOLOR( 0.2f, 0.2f, 0.2f, 1.0f )
          );
  }
  else
  {
    mTangentsBeingDisplayed = false;
    //mViewOfSelection->eraseTangentModel();
  }
}

void EditorBezier::unshowTangents()
{
  if( mTangentsBeingDisplayed )
  {
    mTangentsBeingDisplayed = false;
    mWorkingView->eraseTangentModel();
  }
}


bool EditorBezier::selectTangentIfNear(const D3DXVECTOR3 &aP)
{
  D3DXVECTOR3 maskedPoint = applyMask(aP, mWorkingView->getSelectionMask());

  if (allowsSelection())
  {
    std::vector<D3DXVECTOR3> tangents;
    tangents = mBezSplines[mSelectedSpline].getTangentPoints( mSelectedPoint );


    float distance = D3DXVec3Length( &(tangents[0] - maskedPoint));
    if ( distance < mDistanceThresholdSelect )
    {
      mTangentSelected = true;
      mTangentPrevSelected = true;
    }

    distance = D3DXVec3Length( &(tangents[1] - maskedPoint));
    if ( distance < mDistanceThresholdSelect )
    {
      mTangentSelected = true;
      mTangentPrevSelected = false;
    }
  }

  return mTangentSelected;
}

bool EditorBezier::hasTangentNear(const D3DXVECTOR3 &aP)
{
  D3DXVECTOR3 maskedPoint = applyMask(aP, mWorkingView->getSelectionMask());
  bool tangentNear = false;
  if ( mTangentsBeingDisplayed && mOnePointIsSelected )
  {
    std::vector<D3DXVECTOR3> tangents;
    tangents = mBezSplines[mSelectedSpline].getTangentPoints( mSelectedPoint );
    tangents[0] = applyMask(tangents[0], mWorkingView->getSelectionMask());
    tangents[1] = applyMask(tangents[1], mWorkingView->getSelectionMask());

    float distance = D3DXVec3Length( &(tangents[0] - maskedPoint));
    if ( distance < mDistanceThresholdSelect )
    {
      tangentNear = true;
    }

    distance = D3DXVec3Length( &(tangents[1] - maskedPoint));
    if ( distance < mDistanceThresholdSelect )
    {
      tangentNear = true;
    }
  }

  return tangentNear;
}

void EditorBezier::selectWorkView( QD3DWidgetBezier* aView )
{
  if ( mWorkingView != aView )
  {
    if ( mWorkingView != NULL && mTangentsBeingDisplayed)
        mWorkingView->eraseTangentModel();
    mWorkingView = aView;
    updateWorkView();
  }
}



void EditorBezier::updateWorkView()
{
  if ( mTangentsBeingDisplayed )
    showTangents();
  //TODO: unshow /(?)
}

/*
  TODO: should be non-member
*/
D3DXVECTOR3 EditorBezier::applyMask( const D3DXVECTOR3& aPoint, const D3DXVECTOR3& aMask )
{
  D3DXVECTOR3 ret;
  ret.x = aPoint.x * aMask.x;
  ret.y = aPoint.y * aMask.y;
  ret.z = aPoint.z * aMask.z;
  if (ret.x == -0.0f) ret.x = 0.0f;
  if (ret.y == -0.0f) ret.y = 0.0f;
  if (ret.z == -0.0f) ret.z = 0.0f;
  return ret;
}

int EditorBezier::getUniqueId()
{
  int ret = mFirstFreeUniqueId;
  ++mFirstFreeUniqueId;
  return ret;
}

std::string EditorBezier::getUniqueName()
{
  std::string ret;
  bool  found  = false;
  int   number = 0;
  while ( !found )
  {
    ret = getSplineName(number);
    if( !objectNameExists(ret))
      found = true;
    if(!found)
    {
      ++number;
    }
  }
  return ret;
}

std::string EditorBezier::getSplineName( int i )
{
  std::stringstream ret;
  ret << "Spline " << i;
  return ret.str();
}

bool EditorBezier::objectNameExists ( const std::string& aName )
{
  bool  found  = false;

  for ( unsigned int i = 0; (i < mBezSplines.size()) && !found ; ++i)
  {
    if ( mBezSplines[i].name() == aName )
      found = true;
  }

  return found;
}

int EditorBezier::idObjectViaName ( const std::string& aName )
{
  bool  found  = false;
  int id;

  for ( unsigned int i = 0; (i < mBezSplines.size()) && !found ; ++i)
  {
    if ( mBezSplines[i].name() == aName )
    {
      found = true;
      id = i;
    }
  }

  return id;
}

////////////////////////////////////////////////////////////////////////
/*
  Objects handling
*/
////////////////////////////////////////////////////////////////////////
void  EditorBezier::createNewSpline()
{
  int index = mBezSplines.size();

  mSelectedSpline = index;
  mOnePointIsSelected = false;
  mTangentsBeingDisplayed = false;

  mBezSplines.resize( index + 1 );
  mBezSplines[index] = Bezier( getUniqueId(), getUniqueName() );


  GlobalEditor::gObjTree->addObjectEditor( mBezSplines[index].name() );
  updateViews();
}

void  EditorBezier::copyCurrentSpline()
{
  int index = mBezSplines.size();

  mBezSplines.resize( index + 1 );
  mBezSplines[index] = Bezier( getUniqueId(), getUniqueName(), mBezSplines[mSelectedSpline] );

  mSelectedSpline = index;
  mOnePointIsSelected = false;
  mTangentsBeingDisplayed = false;

  GlobalEditor::gObjTree->addObjectEditor( mBezSplines[index].name() );
  updateViews();
}

void EditorBezier::selectBezierCurve(const std::string &aName)
{
  int id = idObjectViaName(aName);
  selectBezierCurveAndPoint(id,0);
}

void EditorBezier::saveEditorState()
{
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

  mUndoStates[index] = new EditorBezierState( mBezSplines, mFirstFreeUniqueId );
  ++mUndoPosition;
}

void EditorBezier::undoEditorState()
{
  if ( undoPossible())
  {
    if ( mUndoPosition == mUndoStates.size())
      mUndoPosition -= 2;
    else
      --mUndoPosition;
    mUndoStates[mUndoPosition]->restoreBezierState( mBezSplines, mFirstFreeUniqueId );
    updateStateAndViews();
  }
}

void EditorBezier::redoEditorState()
{
  if ( redoPossible())
  {
   ++mUndoPosition;
   mUndoStates[mUndoPosition]->restoreBezierState( mBezSplines, mFirstFreeUniqueId );
   updateStateAndViews();
  }
}

bool EditorBezier::undoPossible()
{
  return (mUndoPosition > 2);
}

bool EditorBezier::redoPossible()
{
  return (mUndoPosition < (mUndoStates.size() - 1));
}

void EditorBezier::updateStateAndViews()
{
  unshowTangents();
  mSelectedSpline = 0;
  mTangentsBeingDisplayed = false;
  mOnePointIsSelected = false;
  updateViews();

  std::vector<std::string> names = getObjectsNames();
  GlobalEditor::gObjTree->initObjectEditor( names );
}

std::vector<std::string> EditorBezier::getObjectsNames()
{
  std::vector<std::string> ret (mBezSplines.size());
  for ( unsigned int i = 0; i < mBezSplines.size() ; ++i)
  {
    ret[i] = mBezSplines[i].name();
  }
  return ret;
}

void EditorBezier::loadFromFile(QFile &aFile)
{
  QDataStream stream(&aFile);

  int numBlocks;
  int numBezierSplines;

  QString s,t;
  stream >> s;
  stream >> numBlocks;
  stream >> t;
  stream >> numBezierSplines;

  mBezSplines.resize( numBezierSplines );
  for ( unsigned int i = 0; i < numBezierSplines ; ++i)
  {
    stream >> mBezSplines[i];
  }

  mFirstFreeUniqueId = numBezierSplines;
  modeCreate();
  updateStateAndViews();
}

void EditorBezier::saveToFile(QFile &aFile)
{
  QDataStream stream(&aFile);

  QString title ("Not Gordian Editor v0.1");
  stream << title;
  stream << (int)1;

  QString bezBlock ("Bezier Block");
  stream << bezBlock;

  stream << mBezSplines.size();

  for ( unsigned int i = 0; i < mBezSplines.size() ; ++i)
  {
    stream << mBezSplines[i];
  }

}

void EditorBezier::setBackgroundImageFileName(QString &aFileName, float aAlpha, float aSizeMult )
{
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
  //mBackgroundImageVertices = GlobalEditor::gViewX->computeBackgroundImageVertices();
  //mBackgroundImageVertices = GlobalEditor::gViewZ->computeBackgroundImageVertices();

  GlobalEditor::gViewX->eraseBackgroundImage();
  GlobalEditor::gViewX->createBackgroundImage( mBackgroundImageVertices, aAlpha );
  GlobalEditor::gViewX->createBackgroundImageTexture( mBmpw, mBmph, (unsigned char*) (&mBackgroundImageTexture[0]) );

  GlobalEditor::gViewY->eraseBackgroundImage();
  GlobalEditor::gViewY->createBackgroundImage( mBackgroundImageVertices, aAlpha );
  GlobalEditor::gViewY->createBackgroundImageTexture( mBmpw, mBmph, (unsigned char*) (&mBackgroundImageTexture[0]) );

  GlobalEditor::gViewZ->eraseBackgroundImage();
  GlobalEditor::gViewZ->createBackgroundImage( mBackgroundImageVertices, aAlpha );
  GlobalEditor::gViewZ->createBackgroundImageTexture( mBmpw, mBmph, (unsigned char*) (&mBackgroundImageTexture[0]) );

  GlobalEditor::gViewF->eraseBackgroundImage();
  GlobalEditor::gViewF->createBackgroundImage( mBackgroundImageVertices, aAlpha );
  GlobalEditor::gViewF->createBackgroundImageTexture( mBmpw, mBmph, (unsigned char*) (&mBackgroundImageTexture[0]) );

  updateViews();
}

void EditorBezier::exportToFile(QFile &aFile)
{
  QTextStream stream(&aFile);

  stream << "namespace SplinesData{" << endl << endl;

  //  Export Splines
  for ( unsigned int i = 0; i < mBezSplines.size() ; ++i)
  {
    mBezSplines[i].exportToC(stream);
  }

  //  Export Sizes
  stream << endl <<  "int splinesSize [" << mBezSplines.size() << "] = { ";
  for ( unsigned int i = 0; i < mBezSplines.size() ; ++i)
  {
    stream << mBezSplines[i].getNumStoredElements();
    if (i != ( mBezSplines.size() - 1))
      stream << ", ";
  }
  stream << "}; " << endl;

  //  Export table to points
  stream << endl <<  "D3DXVECTOR3* splinesArray [" << mBezSplines.size() << "] = { ";
  for ( unsigned int i = 0; i < mBezSplines.size() ; ++i)
  {
    stream << "&SplinesData::Spline"<< mBezSplines[i].getId()<<"[0]";
    if (i != ( mBezSplines.size() - 1))
      stream << ", "<<endl;
  }
  stream << "}; " << endl;

  stream << endl<< "}; // end of namespace SplinesData";
  stream.flush();
}

void EditorBezier::exportToFileLastPoint(QFile &aFile)
{
  QTextStream stream(&aFile);


  //  Export Splines
  stream << "D3DXVECTOR3 point[" <<mBezSplines.size()<<"] = {"<< endl;
  for ( unsigned int i = 0; i < mBezSplines.size() ; ++i)
  {
    mBezSplines[i].exportToCLast(stream);
    if (i != (mBezSplines.size() - 1))
      stream << "," << endl;
  }

  stream << "}; " << endl;


  stream.flush();
}

void EditorBezier::modeSelect()
{
  modeClearPrevious();
  mModeSelect = true;
}

void EditorBezier::modeCreate()
{
  modeClearPrevious();
  mModeCreate = true;
}

void EditorBezier::modeInsert()
{
  modeClearPrevious();
  mModeInsert = true;
}

void EditorBezier::modeMove()
{
  modeClearPrevious();
  mModeMove = true;
}


void EditorBezier::modeClearPrevious()
{
  if ( mModeSelect )
  {
    mModeSelect = false;
    unshowTangents();
  }
  if ( mModeCreate )
  {
    mModeCreate = false;
  }
  if ( mModeInsert )
  {
    mModeInsert = false;
  }
  if (mModeMove)
  {
    mModeMove = false;
  }
}

void EditorBezier::deleteObject()
{
  if ( mModeMove )
    deleteSelectedSpline();
  else
    if ( mModeSelect && mOnePointIsSelected  )
    {
      deleteSelectedPoint();
    }
}

void EditorBezier::deleteSelectedSpline()
{
  unshowTangents();
  if (mBezSplines.size() > 1)
  {
    std::vector<Bezier>::iterator Iter;
    Iter = mBezSplines.begin() + mSelectedSpline;
    mBezSplines.erase(Iter);
    saveEditorState();
    updateStateAndViews();
  }
}

void EditorBezier::deleteSelectedPoint()
{
  if (mBezSplines[mSelectedSpline].getNumPoints() > 2)
  {
    mBezSplines[mSelectedSpline].deletePoint(mSelectedPoint);
    saveEditorState();
    updateStateAndViews();
  }
}

void EditorBezier::muteBackgroundImage()
{
  GlobalEditor::gViewY->muteBackgroundImage();
  GlobalEditor::gViewX->muteBackgroundImage();
  GlobalEditor::gViewZ->muteBackgroundImage();
  GlobalEditor::gViewF->muteBackgroundImage();
}

void EditorBezier::toggleAdjustToGrid()
{
  if ( mAdjustToGrid )
    mAdjustToGrid = false;
  else
    mAdjustToGrid = true;
}

bool EditorBezier::adjustPointToGrid()
{
  return mAdjustToGrid;
}

void EditorBezier::setGridMultiplier(float aMult)
{
  GlobalEditor::gViewY->setGridScaleMultiplier(aMult);
  GlobalEditor::gViewX->setGridScaleMultiplier(aMult);
  GlobalEditor::gViewZ->setGridScaleMultiplier(aMult);
  GlobalEditor::gViewF->setGridScaleMultiplier(aMult);
}

void EditorBezier::closeAllSplines()
{
  for ( unsigned int i = 0; i < mBezSplines.size() ; ++i)
  {
    mBezSplines[i].close();
  }
  saveEditorState();
  updateStateAndViews();
}

void EditorBezier::updateStatusBarByTab( QString aName )
{
  GlobalEditor::gMainWindow->updateStatusBarByTab(aName);
}
