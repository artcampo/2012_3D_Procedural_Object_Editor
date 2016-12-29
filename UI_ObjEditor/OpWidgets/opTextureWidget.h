#ifndef OPTEXTUREWIDGET_H
#define OPTEXTUREWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QFileDialog>

#include "ObjEditor/objectEditor.h"

class ObjectEditor;

class OpTextureWidget : public QFrame
{
    Q_OBJECT
public:
    explicit OpTextureWidget(ObjectEditor* aEditor, QWidget *parent = 0);

public:
  ObjectEditor*       mEditor;

  QPushButton*  mInitButton;
  QVBoxLayout*  mLayout;

  QHBoxLayout*      mTopLayout;
  QComboBox*        mCombo;
  QPushButton*      mAddButton;


  QVBoxLayout*      mInnerLayout;
  QFrame*           mInnerFrame;
  QLabel*           mNameLabel;
  QDoubleSpinBox*   mScaleSpin;

  QPushButton*      mSelectFileButton;

  bool mInhibitSignal;
  bool  mEditionStarted;
  int   mSelectedTexture;
  std::vector<ObjEd::ProcTex> mProcTex;

private:
  void  initNoTex();
  void  initTex();
  void  loadTextures( int newIndex  );

  bool  _selectFileName();

signals:  

public slots:
  void  initTextureEdition();
  void  changeSelectedTex( int index);
  void  texChanged();
  void  addTexture();
  void  selectFileName();

};

#endif
