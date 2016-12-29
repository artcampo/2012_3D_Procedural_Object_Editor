#include "QD3DWidgetObj.hpp"


QD3DWidgetObj::QD3DWidgetObj(QWidget *parent, UINT width, UINT height, ObjView* aView,ObjectEditor* aEditor ):
    QWidget(parent)
{
  mEditor = aEditor;
  setMouseTracking(true);
  app = new ObjEdRenderApp(winId(), width, height);
  mWidth   = width;
  mHeight  = height;
  mView    = aView;
  setAttribute( Qt::WA_OpaquePaintEvent, true);
  setAttribute( Qt::WA_PaintOnScreen, true);
  //setAttribute( Qt::WA_Resized, true);  //does nothing


  //mLayout = new QVBoxLayout(this);


  mMouseLastPos    = QCursor::pos();

  mDragWithGizmoT = false;
  mDragWithGizmoR = false;
  mDragWithGizmoS = false;
  mDragOperation  = false;
  mDragSelection  = false;
  mDragCamera     = false;
  mDragTextureAdd = false;

  mKniveLineNumSelectedEntities = 0;
  mKniveLinePlane = NULL;
  mShowKniveLinePlane = false;

  mShowGizmoT = false;
  mGizmoT = NULL;
  mShowGizmoR = false;
  mGizmoR = NULL;
  mShowGizmoS = false;
  mGizmoS = NULL;

  mGrid = NULL;
  mAxis = NULL;

  mBackgroundImage = NULL;
  mMuteBackground = false;

  mMaximumModels = 100;
  mMaximumModelsProcTex = 100;

  mModels.resize( mMaximumModels );
  mModelAllocated.resize( mMaximumModels );

  mModelsProcTex.resize( mMaximumModelsProcTex );
  mModelAllocatedProcTex.resize( mMaximumModelsProcTex );

  for (int i = 0; i < mMaximumModels; ++i)
  {
    mModelAllocated[i]  = false;
    mModels[i]       = NULL;
  }

  for (int i = 0; i < mMaximumModelsProcTex; ++i)
  {
    mModelAllocatedProcTex[i]  = false;
    mModelsProcTex[i]       = NULL;
  }

  init();

  mEditor->optionsOpKnifeFirstPoint( true );

}

QD3DWidgetObj::~QD3DWidgetObj()
{
  delete mGizmoT;
  delete mGizmoR;
  delete mGizmoS;

  delete mGrid;
  delete mAxis;
  delete mKniveLinePlane;

  delete app;
  delete mView;
}

void QD3DWidgetObj::recomputeGrid( )
{

  if (mGrid != NULL)
    delete mGrid;

  D3DXVECTOR3 u = mView->getTangentVectorU();
  D3DXVECTOR3 v = mView->getTangentVectorV();
  float   scale = mView->getGridScale();

  int xlen;
  int ylen;
  if ( scale > 60)
  {
    xlen = 41;
    ylen = 41;
  }
  if ( scale > 10)
  {
    xlen = 81;
    ylen = 81;
  }
  else
  {
    xlen = 381;
    ylen = 381;
  }

  D3DXVECTOR3 center = u*scale*(xlen/2) + v*scale*(ylen/2);

  std::vector<D3DXVECTOR3> verticesAxis( 4 );
  verticesAxis[0] = +u*scale*(ylen/2 - 1) ;
  verticesAxis[1] = -u*scale*(ylen/2 - 1) ;
  verticesAxis[2] = +v*scale*(xlen/2 - 1) ;
  verticesAxis[3] = -v*scale*(xlen/2 - 1) ;

  std::vector<D3DXVECTOR3> vertices( (xlen + ylen)*2 );

  for ( int i = 0; i < xlen; ++i)
  {
    vertices[i*2 + 0] = u*scale*i - center;
    vertices[i*2 + 1] = u*scale*i + v*scale*(xlen - 1) - center;
    vertices[i*2 + 0 + xlen*2] = v*scale*i - center;
    vertices[i*2 + 1 + xlen*2] = v*scale*i + u*scale*(ylen - 1) - center;
  }

  mGrid = new GridModel(
        vertices,
        verticesAxis,
        D3DXCOLOR( 0.5f, 0.5f, 0.5f, 0.3f ),
        //D3DXCOLOR( 1.0f, 0.5f, 0.5f, 0.3f ),
        app->getD3DXdevice()
        );

}

