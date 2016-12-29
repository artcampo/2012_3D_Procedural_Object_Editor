#ifndef QD3DWIDGET_HPP
#define QD3DWIDGET_HPP

#include <QWidget>
#include <QKeyEvent>
#include <QMessageBox>
#include <QApplication>
#include <sstream>

#include "App/dx_misc.hpp"
#include "APP/RenderViewApp.hpp"

#include "UI_Bezier/ViewManager.h"


#include "Editor_Bezier/EditorGlobal.hpp"
#include "Editor_Bezier/editorbezier.h"

#include "App/beziermodel.h"
#include "App/GridModel.h"
#include "App/pairtangentsmodel.h"
#include "App/AxisModel.h"
#include "App/imagequadmodel.hpp"

class MainWindow;

class QD3DWidgetBezier : public QWidget
{
    Q_OBJECT
public:
  explicit
  QD3DWidgetBezier(QWidget *parent, UINT width, UINT height, const ViewManager& aView );
  ~QD3DWidgetBezier();
  QPaintEngine *paintEngine() const { return NULL; }
  void paintEvent(QPaintEvent* pEvent);
  bool changeShaderPath(LPCWSTR aPath);
  bool init();
  bool reset();

  void eraseAllModels();
  void createModel( std::vector<D3DXVECTOR3>& aPoints,
                    std::vector<D3DXVECTOR3>& aPointsCurve,
                    D3DXCOLOR& aColour,
                    const int aIndex );
  void setModels();

  void createTangentModel(std::vector<D3DXVECTOR3>& aPoints,
                          D3DXCOLOR& aColour );
  void eraseTangentModel();

  void muteBackgroundImage();
  void createBackgroundImage(std::vector<D3DXVECTOR3>& aPoints,
                              float aAlpha );
  void createBackgroundImageTexture ( int w,
                                      int h,
                                      unsigned char *data);
  void eraseBackgroundImage();

  D3DXVECTOR3  getSelectionMask();
  D3DXVECTOR3  getUnitUmultScale();
  void          setGridScaleMultiplier(float aMult );

  std::vector<D3DXVECTOR3> computeBackgroundImageVertices( float aScaleMult );

  void mousePressEvent ( QMouseEvent * event );
  void mouseMoveEvent ( QMouseEvent * event );
  void mouseReleaseEvent ( QMouseEvent * event );
  //void keyPressEvent( QKeyEvent *event );

private:
  void recomputeGrid();
  void recomputeAxis();
  D3DXVECTOR3 projectPoint ( QPoint Apos );

private:

  bool        mMouseDragLeft;
  bool        mMouseDragRight;
  bool        mMouseDragMiddle;
  bool        mDraggingPoint;
  bool        mDraggingTangent;
  bool        mDragHasModified;
  QPoint      mMouseLastPos;

  RenderViewApp*  app;
  UINT        mWidth;
  UINT        mHeight;
  ViewManager*       mView;


  std::vector<BezierModel*> mBezModels;
  std::vector<bool>         mModelAllocated;
  int  mMaximumModels;

  GridModel*      mGrid;
  AxisModel*      mAxis;
  ImageQuadModel* mBackgroundImage;
  bool            mMuteBackground;

  PairTangentsModel*  mPairTangentsModel;

signals:

public slots:

};

#endif // QD3DWIDGET_HPP
