#ifndef _DirGraphEditor_H
#define _DirGraphEditor_H
#include <QFileInfo>

#include <vector>
#include <iostream>
#include <sstream>

#include <QFile>
#include <QWidget>
#include <QString>



#include "App/dx_misc.hpp"
#include "App/beziermodel.h"
#include "App/pairtangentsmodel.h"
#include "App/EditableMeshModelProcTex.h"
#include "Geometry/DirGraph/DirGraph.hpp"
#include "App/EditableDirGraph.h"

class QD3DWidgetDirG;

namespace DirGraphEd{

  struct Selection{
    int typeEntity;
    int idEntity;
  };

  enum eEntityEV{
    eVertex,
    eEdge
  };
};

class DirGraphEditor : public QWidget
{
    Q_OBJECT

public:    
    DirGraphEditor();
    ~DirGraphEditor();
    void init();
    void initOptions();
    void updateStatusBarByTab( QString aName );

    //  File Handling
    void  newFile ();
    void  loadFromFile( QString& aFile );
    void  saveToFile  ( QString& aFile );
    void  exportToFile();
    void  setGraphExportName( QString& aName );



    //  Interfacing
    bool  checkVertex ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXMATRIX& aView, D3DXVECTOR3 aCamPos );
    int   getVertex   ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXMATRIX& aView, D3DXVECTOR3 aCamPos );

    void  highlightEntity ( const D3DXVECTOR3& aRayB, const D3DXVECTOR3& aRayE, D3DXVECTOR3 aCamPos );


    void  changeSelectedVertex( const int aVid );
    int   selectedVertex() const;


    int   createVertex  ( const D3DXVECTOR3& aPos );
    void  createEdge    ( const int aVid1, const int aVid2 );
    void  modifyVertex  ( const int aVid, const D3DXVECTOR3& aPos );
    D3DXVECTOR3 vertexPos ( const int aVid ) const;
    void  deleteHighlighted ();

    void  adjustYCoord();
    void  adjustAllPointsToGrid();

    //  Rendering
    void selectWorkView( QD3DWidgetDirG* aView );
    EditableDirGraph* convertModel();

    void  toggleAdjustGrid( bool v );

    //  Views update    
    void    updateWorkView();

private:
    float distanceVertexThreshold();
    float distanceEdgeThreshold();

private:

    QD3DWidgetDirG*             mWorkingView;
    DirectedGraph::DirGraph*    mDirGraph;
    QString                     mDirGraphName;

    int     mSelectedVertex;
    DirGraphEd::Selection mHighV;
    DirGraphEd::Selection mHighE;

public:
    QString    mFileName;
    D3DXVECTOR3 mHighlightedSegmentPoint;
    D3DXVECTOR3 mHighlightedPoint;

    //  misc configuration
private:
    float mDistanceThresholdSelectVertex;
    float mDistanceThresholdSelectEdge;

    //  Booleans of state
    bool    mAdjustToGrid;
    float   mGridSize;

private:
    void cleanState();
    DirGraphEd::Selection selectVertex(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE,
                                       float aDistThreshold, D3DXVECTOR3 aCamPos );
    DirGraphEd::Selection selectEdge(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE,
                                     float aDistThreshold, D3DXVECTOR3 aCamPos );

    float adjustCoordinate  (const float aCoord, float aMod );
    void  adjustToGrid      ( D3DXVECTOR3& aP );

signals:
    void  updateViews();
    void  dirGraphNameChanged( QString aName );

};

#endif // DirGraphEditor
