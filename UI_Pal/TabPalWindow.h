#ifndef _TabPalWindow_H
#define _TabPalWindow_H


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

#include "UI_Pal/colourwidget.h"

class TabPalWindow : public QFrame
{
    Q_OBJECT
public:
    TabPalWindow(QWidget *parent);
    ~TabPalWindow();

    void  newColour();
    void exportToNpal( QFile& aFile );

private:
    QVBoxLayout*  mLayout;
    std::vector<QHBoxLayout*>  mColLayout;


    std::vector<ColourWidget*> mColours;


private slots:

};

#endif // TabPalWindow_H
