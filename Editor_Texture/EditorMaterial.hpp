#ifndef EDITOR_MATERIAL_H
#define EDITOR_MATERIAL_H

#include <vector>
#include <QString>
#include <QFile>
#include <QApplication>
#include <QDataStream>
#include <QTextStream>


#include "Editor_Texture/DataLayer/ProcShader.hpp"
#include "Editor_Texture/DataLayer/Library/ChannelLibrary.hpp"

class EditorMaterial
{
public:    
    EditorMaterial();
    ~EditorMaterial();
    void init();
    void updateStatusBarByTab( QString aName );

    //  File Handling
    void  newFile     ();
    void  loadFromFile( QFile& aFile );
    void  saveToFile  ( QFile& aFile );
    void  exportToFile( QString& aName );
    void  exportToFileDeferred( QString& aName, int aVertexLayout );

    //  Channel library
    void  channelSave ( QFile& aFile );
    void  channelLoad ( QFile& aFile );


    //  Object handling
    void createComposition( int aNewOpId, float aBlendFactor, std::string& aFatherLayer);
    void swapLayerSignal ( std::string aLayer1, std::string aLayer2, bool aSwapCompositeOp );
    void deleteLastLayerSignal ( std::string& aLayerOp, std::string& aLayerSignal, std::string& aLayerSignalKeep );

    //  Misc


    std::vector<std::string>  getLayersSigNames_ModeList ();    
    std::vector<std::string>  getLayersOpNames_ModeList ();

    //  Views update

    void    updatePreview();

private:

    int           getUniqueIdSignal();
    int           getUniqueIdLayer();
    std::string   getSignalName( int i );
    std::string   getLayerName( int i );

public:
    ProcShader*   mProcShader;
private:

    //  Editor
    int         mFirstFreeUniqueIdSignal;
    int         mFirstFreeUniqueIdLayer;

/*
    std::vector<EditorMaterialState*>   mUndoStates;
    int         mUndoPosition;
*/
};

#endif // EditorMaterial_H
