#ifndef QD3DWIDGET_DIRG_HPP
#define QD3DWIDGET_DIRG_HPP

#include <QWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QApplication>
#include <QVBoxLayout>

#include <sstream>


#include "App/dx_misc.hpp"
#include "APP/DirGraphEdRenderApp.hpp"


#include "App/GridModel.h"
#include "App/pairtangentsmodel.h"
#include "App/AxisModel.h"
#include "ObjEditor/Gizmos/GizmoT.hpp"

#include "UI_DirGraph/ObjViewDG.h"
#include "Editor_DirGraph/DirGraphEditor.hpp"
#include "App/EditableDirGraph.h"
#include "Geometry/Intersections.hpp"

class MainWindow;

class QD3DWidgetDirG : public QWidget
{
    Q_OBJECT
public:
  explicit
  QD3DWidgetDirG(QWidget *parent, UINT width, UINT height, ObjViewDG* aView, DirGraphEditor* aEditor );
  ~QD3DWidgetDirG();
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

  //  ProcTex model
public:

  //  other
public:
  float   camScale();


  void    setGridScaleMultiplier(float aMult );

  void mousePressEvent    ( QMouseEvent * event );
  void mouseMoveEvent     ( QMouseEvent * event );
  void mouseReleaseEvent  ( QMouseEvent * event );


private:
  void recomputeGrid();
  void recomputeAxis();

  bool          checkVertex   ( QPoint p );
  int           getVertex     ( QPoint p );
  D3DXVECTOR3   projectPoint  ( QPoint Apos );  
  void          mousePosToRay ( QPoint p, D3DXVECTOR3& p1, D3DXVECTOR3& p2 );

public:
  ObjViewDG*          mView;
  DirGraphEditor*     mEditor;
  GizmoTMeshModel*    mGizmoTModel;

private:
  GridModel*        mGrid;
  AxisModel*        mAxis;
  GizmoT            mGizmoT;
  ColourMapping     mColours;
  LineListModel*    mNewEdgeLine;
  bool              mShowNewEdgeLine;

private:
  void  initMisc();
  void  updateGizmoTModel();
  void  createGizmoT();
  bool  checkGizmoT( QPoint& p );
  bool  checkGizmoT(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, bool& aUpdateGizmo );

public:
  DirGraphEdRenderApp*  app;

private:
  bool        mDragWithGizmoT;
  bool        mDragCamera;
  bool        mDragMove;
  bool        mShowGizmoT;
  int         mDraggedVertexId;
  QPoint      mMouseLastPos;

  //  Misc
  UINT        mWidth;
  UINT        mHeight;
  QVBoxLayout* mLayout;

  float       mScaleGizmoT;
  float       mThresholdGizmoT;

};

#endif // QD3DWIDGET_DIRG_HPP
