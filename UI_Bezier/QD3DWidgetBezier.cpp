#include "QD3DWidgetBezier.hpp"


QD3DWidgetBezier::QD3DWidgetBezier(QWidget *parent, UINT width, UINT height, const ViewManager& aView ):
    QWidget(parent)
{
  setMouseTracking(true);
  app = new RenderViewApp(winId(), width, height);
  mWidth   = width;
  mHeight  = height;
  mView    = new ViewManager(aView);
  setAttribute( Qt::WA_OpaquePaintEvent, true);
  setAttribute( Qt::WA_PaintOnScreen, true);

  mMouseDragMiddle = false;
  mMouseDragRight  = false;
  mMouseDragLeft   = false;

  mDraggingPoint    = false;
  mDraggingTangent  = false;
  mDragHasModified  = false;

  mGrid = NULL;
  mAxis = NULL;
  mPairTangentsModel = NULL;
  mBackgroundImage = NULL;
  mMuteBackground = false;

  mMaximumModels = 100;

  mBezModels.resize( mMaximumModels );
  mModelAllocated.resize( mMaximumModels );

  for (int i = 0; i < mMaximumModels; ++i)
  {
    mModelAllocated[i]  = false;
    mBezModels[i]       = NULL;
  }
}

QD3DWidgetBezier::~QD3DWidgetBezier()
{
  delete app;
  delete mView;
}

void QD3DWidgetBezier::recomputeGrid( )
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

void QD3DWidgetBezier::recomputeAxis( )
{
  if (mAxis != NULL)
    delete mAxis;

  std::vector<D3DXVECTOR3> vertices( 6 );

  D3DXVECTOR3 center = mView->getCamAt();
  if ( !mView->isFreeView())
  {
    center +=  mView->getScale()*0.1f*-8.0f*mView->getTangentVectorU();
    center -=  mView->getScale()*0.1f*8.0f*mView->getTangentVectorV();
  }

  vertices[0] = center;
  vertices[1] = center + mView->getScale()*0.1f*D3DXVECTOR3(1.0f,0.0f,0.0f);

  vertices[2] = center;
  vertices[3] = center + mView->getScale()*0.1f*D3DXVECTOR3(0.0f,1.0f,0.0f);

  vertices[4] = center;
  vertices[5] = center + mView->getScale()*0.1f*D3DXVECTOR3(0.0f,0.0f,1.0f);

  mAxis = new AxisModel(
        vertices,
        app->getD3DXdevice()
        );
}

bool QD3DWidgetBezier::init()
{
 bool ret = app->init();
 recomputeGrid();
 recomputeAxis();
 return ret;
}

bool QD3DWidgetBezier::reset()
{
  delete app;
  app = new RenderViewApp(winId(), mWidth, mHeight);

  return app->init ();

}

D3DXVECTOR3  QD3DWidgetBezier::getSelectionMask()
{
  return mView->getSelectionMask();
}

void QD3DWidgetBezier::paintEvent( QPaintEvent* pEvent)
{
  app->drawScene( mView->getCamPos(),
                  mView->getCamAt(),
                  mView->getUpVector(),
                  mGrid,
                  mAxis,
                  mPairTangentsModel,
                  mView->getTitle(),
                  mMuteBackground,
                  mView->getGridScale()
                  );
  update();
}

void QD3DWidgetBezier::createModel( std::vector<D3DXVECTOR3>& aPoints,
                              std::vector<D3DXVECTOR3>& aPointsCurve,
                              D3DXCOLOR& aColour,
                              const int aIndex )
{
  if (aIndex > mMaximumModels)
  {
    mModelAllocated.resize( aIndex + 1 );
    mBezModels.resize( aIndex + 1 );
    for (int i = mMaximumModels; i < aIndex ; ++i)
    {
      mModelAllocated[i] = false;
      mBezModels[i] = NULL;
    }
    mMaximumModels = aIndex + 1;
  }

  if(!mModelAllocated[aIndex])
  {
    mModelAllocated[aIndex] = true;
    mBezModels[aIndex]=NULL;
  }

  if (mBezModels[aIndex]!=NULL)
    delete mBezModels[aIndex];

  mBezModels[aIndex] = new BezierModel( aPoints,
                                   aPointsCurve,
                                   aColour,
                                   app->getD3DXdevice(),
                                   GlobalEditor::gEditor->hasPointSelected(),
                                   GlobalEditor::gEditor->getIndexPointSelected()
                                   );

}

