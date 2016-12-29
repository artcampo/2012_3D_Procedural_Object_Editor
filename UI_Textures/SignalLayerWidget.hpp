#ifndef SIGNALLAYOUT_HPP
#define SIGNALLAYOUT_HPP


#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QMessageBox>
#include <QFrame>
#include <QGridLayout>
#include <QDir>
#include <QCoreApplication>
#include <QComboBox>
#include <QSlider>
#include <QSharedPointer>
#include <QApplication>
#include <QCheckBox>

#include <sstream>
#include <string>
#include <iostream>

#include "UI_Textures/QD3DWidgetTexture.hpp"

#include "UI_Textures/MinorWidgets/SingleColourWidget.h"
#include "UI_Textures/MinorWidgets/SignalNameButton.hpp"


class ProcShader;
class ProfilerDp;
class SplineNWidget;

class SignalLayerWidget : public QFrame
{
  Q_OBJECT
public:
  SignalLayerWidget::~SignalLayerWidget();
  SignalLayerWidget(QWidget *parent,
                    Layer* aLayer,
                    bool aIsLastLayer,
                    bool aIsAlone,
                    std::string& aLayerOpName,
                    ProcShader* aProcShader,
                    int aChannelId,
                    QD3DWidgetTexture *aFinalMaterial,
                    QD3DWidgetTexture *aCurrentSignLay,
                    ProfilerDp* aProfiler);

  int getOpToCompositeId();
  string getLayerName();

  void  loadSignalVariables();
  void  loadCompositeOperationVariables();
  float compositeLayerBlendFactor();

  void  profileShaderCompile();
  void  profileRedrawFinal( std::string& tempName, ProfilerDp* prof );
  //  Qt mouse
public:

  void mousePressEvent    (QMouseEvent *event);
  void mouseMoveEvent     (QMouseEvent *event);
  void mouseReleaseEvent  ( QMouseEvent * event );

  void dropEvent(QDropEvent *de);
  void dragMoveEvent(QDragMoveEvent *de);
  void dragEnterEvent(QDragEnterEvent *event);

  void setDragging( bool aValue );
private:
  QPoint  mDragStartPosition;
  bool    mDragging;
  // copy of pointers to main objects
private:
  bool        mIsLastLayer;
  bool        mIsAlone;
  int         mChannelId;
  ProcShader* mProcShader;
  QD3DWidgetTexture *mFinalMaterial;
  QD3DWidgetTexture *mCurrentSignLay;

  //  widgets
private:
  //QVBoxLayout*  mLayout;
  QVBoxLayout*  mMainLayout;  
  QGridLayout*  mSignalLayout;
  QHBoxLayout*  mCompOpLayout;

  // layer layout
  QHBoxLayout*  mLayerSignalSuperLayout;
  QVBoxLayout*  mLayerSignalButtonsLayout;
  QPushButton*  mDeleteLayerButtton;
  QPushButton*  mReinitSignalButtton;

  QComboBox*    mChooseSignal;
  QComboBox*    mChooseSignalType;


  // signal layout
  QLabel*       mLayerLabel;
  QLabel*       mInputParamsLabel;
  QComboBox*    mChooseInputParams;
  QCheckBox*    mWeightLabel;
  QSlider*      mChooseWeightX;
  QSlider*      mChooseWeightY;
  QSlider*      mChooseWeightZ;
  QSpinBox*         mOctSpin;
  QDoubleSpinBox*   mOctMultSpin;
  SplineNWidget*    m2GradientPicker;

  //  Signal: warp
  QComboBox*        mChooseSignalWarp;
  QDoubleSpinBox*   mWarpLengthSpin;
  QCheckBox*        mWarpCheckBox;
  string            mWarpSignalName;

  //  Signal: lines
  QCheckBox*    mFreq1Label;
  QCheckBox*    mPhaseLabel;
  QLabel*    mThresholdLabel;

  bool       mFreq1SliderSingleValue;
  QSlider*   mFreq1SliderX;
  QSlider*   mFreq1SliderY;
  QSlider*   mFreq1SliderZ;

  bool       mPhaseSliderSingleValue;
  QSlider*   mPhaseSliderX;
  QSlider*   mPhaseSliderY;
  QSlider*   mPhaseSliderZ;


  //QDoubleSpinBox*   mFreq2Spin;
  QDoubleSpinBox*   mThresholdSpin;

  //  composed op layout
  QComboBox*    mChooseOperation;
  QSlider*      mOpBlendFactorSlider;

  //  SIGNAL: khan-1
  QDoubleSpinBox*   mFreq1floatSpin;
  QDoubleSpinBox*   mFreq2floatSpin;


private:
  int mIndexOfLayerSignal;
  bool mInhibitSignalChangedSlot;
  bool mNewLayerOpModeSet;

private:
  std::string   mNameLayer;     //Main Layer-Signal
  std::string   mSignal;          //its signal
  int           mSignalType;
  std::string   mCompositeLayerOpName;  //its composite operation
  int           mCompositeLayerOpId;
  float         mCompositeLayerOpBlend;

  //  SIGNAL: General
  int                         mColorId;
  std::vector<D3DXCOLOR>      mColor;
  int           mInputParams;
  D3DXVECTOR3   mNoiseW;
  int           mNoiseNoct;
  float         mNoiseMoct;

  //  SIGNAL: noise
  //TODO: remove
  float         mTurbulence;
  QSlider*      mTurbulenceSlider;

  //  SIGNAL: Lines / Strips
  D3DXVECTOR3     mFreq1;
  D3DXVECTOR3     mPhase;
  float           mThreshold;

  D3DXVECTOR3     mComposedFreq1;

  //  SIGNAL: khan-1
  float   mFreq1float;
  float   mFreq2float;

  //PROFILING
private:
  ProfilerDp* mProfiler;  //just pointer  

  std::wstring makeShaderFilePath ( std::string& aActualSignal );
public:
  void  redrawSignalPreview();
  void  redrawFinal();
 private:
  void  loadWidgetsSignalGeneral();
  void  loadWidgetsSignalNoise();
  void  loadWidgetsSignalNoiseSin();
  void  loadWidgetsSignalStrips();
  void  loadWidgetsSignalLines();
  void  loadWidgetsSignalKhan1();

  D3DXVECTOR3 getDifferentValue( float v1, float v2, float v3 );

private slots:
  void signalChanged();
  void signalOpChanged( int aOpId );
  void signalOpBlendChanged();
  void signalTypeChanged();
  void signalToBeDeleted();
  void signalToBeReinitialized();
  void changeSignalOfLayer( const QString& aSelectedSignal );
  void changeWarpSignalOfLayer(const QString& aSelectedWarpSignal );
signals:
  void forceReloadLayersAndSignals();
  void forceReloadSignals();
  void setDragSource( const std::string, const bool );
  void setDragDestination( const std::string, const bool );
  void forceDeleteListLayerAndSignal();


};


#endif // SIGNALLAYOUT_HPP