void QD3DWidgetObj::recomputeAxis( )
{

  if (mAxis != NULL)
    delete mAxis;

  std::vector<D3DXVECTOR3> vertices( 6 );

  D3DXVECTOR3 center = mView->getCamAt();
  float distance = 8.0f;
  center +=  mView->getScale()*0.1f*-14.0f*mView->getCamTangentU();
  center +=  mView->getScale()*0.1f*+8.0f*mView->getCamTangentV();
  //center = D3DXVECTOR3(0,0,0);

  D3DXVECTOR3 i = D3DXVECTOR3(1.0f,0.0f,0.0f);
  D3DXVECTOR3 j = D3DXVECTOR3(0.0f,1.0f,0.0f);
  D3DXVECTOR3 k = D3DXVECTOR3(0.0f,0.0f,1.0f);


  vertices[0] = center;
  vertices[1] = center + mView->getScale()*0.1f*i;

  vertices[2] = center;
  vertices[3] = center + mView->getScale()*0.1f*j;

  vertices[4] = center;
  vertices[5] = center + mView->getScale()*0.1f*k;

  mAxis = new AxisModel(
        vertices,
        app->getD3DXdevice()
        );

}

bool QD3DWidgetObj::init()
{
 bool ret = app->init();
 recomputeGrid();
 recomputeAxis();
 return ret;
}

bool QD3DWidgetObj::reset()
{
  delete app;
  app = new ObjEdRenderApp(winId(), mWidth, mHeight);

  return app->init ();

}



void QD3DWidgetObj::paintEvent( QPaintEvent* pEvent)
{
  if ( mEditor->renderMode() == ObjEd::eTransparentMode )
  {
    app->drawScene( mView->getCamPos(),
                    mView->getCamAt(),
                    mView->getUpVector(),
                    mGrid,
                    mAxis,
                    mView->getTitle(),
                    mMuteBackground,
                    mView->getGridScale(),
                    mShowGizmoT,
                    mGizmoT,
                    mShowGizmoR,
                    mGizmoR,
                    mShowGizmoS,
                    mGizmoS,
                    mShowKniveLinePlane,
                    mKniveLinePlane
                    );
  }

  if ( mEditor->renderMode() == ObjEd::ePhongMode )
  {
    app->drawScenePhong(

                    mView->getCamPos(),
                    mView->getCamAt(),
                    mView->getUpVector(),
                    mGrid,
                    mAxis,
                    mView->getTitle(),
                    mMuteBackground,
                    mView->getGridScale(),
                    mShowGizmoT,
                    mGizmoT,
                    mShowGizmoR,
                    mGizmoR,
                    mShowGizmoS,
                    mGizmoS,
                    mShowKniveLinePlane,
                    mKniveLinePlane
                    );
  }

  if ( mEditor->renderMode() == ObjEd::eProcTexMode )
  {
    app->drawSceneTextured(

                    mView->getCamPos(),
                    mView->getCamAt(),
                    mView->getUpVector(),
                    mGrid,
                    mAxis,
                    mView->getTitle(),
                    mMuteBackground,
                    mView->getGridScale(),
                    mShowGizmoT,
                    mGizmoT,
                    mShowGizmoR,
                    mGizmoR,
                    mShowGizmoS,
                    mGizmoS,
                    mShowKniveLinePlane,
                    mKniveLinePlane
                    );
  }

  update();
}

void QD3DWidgetObj::createModel( EditableMeshModel* aModel,
                              const int aIndex )
{

  if (aIndex > mMaximumModels)
  {
    mModelAllocated.resize( aIndex + 1 );
    mModels.resize( aIndex + 1 );
    for (int i = mMaximumModels; i < aIndex ; ++i)
    {
      mModelAllocated[i] = false;
      mModels[i] = NULL;
    }
    mMaximumModels = aIndex + 1;
  }

  if(!mModelAllocated[aIndex])
  {
    mModelAllocated[aIndex] = true;
    mModels[aIndex]=NULL;
  }

  if (mModels[aIndex]!=NULL)
    delete mModels[aIndex];

  mModels[aIndex] = aModel;

  setModels();
}

