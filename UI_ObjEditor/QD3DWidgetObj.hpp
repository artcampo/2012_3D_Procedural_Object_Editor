#ifndef QD3DWIDGET_OBJ_HPP
#define QD3DWIDGET_OBJ_HPP

#include <QWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QApplication>

#include <QSizeGrip>
#include <QVBoxLayout>

#include <sstream>

#include "ObjView.h"

#include "App/dx_misc.hpp"
#include "APP/ObjEdRenderApp.hpp"

//TODO: these should be removed. only to access status bar
#include "Editor_Bezier/EditorGlobal.hpp"
#include "Editor_Bezier/editorbezier.h"
//end

#include "App/EditableMeshModel.h"
#include "App/EditableMeshModelProcTex.h"
#include "App/GridModel.h"
#include "App/pairtangentsmodel.h"
#include "App/AxisModel.h"
#include "App/imagequadmodel.hpp"

#include "ObjEditor/ObjEditorSharedData.hpp"
#include "ObjEditor/objectEditor.h"

#include "App/ObjEd/GizmoTMeshModel.hpp"

class MainWindow;

class QD3DWidgetObj : public QWidget
{
    Q_OBJECT
public:
  explicit
  QD3DWidgetObj(QWidget *parent, UINT width, UINT height, ObjView* aView, ObjectEditor* aEditor );
  ~QD3DWidgetObj();
  QPaintEngine *paintEngine() const { return NULL; }
  void paintEvent(QPaintEvent* pEvent);
  bool changeShaderPath(LPCWSTR aPath);

  bool init();
  bool reset();

  //  Models handling
public:
  ID3D10Device* getD3DXdevice();

  //  Regular model
public:
  void eraseAllModels();
  void createModel( EditableMeshModel* aModel,
                    const int aIndex );
  void setModels();

  //  ProcTex model
public:
  void eraseAllModelsProcTex();
  void createModelProcTex( EditableMeshModelProcTex* aModel,
                    const int aIndex );

  void updateGizmoModel();
  void updateGizmoTModel();
  void updateGizmoRModel();
  void updateGizmoSModel();
  void hideGizmoT();
  void hideGizmoR();
  void hideGizmoS();
  void showGizmoT();
  void showGizmoR();
  void showGizmoS();

  void setMode(int aMode);

  void createGizmo();

  void cancelKnifeOperation();

  //  other
public:
  float   camScale();

  //  Image handling
public:
  void muteBackgroundImage();
  void createBackgroundImage(std::vector<D3DXVECTOR3>& aPoints,
                              float aAlpha );
  void createBackgroundImageTexture ( int w,
                                      int h,
                                      unsigned char *data);

  void eraseBackgroundImage();



  void          setGridScaleMultiplier(float aMult );

  //std::vector<D3DXVECTOR3> computeBackgroundImageVertices( float aScaleMult );

  void mousePressEvent ( QMouseEvent * event );
  void mouseMoveEvent ( QMouseEvent * event );
  void mouseReleaseEvent ( QMouseEvent * event );  

  //  For specific operations
public:
  void mousePressEventKniveLine ( QMouseEvent * event );
  void mouseMoveEventKniveLine ( QMouseEvent * event );

  void mousePressEventTexture ( QMouseEvent * event );
  void mouseMoveEventTexture ( QMouseEvent * event );


private:
  void recomputeGrid();
  void recomputeAxis();
  D3DXVECTOR3 projectPoint ( QPoint Apos );

  bool checkEntity ( QPoint p );
  bool checkGizmoT ( QPoint p );
  bool checkGizmoR ( QPoint p );
  bool checkGizmoS ( QPoint p );

  void  createKniveLine();

  D3DXVECTOR3 projectScreenRay ( float x, float y, float z );


public:
  ObjView*          mView;
private:
  GridModel*        mGrid;
  AxisModel*        mAxis;

  GizmoTMeshModel*  mGizmoT;
  bool              mShowGizmoT;
  GizmoTMeshModel*  mGizmoR;
  bool              mShowGizmoR;
  GizmoTMeshModel*  mGizmoS;
  bool              mShowGizmoS;

  LineListModel*    mKniveLinePlane;
  bool              mShowKniveLinePlane;


public:
  ObjEdRenderApp*  app;
private:
  int mMode;
  ObjectEditor* mEditor;

  //
private:
  bool        mDragWithGizmoT;
  bool        mDragWithGizmoR;
  bool        mDragWithGizmoS;
  bool        mDragCamera;
  bool        mDragOperation;
  bool        mDragSelection;
  bool        mDragTextureAdd;
  QPoint      mMouseLastPos;

  int   mKniveLineNumSelectedEntities;
  ObjEd::Selection  mKniveEntity[2];
  D3DXVECTOR3 mKniveLinePoint[2];

  //  Mish
  UINT        mWidth;
  UINT        mHeight;
  QVBoxLayout* mLayout;


  ImageQuadModel* mBackgroundImage;
  bool            mMuteBackground;

  std::vector<EditableMeshModel*> mModels;
  std::vector<bool>         mModelAllocated;
  int  mMaximumModels;

  std::vector<EditableMeshModelProcTex*> mModelsProcTex;
  std::vector<bool>         mModelAllocatedProcTex;
  int  mMaximumModelsProcTex;

  //DEBUG


  /*
  void  click       ( QPoint p );
  void  drag        ( QPoint p );*/

public slots:

};

#endif // QD3DWIDGET_OBJ_HPP
