#include "QD3DWidgetDirG.hpp"


QD3DWidgetDirG::QD3DWidgetDirG(QWidget *parent, UINT width, UINT height, ObjViewDG* aView,DirGraphEditor* aEditor ):
    QWidget(parent)
{
  mEditor = aEditor;
  setMouseTracking(true);
  app = new DirGraphEdRenderApp(winId(), width, height);
  mWidth   = width;
  mHeight  = height;
  mView    = aView;
  setAttribute( Qt::WA_OpaquePaintEvent, true);
  setAttribute( Qt::WA_PaintOnScreen, true);

  mView->mCamPan    = D3DXVECTOR2(0.5,-0.5f);
  mView->mCamScale  = 5.0f;

  mGizmoTModel = NULL;
  mMouseLastPos    = QCursor::pos();

  mDragMove       = false;
  mDragCamera     = false;
  mDragWithGizmoT = false;
  mShowGizmoT     = false;

  mShowNewEdgeLine = true;
  mNewEdgeLine = NULL;

  mGrid = NULL;
  mAxis = NULL;

  mScaleGizmoT = 10.0f;
  mThresholdGizmoT = 0.6f;

  initMisc();
  init();


}

QD3DWidgetDirG::~QD3DWidgetDirG()
{
  delete mGrid;
  delete mAxis;
  delete app;
  delete mView;
}

void QD3DWidgetDirG::recomputeGrid( )
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

void QD3DWidgetDirG::recomputeAxis( )
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

bool QD3DWidgetDirG::init()
{
 bool ret = app->init();
 recomputeGrid();
 recomputeAxis();
 return ret;
}

bool QD3DWidgetDirG::reset()
{
  delete app;
  app = new DirGraphEdRenderApp(winId(), mWidth, mHeight);

  return app->init ();

}



void QD3DWidgetDirG::paintEvent( QPaintEvent* pEvent)
{
  if ( mNewEdgeLine )
    delete mNewEdgeLine;
  std::vector<D3DXVECTOR3> v;

  int id = mEditor->selectedVertex();

  v.push_back( mEditor->vertexPos(id) );
  v.push_back( projectPoint( mMouseLastPos ) );

  mNewEdgeLine = new LineListModel( v, D3DXCOLOR(0,1,0,1), app->getD3DXdevice() );

  app->drawScene( mView->getCamPos(),
                  mView->getCamAt(),
                  mView->getUpVector(),
                  mGrid,
                  mAxis,
                  mShowGizmoT,
                  mGizmoTModel,
                  mView->getTitle(),
                  true,
                  mView->getGridScale(),
                  mShowNewEdgeLine,
                  mNewEdgeLine
                  );

  update();
}


D3DXVECTOR3 QD3DWidgetDirG::projectPoint ( QPoint Apos )
{

  D3DXVECTOR3 to;
  to = mView->unitVector();
  D3DXVec3Cross( &to, &to, &mView->getCamTangentU()  );
  Intersections::Plane plane( mView->getCamTangentU(), to, mView->getCamAt() );

  D3DXVECTOR3 p1, p2;
  D3DXVECTOR3 u = D3DXVECTOR3( Apos.x(), Apos.y(), 0.0f );
  D3DXVec3Unproject( &p1,
                     &u,
                     &app->mViewport,
                     &app->mProjection,
                     &app->mView,
                     &app->mWorld);
  u = D3DXVECTOR3( Apos.x(), Apos.y(), 1.0f );
  D3DXVec3Unproject( &p2,
                     &u,
                     &app->mViewport,
                     &app->mProjection,
                     &app->mView,
                     &app->mWorld);

  bool unique;
  D3DXVECTOR3 intersect = Intersections::intersectionLinePlane(p1,p2, plane, unique);

  return intersect;
}


void QD3DWidgetDirG::mousePressEvent ( QMouseEvent * event )
{
  //  Case 1: moving camera
  if (event->modifiers() == Qt::AltModifier)
    return;

  QPoint p = event->pos();
  if ( event->buttons() == Qt::LeftButton )
  {
    if ( checkVertex( p ) )
    {
      int targetVertex = getVertex(p);
      if ( targetVertex != mEditor->selectedVertex() )
      {
        mEditor->createEdge( mEditor->selectedVertex(), targetVertex );      
      }
    }
    else
    {
      int newId = mEditor->createVertex( projectPoint(p) );
      mEditor->createEdge( mEditor->selectedVertex(), newId );
      mEditor->changeSelectedVertex( newId );

    }
  }

  if ( event->buttons() == Qt::RightButton )
  {

    if ( checkVertex( p ) )
    {
      mDraggedVertexId = getVertex(p);
      mEditor->changeSelectedVertex( mDraggedVertexId );
      mDragMove = true;
    }

    mDragWithGizmoT = checkGizmoT( p );
    if ( mDragWithGizmoT )
    {
      mDraggedVertexId = mEditor->selectedVertex();
    }
  }

  createGizmoT();
  updateGizmoTModel();

}