void QD3DWidgetBezier::setModels()
{
  app->setBezModels( mBezModels );
}

D3DXVECTOR3 QD3DWidgetBezier::projectPoint ( QPoint Apos )
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


void QD3DWidgetBezier::mousePressEvent ( QMouseEvent * event )
{
  GlobalEditor::gEditor->selectWorkView( this );

  QPoint pos = event->pos();
  D3DXVECTOR3 p = projectPoint(pos);
  bool pointSelected = false;

  if(event->button() == Qt::LeftButton &&
     mView->allowsSelection() &&
     GlobalEditor::gEditor->allowsSelection())
    {
    pointSelected = GlobalEditor::gEditor->selectPointIfNear(p);
    }

  if(event->button() == Qt::LeftButton &&
     mView->allowsSelection() &&
     GlobalEditor::gEditor->allowsTangentDragging() &&
     !pointSelected)
    {
    pointSelected = GlobalEditor::gEditor->hasTangentNear(p);
    mDraggingTangent  = true;
    }

  if(!pointSelected)
    if(event->button() == Qt::LeftButton &&
       mView->allowsAppend() &&
       GlobalEditor::gEditor->allowsAppend() &&
       !GlobalEditor::gEditor->hasPointNear(p) &&
       !GlobalEditor::gEditor->hasTangentNear(p) )
      {
        if ( GlobalEditor::gEditor->adjustPointToGrid() )
          p = mView->adjustPointToGrid (p);

        GlobalEditor::gEditor->appendPoint ( p );
        mDraggingTangent  = true;
      }

}

void QD3DWidgetBezier::mouseMoveEvent ( QMouseEvent * event )
{
  GlobalEditor::gEditor->selectWorkView( this );


  //  Zooming
  if(event->buttons() == Qt::RightButton)
  {
    if (!mMouseDragRight)
    {
      mMouseDragRight = true;
      mMouseLastPos = event->pos();
    }
    else
    {
      QPoint difference = event->pos() - mMouseLastPos;
      mMouseLastPos = event->pos();

      mView->scale( difference.y() );
      recomputeGrid();
      recomputeAxis();
    }
  }

  //  Panning
  if(event->buttons() == Qt::MiddleButton &&
     mView->allowsPan())
  {
    if (!mMouseDragMiddle)
    {
      mMouseDragMiddle = true;
      mMouseLastPos = event->pos();
    }
    else
    {
      QPoint difference = event->pos() - mMouseLastPos;
      mMouseLastPos = event->pos();

      mView->pan( difference.x()/ float(width()-1),
                  difference.y()/float(height()-1));

      recomputeAxis();
    }
  }

  //  Orbiting
  if(event->buttons() == Qt::LeftButton &&
     mView->allowsOrbitation())
  {
    if (!mMouseDragLeft)
    {
      mMouseDragLeft = true;
      mMouseLastPos = event->pos();
    }
    else
    {
      QPoint difference = event->pos() - mMouseLastPos;
      mMouseLastPos = event->pos();
      //mView->orbit( difference.x(), difference.y());
      mView->orbit( difference.y(), difference.x());
      recomputeAxis();
    }
  }

  //  Project
  D3DXVECTOR3 p = projectPoint( event->pos() );

  //  Point displacement
  if(event->buttons() == Qt::LeftButton &&
     GlobalEditor::gEditor->allowsPointDragging() &&
     //GlobalEditor::gEditor->hasPointNear(p) &&
     !mDraggingTangent)
  {
    if ( GlobalEditor::gEditor->adjustPointToGrid() )
      p = mView->adjustPointToGrid (p);
    GlobalEditor::gEditor->dragSelectedPoint( p );
    mDraggingPoint    = true;
    mDragHasModified  = true;
  }

  //  Tangent displacement, broken one
  if(event->buttons() == Qt::LeftButton &&
     (event->modifiers() == Qt::ShiftModifier) &&
     GlobalEditor::gEditor->allowsTangentDragging() &&
     //GlobalEditor::gEditor->hasTangentNear(p) &&
     !mDraggingPoint
     )
  {
    GlobalEditor::gEditor->selectTangentIfNear(p);
    GlobalEditor::gEditor->dragSelectedTangent( p );
    mDraggingTangent  = true;
    mDragHasModified  = true;
  }

  if(event->buttons() == Qt::LeftButton &&
     (event->modifiers() != Qt::ShiftModifier) &&
     GlobalEditor::gEditor->allowsTangentDragging() &&
     //GlobalEditor::gEditor->hasTangentNear(p) &&
     !mDraggingPoint
     )
  {
    GlobalEditor::gEditor->selectTangentIfNear(p);
    GlobalEditor::gEditor->dragSelectedTangentCorrectOpposite( p );
    mDraggingTangent  = true;
    mDragHasModified  = true;
  }


  std::ostringstream st;
  st << "Position: " << p.x << ","<< p.y << ","<< p.z;
  QString cursorPosition ( st.str().c_str() );


  GlobalEditor::gEditor->updateStatusBarByTab ( cursorPosition );
}