void QD3DWidgetObj::createModelProcTex( EditableMeshModelProcTex* aModel,
                              const int aIndex )
{

  if (aIndex > mMaximumModelsProcTex)
  {
    mModelAllocatedProcTex.resize( aIndex + 1 );
    mModelsProcTex.resize( aIndex + 1 );
    for (int i = mMaximumModelsProcTex; i < aIndex ; ++i)
    {
      mModelAllocatedProcTex[i] = false;
      mModelsProcTex[i] = NULL;
    }
    mMaximumModelsProcTex = aIndex + 1;
  }

  if(!mModelAllocatedProcTex[aIndex])
  {
    mModelAllocatedProcTex[aIndex] = true;
    mModelsProcTex[aIndex]=NULL;
  }

  if (mModelsProcTex[aIndex]!=NULL)
    delete mModelsProcTex[aIndex];

  mModelsProcTex[aIndex] = aModel;

  setModels();
}

void QD3DWidgetObj::setModels()
{
  app->setModels( mModels );
  app->setModels( mModelsProcTex );
}

D3DXVECTOR3 QD3DWidgetObj::projectPoint ( QPoint Apos )
{
  int posx = Apos.x();
  int posy = Apos.y();
  float px = posx / float(width()-1);
  float py = posy / float(height()-1);
  px -= 0.5f;
  py -= 0.5f;
  px *= 2.0f;
  py *= -2.0f;
  D3DXVECTOR3 p = mView->projectPoint( px, py );
  return p;
}


void QD3DWidgetObj::mousePressEvent ( QMouseEvent * event )
{
  if ( mEditor->modeOp() == eOpModeKniveLine && event->modifiers() != Qt::AltModifier)
  {
    mousePressEventKniveLine(event);
    return;
  }

  if ( mEditor->modeOp() == eOpModeTexture && event->modifiers() != Qt::AltModifier)
  {
    mousePressEventTexture(event);
    return;
  }

  // EXPERIMENTAL
  if ( mEditor->allowsSelection() &&
       event->modifiers() != Qt::ControlModifier)
  {

    mDragSelection = true;

  }

  //  Case 1: moving camera
  if (event->modifiers() == Qt::AltModifier)
    return;

  QPoint p = event->pos();

  //  Case 21: drag gizmo
  //  Check gizmo  
  mDragWithGizmoT = checkGizmoT(p);
  if ( mDragWithGizmoT )
  {
    mEditor->startOperationGizmoT();
    return;
  }

  //  Case 22: drag gizmo
  mDragWithGizmoR = checkGizmoR(p);
  if ( mDragWithGizmoR )
  {
    mEditor->startOperationGizmoR();
    return;
  }

  //  Case 23: drag gizmo
  mDragWithGizmoS = checkGizmoS(p);
  if ( mDragWithGizmoS )
  {
    mEditor->startOperationGizmoS();
    return;
  }


  //  Case 3: select entity
  //  Check entities

  bool entityHighlighted = checkEntity(p);

  //  Case 3.1: select entity
  if ( entityHighlighted &&
       mEditor->allowsSelection() &&
       event->modifiers() != Qt::ShiftModifier &&
       event->modifiers() != Qt::ControlModifier)
  {
    if ( mEditor->modeOp() !=  ObjEd::eOpModeSelection)
      mEditor->unselectEntities();
    mEditor->selectEntityHighlighted();
    createGizmo();
    mDragSelection = true;
  }

  //  Case 3.2: addSelect entity
  if ( entityHighlighted &&
       mEditor->allowsModifySelection() &&
       event->modifiers() == Qt::ShiftModifier )
  {
    mEditor->selectAddEntityHighlighted();
    createGizmo();
  }

  //  Case 3.3: removeSelect entity
  if ( entityHighlighted &&
       mEditor->allowsModifySelection() &&
       event->modifiers() == Qt::ControlModifier )
  {
    mEditor->selectRemoveEntityHighlighted();
    createGizmo();
  }

  if (entityHighlighted)
    mDragOperation = false;

  //  Case 4: drag operation
  if ( !entityHighlighted &&
       mEditor->allowsOperation() &&
       event->modifiers() != Qt::ShiftModifier &&
       event->modifiers() != Qt::ControlModifier)
  {
    if (mEditor->modeOp() == ObjEd::eOpModeSelection)
    {
      mDragSelection = true;
    }
    else
    {
      mDragOperation = true;
      mEditor->startOperation();
    }
    return;
  }



}

