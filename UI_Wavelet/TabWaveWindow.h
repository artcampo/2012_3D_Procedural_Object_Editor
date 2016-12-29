#ifndef _TabWaveWindow_H
#define _TabWaveWindow_H


#include <QMessageBox>
#include <QString>
#include <QGridLayout>
#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QColor>
#include <QFrame>
#include <QFile>
#include <QTextStream>
#include <QSpinBox>


class TabWaveWindow : public QFrame
{
    Q_OBJECT
public:
    TabWaveWindow(QWidget *parent);
    ~TabWaveWindow();



private:
    QVBoxLayout*  mMainLayout;
    QHBoxLayout*  mControlLayout;
    QHBoxLayout*  mInputLayout;
    QHBoxLayout*  mOutputLayout;

    bool    mInhibitInputSliderSignal;

    std::vector<QSlider*>  mInSlider;
    std::vector<QSlider*>  mOutSlider;

    QSpinBox*         mInputSpin;
    QSpinBox*         mOutputSpin;

    int mNumBits;

    int mInput;
    int mOutput;

private:
    void readInputSignal();
    void recomputeSignal();

private slots:
    void  inputChanged();
    void  inputIntChanged();

};

#endif // TabPalWindow_H