void QD3DWidgetDirG::mouseMoveEvent ( QMouseEvent * event )
{
  QPoint p = event->pos();

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

  if ( event->buttons() != Qt::LeftButton &&
       event->buttons() != Qt::RightButton &&
       !mDragWithGizmoT)
  {
      checkGizmoT(p);
  }

  //  Check gizmo
  if ( event->buttons() == Qt::RightButton && mDragWithGizmoT )
  {
    D3DXVECTOR3 rayb1, raye1;
    D3DXVECTOR3 rayb2, raye2;
    mousePosToRay( mMouseLastPos, rayb1, raye1 );
    mousePosToRay( p, rayb2, raye2 );

    D3DXVECTOR3 pos1 = mGizmoT.newPosition( rayb1,
                                            raye1,
                                            mView->cameraTangetU(),
                                            mView->cameraTangetV());
    D3DXVECTOR3 pos2 = mGizmoT.newPosition( rayb2,
                                            raye2,
                                            mView->cameraTangetU(),
                                            mView->cameraTangetV());

    D3DXVECTOR3 pos;
    pos  = mEditor->vertexPos( mEditor->selectedVertex() );
    pos += (pos2 - pos1);

    mEditor->toggleAdjustGrid( false );
    mEditor->modifyVertex( mDraggedVertexId, pos );
    mEditor->toggleAdjustGrid( true );
    createGizmoT();
    updateGizmoTModel();
    mEditor->updateWorkView();

  }
  else
  {
    if ( mDragMove )
    {
      mEditor->modifyVertex( mDraggedVertexId, projectPoint(p) );
      createGizmoT();
      updateGizmoTModel();
      mEditor->updateWorkView();
    }
    else
    {
      D3DXVECTOR3 p1, p2;
      mousePosToRay( p, p1, p2 );
      mEditor->highlightEntity( p1, p2, mView->getCamPos() );
      createGizmoT();
      updateGizmoTModel();
      mEditor->updateWorkView();
    }
  }


  mMouseLastPos = event->pos();
}

void QD3DWidgetDirG::mouseReleaseEvent ( QMouseEvent * event )
{
  if (mDragCamera)
  {
    mEditor->updateWorkView();
  }

  mDragCamera     = false;
  mDragMove       = false;
  mDragWithGizmoT = false;
}



ID3D10Device * QD3DWidgetDirG::getD3DXdevice()
{
  return app->getD3DXdevice();
}

float QD3DWidgetDirG::camScale()
{
  return mView->getScale();
}


bool QD3DWidgetDirG::checkVertex ( QPoint p )
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

  return mEditor->checkVertex ( p1, p2, app->mView, mView->getCamPos() );
}

int QD3DWidgetDirG::getVertex( QPoint p )
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

  return mEditor->getVertex ( p1, p2, app->mView, mView->getCamPos() );
}

void QD3DWidgetDirG::mousePosToRay ( QPoint p, D3DXVECTOR3& p1, D3DXVECTOR3& p2 )
{
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
}

void QD3DWidgetDirG::initMisc()
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


  mColours.fac      = D3DXCOLOR( 0.2f, 0.2f, 0.8f, 0.2f );
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


void QD3DWidgetDirG::updateGizmoTModel()
{
  if (mGizmoTModel != NULL)
    delete mGizmoTModel;
  mGizmoT.setScale( mScaleGizmoT );
  mGizmoTModel = mGizmoT.convert( app->getD3DXdevice(), &mColours);
  mShowGizmoT = true;

}

void QD3DWidgetDirG::createGizmoT()
{
  mGizmoT.mCenter = mEditor->vertexPos( mEditor->selectedVertex() );
  mGizmoT.mBase[0] = D3DXVECTOR3(1,0,0);
  mGizmoT.mBase[1] = D3DXVECTOR3(0,1,0);
  mGizmoT.mBase[2] = D3DXVECTOR3(0,0,1);
}

bool QD3DWidgetDirG::checkGizmoT( QPoint& p )
{
  D3DXVECTOR3 rayB;
  D3DXVECTOR3 rayE;
  bool updateGizmo;
  mousePosToRay( p, rayB, rayE );
  bool collided = checkGizmoT( rayB, rayE, updateGizmo );
  if (updateGizmo)
    updateGizmoTModel();
  return collided;
}

bool QD3DWidgetDirG::checkGizmoT(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, bool& aUpdateGizmo )
{
  bool highlighted = false;

  bool previousState = mGizmoT.hasHighlight();
  highlighted = mGizmoT.check( aRayB, aRayE, mThresholdGizmoT );

  aUpdateGizmo = (previousState != highlighted);

  return highlighted;
}