void QD3DWidgetObj::mouseMoveEvent ( QMouseEvent * event )
{
  if ( mEditor->modeOp() == eOpModeKniveLine &&
       event->modifiers() != Qt::AltModifier )
  {
    mouseMoveEventKniveLine(event);
    return;
  }

  if ( mEditor->modeOp() == eOpModeTexture &&
       event->modifiers() != Qt::AltModifier )
  {
    mouseMoveEventTexture(event);
    return;
  }




  QPoint p = event->pos();

  if ( mDragSelection  &&
       !mDragWithGizmoT &&
       !mDragWithGizmoR &&
       !mDragWithGizmoS &&
       !mDragOperation &&
       event->modifiers() != Qt::AltModifier)
  {
    bool entityHighlighted = checkEntity(p);
    if ( entityHighlighted &&
         mEditor->allowsSelection() &&
         //!mEditor->allowsModifySelection() &&
         event->modifiers() != Qt::ShiftModifier &&
         event->modifiers() != Qt::ControlModifier)
    {
      //mEditor->unselectEntities();
      mEditor->selectEntityHighlighted();
      createGizmo();
      return;
    }

    if ( entityHighlighted && mEditor->allowsModifySelection() )
      {
       mEditor->selectAddEntityHighlighted();
       createGizmo();
       return;
      }

  }

  //  Move without buttons
  if ( event->buttons() != Qt::LeftButton &&
       event->buttons() != Qt::RightButton &&
       event->buttons() != Qt::MiddleButton )
  {
    bool gizmoHighlighted;
    if( mEditor->allowsGizmoT() )
      gizmoHighlighted = checkGizmoT(p);
    if( mEditor->allowsGizmoR() )
      gizmoHighlighted = checkGizmoR(p);
    if( mEditor->allowsGizmoS() )
      gizmoHighlighted = checkGizmoS(p);


    bool entityHighlighted = checkEntity(p);



    if ( entityHighlighted )
    {
      ObjEd::Selection h =  mEditor->entityHighlighted();
      std::ostringstream st;
      st << ObjEd::gsModeNames[h.typeEntity] << " ";
      st << "Id: " << h.idEntity;
      if ( h.typeEntity == eModeEdge || h.typeEntity == eModeFace )
      {
        std::vector<int> ids = mEditor->entityHighlightedDEBUG();
        st << " ( Internal: ";
        for(  unsigned int i =0 ; i< ids.size(); ++i)
          st << ids[i] <<" ";
        st << ")";
      }
      QString text ( st.str().c_str() );
      GlobalEditor::gEditor->updateStatusBarByTab ( text );
    }
    else
    {
      MeshInfo m = mEditor->infoMeshText();

      std::ostringstream description;
      description << "Nvertices: " << m.nv << " ";
      description << "Nedges: " << m.ne << " ";
      description << "Nfaces: " << m.nf << " ";
      description << "Size in bytes: " << m.lastKnowSize << " ";
      QString text ( description.str().c_str() );
      GlobalEditor::gEditor->updateStatusBarByTab ( text );
    }

    mMouseLastPos = event->pos();
    return;
  }

  //  Move with buttons

  //  Camera movement
  if(event->modifiers() == Qt::AltModifier)
  {
    mDragCamera = true;
    //  Scale
    if(event->buttons() == Qt::RightButton)
    {
      QPoint difference = event->pos() - mMouseLastPos;
      mMouseLastPos = event->pos();

      mView->scaleUpdate( difference.y() );
      recomputeGrid();
      recomputeAxis();
      updateGizmoModel();
    }
    // Panning
    if(event->buttons() == Qt::MiddleButton)
    {
      QPoint difference = event->pos() - mMouseLastPos;
      mMouseLastPos = event->pos();
      mView->panUpdate( difference.x()/ float(width()-1),
                  difference.y()/float(height()-1));
      recomputeAxis();
    }
    // Orbiting
    if(event->buttons() == Qt::LeftButton &&
       mView->allowsOrbitation())
    {
      QPoint difference = event->pos() - mMouseLastPos;
      mMouseLastPos = event->pos();
      mView->orbitUpdate( difference.y(), difference.x());
      recomputeAxis();
    }
  }
  else
  {
    QPoint differenceOp = event->pos() - mMouseLastPos;


    //  Check gizmo
    if ( event->buttons() == Qt::LeftButton &&
         ( mDragWithGizmoT || mDragWithGizmoR || mDragWithGizmoS))
    {

      D3DXVECTOR3 rayb = D3DXVECTOR3( event->pos().x(), event->pos().y(), 0.0f );
      D3DXVec3Unproject( &rayb,
                         &rayb,
                         &app->mViewport,
                         &app->mProjection,
                         &app->mView,
                         &app->mWorld);


      D3DXVECTOR3 raye = D3DXVECTOR3( event->pos().x(), event->pos().y(), 1.0f );
      D3DXVec3Unproject( &raye,
                         &raye,
                         &app->mViewport,
                         &app->mProjection,
                         &app->mView,
                         &app->mWorld);

      D3DXVECTOR3 rayb2 = D3DXVECTOR3( mMouseLastPos.x(), mMouseLastPos.y(), 0.0f );
      D3DXVec3Unproject( &rayb2,
                         &rayb2,
                         &app->mViewport,
                         &app->mProjection,
                         &app->mView,
                         &app->mWorld);


      D3DXVECTOR3 raye2 = D3DXVECTOR3( mMouseLastPos.x(), mMouseLastPos.y(), 1.0f );
      D3DXVec3Unproject( &raye2,
                         &raye2,
                         &app->mViewport,
                         &app->mProjection,
                         &app->mView,
                         &app->mWorld);

      if ( mDragWithGizmoT )
      {
        mEditor->performOperationGizmoT( rayb, raye, rayb2, raye2, mView->cameraTangetU(), mView->cameraTangetV() );
        mEditor->createGizmoT();
        updateGizmoTModel();
      }
      if ( mDragWithGizmoR )
      {
        mEditor->performOperationGizmoR( rayb, raye, rayb2, raye2, mView->cameraTangetU(), mView->cameraTangetV() );
        mEditor->createGizmoR();
        updateGizmoRModel();
      }
      if ( mDragWithGizmoS )
      {
        mEditor->performOperationGizmoS( rayb, raye, rayb2, raye2, mView->cameraTangetU(), mView->cameraTangetV() );
        mEditor->createGizmoS();
        updateGizmoSModel();
      }

      //TODO: do not call this if it has to be updated at the end of the drag
      if ( mEditor->renderMode() != eProcTexMode )
        mEditor->updateWorkView();
    }
    else
      //  Do operation
      if ( event->buttons() == Qt::LeftButton &&
           mDragOperation &&
           mEditor->allowsOperation() )
      {
        if (event->modifiers() == Qt::ShiftModifier )
        {
          //  Apply steps
          float steppedX, steppedY;
          steppedX = differenceOp.x() * mEditor->stepOperation();
          steppedY = differenceOp.y() * mEditor->stepOperation();

          mEditor->performOperation( steppedX, steppedY );
        }
        else
          mEditor->performOperation( differenceOp.x(),
                                     differenceOp.y());

        std::ostringstream description;
        description << "Value: " << mEditor->operationValue();
        QString text ( description.str().c_str() );
        GlobalEditor::gEditor->updateStatusBarByTab ( text );
    }
  }

  mMouseLastPos = event->pos();
}

