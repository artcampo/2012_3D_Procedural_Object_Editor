#ifndef _TabTextureWindow_H
#define _TabTextureWindow_H


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

#include "UI_Textures/QD3DWidgetTexture.hpp"
#include "UI_Textures/MinorWidgets/ChannelButton.hpp"
#include "UI_Textures/MinorWidgets/ChannelWidget.hpp"
#include "UI_Textures/MinorWidgets/ChannelSpecWidget.hpp"
#include "UI_Textures/SignalLayerWidget.hpp"



#include "Editor_Texture/EditorMaterial.hpp"


class TabTextureWindow : public QFrame
{
    Q_OBJECT
public:
    TabTextureWindow(QWidget *parent);
    ~TabTextureWindow();

    EditorMaterial*   mEditor;
private:


    //  Main Tab
    QHBoxLayout*  mMainLayout;
    QVBoxLayout*  mChannelLayout;
    QVBoxLayout*  mSignLayLayout;
    QVBoxLayout*  mSignLayLayoutPreScroll;
    QVBoxLayout*  mPreviewLayout;


    //  Channel Layout
    ChannelButton*  mRGB;
    ChannelButton*  mRGBFar;
    //ChannelButton*  mAlpha;
    ChannelButton*  mBump;
    ChannelButton*  mSpec;
    QCheckBox*      mBumpCheckBox;

    ChannelWidget*      mChannelWidget;
    ChannelSpecWidget*  mChannelSpecWidget;


    //  SignLay Layout
    QScrollArea*      mSignalsScrollArea;
    QWidget*          mSignalsScrollAreaContents;
    std::vector<SignalLayerWidget*>  mSignLayers;
    QPushButton*      mAddNewSignalLayer;

    //  Preview Layout
    QD3DWidgetTexture *mFinalMaterial;
    QD3DWidgetTexture *mCurrentSignLay;

    //  Drag
    std::string   mDragSourceLayer;
    bool          mDraggingOverLastLayer;

    int   mIndexSelectedChannel;
    bool  mDisableRedraw;

    //  updated often, be careful!
    std::vector<std::string> layersSig;
    std::vector<std::string> layersOp;

    //PROFILING
  private:
    ProfilerDp* mProfiler;  //OBJECT STORED

private:
    bool    initDxWidgets();
    void    getSignalsAndLayouts();
    void    getSignalsAndLayouts_ModeList();
    void    removeSignalsAndLayouts();
    void    addNewSignal_ModeList();
    void    setChannelWidget( const int aPreviousChannel );
    void    initChannelWidgets();

    std::vector<QString> channelLibraryList();

public:
    //  Main IO
    void    loadFromFile( QFile& aFile );
    void    saveToFile  ( QFile& aFile );
    void    shaderExport(QString& aName);
    void    shaderExportDeferred( QString& aName, int aVertexLayout );
public slots:
    void    addNewSignal();
    void    reloadSignalsAndLayers();
    void    reloadSignals();
    void    getSourceDrag       ( const std::string aName, const bool aIsLastLayer );
    void    getDestinationDrag  ( const std::string aDragDestinationLayer, const bool aIsLastLayer );
    void    channelSelected     ( int aChannelId );
    void    changeShaderPropierties ();
    void    channelNameChanged ();
    void    channelDepthChanged();

    void    channelLoad( QString aName );
    void    channelSave( QString aName );
/*
    void    shaderLoad( QString aName );
    void    shaderSave( QString aName );
*/
    void    channelList ();
    void    deleteListLayerAndSignal();

    void    channelSpecChanged();

};

#endif // TabPalWindow_H