void QD3DWidgetBezier::mouseReleaseEvent ( QMouseEvent * event )
{
  if (mDragHasModified)
    GlobalEditor::gEditor->dragFinished();

  GlobalEditor::gEditor->selectWorkView( this );
  GlobalEditor::gEditor->updateWorkView();

  mMouseDragMiddle = false;
  mMouseDragRight  = false;
  mMouseDragLeft   = false;

  mDraggingPoint    = false;
  mDraggingTangent  = false;

  mDragHasModified  = false;
}

void QD3DWidgetBezier::eraseTangentModel()
{
  if (mPairTangentsModel != NULL)
    delete mPairTangentsModel;
  mPairTangentsModel = NULL;
}

void QD3DWidgetBezier::createTangentModel(std::vector<D3DXVECTOR3> &aPoints, D3DXCOLOR &aColour)
{
  eraseTangentModel();
  mPairTangentsModel = new PairTangentsModel(
        aPoints,
        aColour,
        app->getD3DXdevice()
        );
}

D3DXVECTOR3 QD3DWidgetBezier::getUnitUmultScale()
{
  return (mView->getTangentVectorU() * mView->getScale());
}

void QD3DWidgetBezier::eraseBackgroundImage()
{
  if(mBackgroundImage != NULL)
  {
    app->releaseBackgroundImageTexture();
    delete mBackgroundImage;
    mBackgroundImage = NULL;
  }
}

void QD3DWidgetBezier::createBackgroundImage(std::vector<D3DXVECTOR3> &aPoints, float aAlpha)
{
  mBackgroundImage = new ImageQuadModel( aPoints, aAlpha, app->getD3DXdevice() );
  app->setBackgroundImageModel( mBackgroundImage );
}

std::vector<D3DXVECTOR3> QD3DWidgetBezier::computeBackgroundImageVertices( float aScaleMult )
{
  std::vector<D3DXVECTOR3> v (4);

  float mult = 60.0f*aScaleMult;
  v[0] =  mView->getScale()*mult*0.1f*mView->getTangentVectorU()
        + mView->getScale()*mult*0.1f*mView->getTangentVectorV() ;

  v[1] = -mView->getScale()*mult*0.1f*mView->getTangentVectorU()
        + mView->getScale()*mult*0.1f*mView->getTangentVectorV() ;

  v[2] = -mView->getScale()*mult*0.1f*mView->getTangentVectorU()
        - mView->getScale()*mult*0.1f*mView->getTangentVectorV() ;

  v[3] = mView->getScale()*mult*0.1f*mView->getTangentVectorU()
       - mView->getScale()*mult*0.1f*mView->getTangentVectorV() ;

  return v;
}

void QD3DWidgetBezier::createBackgroundImageTexture(int w, int h, unsigned char *data)
{
  app->setBackgroundImageTexture(w,h,data);
}

void QD3DWidgetBezier::eraseAllModels()
{
  for (int i = 0; i < mMaximumModels ; ++i)
  {
    if (mModelAllocated[i])
    {
      mModelAllocated[i] = false;
      delete mBezModels[i];
      mBezModels[i] = NULL;
    }
  }
  app->setBezModels(mBezModels);

}

void QD3DWidgetBezier::muteBackgroundImage()
{
  if (mMuteBackground)
    mMuteBackground = false;
  else
    mMuteBackground = true;
}

void QD3DWidgetBezier::setGridScaleMultiplier(float aMult )
{
  mView->setGridScaleMultiplier(aMult);
  recomputeGrid();
}