void QD3DWidgetObj::mouseReleaseEvent ( QMouseEvent * event )
{
  if (mDragOperation)
    mEditor->endOperation();

  if (mDragWithGizmoT)
    mEditor->endOperationGizmoT();

  if (mDragWithGizmoR)
    mEditor->endOperationGizmoR();

  if (mDragWithGizmoS)
    mEditor->endOperationGizmoS();

  if (mDragCamera)
  {
    if ( mEditor->renderMode() != eProcTexMode )
      mEditor->updateWorkView();
  }

  if (mDragTextureAdd)
    mEditor->endOpTexture();

  mDragOperation  = false;
  mDragWithGizmoT = false;
  mDragWithGizmoR = false;
  mDragWithGizmoS = false;
  mDragSelection  = false;
  mDragTextureAdd = false;
}


void QD3DWidgetObj::eraseBackgroundImage()
{/*
  if(mBackgroundImage != NULL)
  {
    app->releaseBackgroundImageTexture();
    delete mBackgroundImage;
    mBackgroundImage = NULL;
  }*/
}

void QD3DWidgetObj::createBackgroundImage(std::vector<D3DXVECTOR3> &aPoints, float aAlpha)
{/*
  mBackgroundImage = new ImageQuadModel( aPoints, aAlpha, app->getD3DXdevice() );
  app->setBackgroundImageModel( mBackgroundImage );*/
}


