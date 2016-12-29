#ifndef OPEXTRUDEWIDGET_H
#define OPEXTRUDEWIDGET_H

#include <QFrame>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>

class OpExtrudeWidget : public QFrame
{
    Q_OBJECT
public:      
    OpExtrudeWidget( QWidget *parent );

    //direct access from parent class
public:
    QSpinBox*   mPartsOSpin;
    QSpinBox*   mPartsISpin;
    QSpinBox*   mPartsMSpin;
private:
    QVBoxLayout*  mLayout;

    QTabWidget*  mTab;

    QComboBox*   mCombo;

    QWidget*     mExtTab[3];
    QVBoxLayout* mTLay[3];



    QLabel*     mPartsLabel[3];

private:
    int   mExtMode;

    void removeAllTabs();

signals:
    void extrudeTypeChanged( int index );
public slots:
    void changeType( int index );

};

#endif // OPEXTRUDEWIDGET_H