void QD3DWidgetObj::createBackgroundImageTexture(int w, int h, unsigned char *data)
{
  //app->setBackgroundImageTexture(w,h,data);
}

void QD3DWidgetObj::eraseAllModels()
{

  for (int i = 0; i < mMaximumModels ; ++i)
  {
    if (mModelAllocated[i])
    {
      mModelAllocated[i] = false;
      delete mModels[i];
      mModels[i] = NULL;
    }
  }
  app->setModels(mModels);

}

void QD3DWidgetObj::eraseAllModelsProcTex()
{

  for (int i = 0; i < mMaximumModelsProcTex ; ++i)
  {
    if (mModelAllocatedProcTex[i])
    {
      mModelAllocatedProcTex[i] = false;
      delete mModelsProcTex[i];
      mModelsProcTex[i] = NULL;
    }
  }
  app->setModels(mModelsProcTex);

}

void QD3DWidgetObj::muteBackgroundImage()
{
  if (mMuteBackground)
    mMuteBackground = false;
  else
    mMuteBackground = true;
}

void QD3DWidgetObj::setGridScaleMultiplier(float aMult )
{
  mView->setGridScaleMultiplier(aMult);
  recomputeGrid();
}

ID3D10Device * QD3DWidgetObj::getD3DXdevice()
{
  return app->getD3DXdevice();
}

void QD3DWidgetObj::setMode(int aMode)
{
  mMode = aMode;
}

bool QD3DWidgetObj::checkEntity ( QPoint p )
{
  D3DXVECTOR3 p1, p2;
  D3DXVECTOR3 u = D3DXVECTOR3( p.x(), p.y(), 0.0f );
  D3DXVec3Unproject( &p1,
                     &u,
                     &app->mViewport,
                     &app->mProjection,
                     &app->mView,
                     &app->mWorld);
  u = D3DXVECTOR3( p.x(), p.y(), 1.0f );
  D3DXVec3Unproject( &p2,
                     &u,
                     &app->mViewport,
                     &app->mProjection,
                     &app->mView,
                     &app->mWorld);

  return mEditor->checkEntity ( p1, p2 );
}

bool QD3DWidgetObj::checkGizmoT ( QPoint p )
{
  if ( mShowGizmoT )
  {
    D3DXVECTOR3 p1, p2;
    D3DXVECTOR3 u = D3DXVECTOR3( p.x(), p.y(), 0.0f );
    D3DXVec3Unproject( &p1,
                       &u,
                       &app->mViewport,
                       &app->mProjection,
                       &app->mView,
                       &app->mWorld);
    u = D3DXVECTOR3( p.x(), p.y(), 1.0f );
    D3DXVec3Unproject( &p2,
                       &u,
                       &app->mViewport,
                       &app->mProjection,
                       &app->mView,
                       &app->mWorld);

    bool updateGizmo;
    bool ret = mEditor->checkGizmoT ( p1, p2, updateGizmo );
    if (updateGizmo)
      updateGizmoTModel();
    return ret;
  }
  else
    return false;
}

bool QD3DWidgetObj::checkGizmoR ( QPoint p )
{
  if ( mShowGizmoR )
  {
    D3DXVECTOR3 p1, p2;
    D3DXVECTOR3 u = D3DXVECTOR3( p.x(), p.y(), 0.0f );
    D3DXVec3Unproject( &p1,
                       &u,
                       &app->mViewport,
                       &app->mProjection,
                       &app->mView,
                       &app->mWorld);
    u = D3DXVECTOR3( p.x(), p.y(), 1.0f );
    D3DXVec3Unproject( &p2,
                       &u,
                       &app->mViewport,
                       &app->mProjection,
                       &app->mView,
                       &app->mWorld);

    bool updateGizmo;
    bool ret = mEditor->checkGizmoR ( p1, p2, updateGizmo );
    if (updateGizmo)
      updateGizmoRModel();
    return ret;
  }
  else
    return false;
}

bool QD3DWidgetObj::checkGizmoS ( QPoint p )
{
  if ( mShowGizmoS )
  {
    D3DXVECTOR3 p1, p2;
    D3DXVECTOR3 u = D3DXVECTOR3( p.x(), p.y(), 0.0f );
    D3DXVec3Unproject( &p1,
                       &u,
                       &app->mViewport,
                       &app->mProjection,
                       &app->mView,
                       &app->mWorld);
    u = D3DXVECTOR3( p.x(), p.y(), 1.0f );
    D3DXVec3Unproject( &p2,
                       &u,
                       &app->mViewport,
                       &app->mProjection,
                       &app->mView,
                       &app->mWorld);

    bool updateGizmo;
    bool ret = mEditor->checkGizmoS ( p1, p2, updateGizmo );
    if (updateGizmo)
      updateGizmoSModel();
    return ret;
  }
  else
    return false;
}


void QD3DWidgetObj::updateGizmoModel()
{
  if (mShowGizmoT)
    updateGizmoTModel();
  if (mShowGizmoR)
    updateGizmoRModel();
  if (mShowGizmoS)
    updateGizmoSModel();
}

void QD3DWidgetObj::updateGizmoTModel()
{
  if (mGizmoT != NULL)
    delete mGizmoT;
  mGizmoT = mEditor->gizmoT( app->getD3DXdevice(), mView->getScale() );

}

void QD3DWidgetObj::updateGizmoRModel()
{
  if (mGizmoR != NULL)
    delete mGizmoR;
  mGizmoR = mEditor->gizmoR( app->getD3DXdevice(), mView->getScale() );

}

void QD3DWidgetObj::updateGizmoSModel()
{

  if (mGizmoS != NULL)
    delete mGizmoS;
  mGizmoS = mEditor->gizmoS( app->getD3DXdevice(), mView->getScale() );

}

float QD3DWidgetObj::camScale()
{
  return mView->getScale();
}

void QD3DWidgetObj::hideGizmoT()
{
  mShowGizmoT = false;
}

void QD3DWidgetObj::showGizmoT()
{
  mShowGizmoT = true;
}

void QD3DWidgetObj::hideGizmoR()
{
  mShowGizmoR = false;
}

void QD3DWidgetObj::showGizmoR()
{
  mShowGizmoR = true;
}

void QD3DWidgetObj::hideGizmoS()
{
  mShowGizmoS = false;
}

void QD3DWidgetObj::showGizmoS()
{
  mShowGizmoS = true;
}

void QD3DWidgetObj::mousePressEventKniveLine(QMouseEvent *event)
{
  QPoint p = event->pos();

  bool vertexHighlight;
  bool edgeHighlight = false;

  mEditor->setMode( eModeVertex );
  vertexHighlight = checkEntity(p);
  if ( !vertexHighlight )
  {
    mEditor->setMode( eModeEdge );
    edgeHighlight = checkEntity(p);
  }

  if ( vertexHighlight || edgeHighlight )
  {
    mKniveEntity[ mKniveLineNumSelectedEntities ] = mEditor->entityHighlighted();

    if ( mKniveLineNumSelectedEntities == 0)
    {
      if ( vertexHighlight )
      {
        mKniveLinePoint[0] = mEditor->highlightedPoint();
        mKniveLinePoint[1] = mEditor->highlightedPoint();
      }
      if ( edgeHighlight )
      {
        mKniveLinePoint[0] = mEditor->highlightedSegmentPoint();
        mKniveLinePoint[1] = mEditor->highlightedSegmentPoint();
      }
      ++mKniveLineNumSelectedEntities;
      mEditor->optionsOpKnifeFirstPoint( false );
    }
    else
    if ( mKniveLineNumSelectedEntities == 1)
    {
      if ( vertexHighlight )
      {
        mKniveLinePoint[1] = mEditor->highlightedPoint();
      }
      if ( edgeHighlight )
      {
        mKniveLinePoint[1] = mEditor->highlightedSegmentPoint();
      }      

      if ( mKniveEntity[0].idComponent ==  mKniveEntity[1].idComponent )
      {
        D3DXVECTOR3 point = projectScreenRay ( p.x(), p.y(), 1.0f );

        KniveLineInfo info;
        info.segmentPointB = mKniveLinePoint[0];
        info.segmentPointE = mKniveLinePoint[1];
        info.entityB  = mKniveEntity[0];
        info.entityE  = mKniveEntity[1];

        mKniveLineNumSelectedEntities = 0;
        mShowKniveLinePlane = false;


        mEditor->startOperation();
        mEditor->startOpKniveLine( info );
        mEditor->performOpKniveLine( info, app->mView, mView->getCamPos() );
        mEditor->endOperation();
        //mEditor->endOpKniveLine();

        mEditor->optionsOpKnifeFirstPoint( true );
      }

    }


  }
}

void QD3DWidgetObj::mouseMoveEventKniveLine(QMouseEvent *event)
{
  QPoint p = event->pos();

  mEditor->setMode( eModeVertex );
  bool vertexHighlight = checkEntity(p);
  bool edgeHighlight = false;
  if ( !vertexHighlight )
  {
    mEditor->setMode( eModeEdge );
    edgeHighlight = checkEntity(p);    
  }

  if ( mKniveLineNumSelectedEntities == 1 )
  {    
    D3DXVECTOR3 p1 = projectScreenRay ( p.x(), p.y(), 0.0f );
    D3DXVECTOR3 p2 = projectScreenRay ( p.x(), p.y(), 1.0f );

    mKniveLinePoint[1] = p1 + 10.0f*(p2-p1);
    createKniveLine();
    mShowKniveLinePlane = true;
  }
}

void QD3DWidgetObj::createKniveLine()
{

  if (mKniveLinePlane != NULL)
    delete mKniveLinePlane;

  std::vector<D3DXVECTOR3> v(2);
  v[0] = mKniveLinePoint[0];
  v[1] = mKniveLinePoint[1];
  mKniveLinePlane = new LineListModel( v, D3DXCOLOR(0.2f, 0.2f, 0.2f, 1), app->getD3DXdevice() );

}

/////////////////////////////////////////////////////////////
//  texture

void QD3DWidgetObj::mousePressEventTexture(QMouseEvent *event)
{
  mDragTextureAdd = true;
  mEditor->startOpTexture();
  mouseMoveEventTexture( event );
}

void QD3DWidgetObj::mouseMoveEventTexture(QMouseEvent *event)
{
  QPoint p = event->pos();

  bool faceHighlight = checkEntity(p);
  if ( faceHighlight && mDragTextureAdd )
  {
    ObjEd::Selection sel = mEditor->entityHighlighted();

    if ( sel.typeEntity == eModeFace)
      mEditor->performOpTexture( sel.idComponent, sel.idEntity );
  }

}

D3DXVECTOR3 QD3DWidgetObj::projectScreenRay(float x, float y, float z)
{
  D3DXVECTOR3 u = D3DXVECTOR3(x,y,z);
  D3DXVec3Unproject( &u,
                     &u,
                     &app->mViewport,
                     &app->mProjection,
                     &app->mView,
                     &app->mWorld);
  return u;
}

void QD3DWidgetObj::createGizmo()
{
  if ( mEditor->allowsGizmoT())
  {
    mEditor->createGizmoT();
    updateGizmoTModel();
  }
  if ( mEditor->allowsGizmoR())
  {
    mEditor->createGizmoR();
    updateGizmoRModel();
  }
  if ( mEditor->allowsGizmoS())
  {
    mEditor->createGizmoS();
    updateGizmoSModel();
  }
}

void QD3DWidgetObj::cancelKnifeOperation()
{
  mKniveLineNumSelectedEntities = 0;
  mShowKniveLinePlane = false;
}


