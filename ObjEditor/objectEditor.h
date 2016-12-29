#ifndef _ObjectEditor_H
#define _ObjectEditor_H
#include <QFileInfo>

#include <vector>
#include <iostream>
#include <sstream>

#include <QFile>
#include <QWidget>
#include <QString>

#include "objeditorstate.h"

#include "App/dx_misc.hpp"
#include "App/beziermodel.h"
#include "App/pairtangentsmodel.h"
#include "App/EditableMeshModelProcTex.h"

//#include "UI_ObjEditor/QD3DWidgetObj.hpp"
class QD3DWidgetObj;
//#include "UI_ObjEditor/ObjectTreeWidget.h"

#include "Exterior/bmp.hpp"



#include "ObjEditor/Mesh/Mesh.hpp"
#include "ObjEditor/Mesh/editableobject.h"
#include "ObjEditor/ObjEditorSharedData.hpp"
#include "ObjEditor/Mesh/OperationsInfo.hpp"
#include "ObjEditor/Mesh/OperationsOptions.hpp"

#include "App/ObjEd/GizmoTMeshModel.hpp"
#include "ObjEditor/Gizmos/GizmoT.hpp"
#include "ObjEditor/Gizmos/GizmoS.hpp"
#include "ObjEditor/Gizmos/GizmoR.hpp"

class ObjectEditor : public QWidget
{
    Q_OBJECT

public:    
    ObjectEditor();
    ~ObjectEditor();
    void init();
    void initOptions();
    void updateStatusBarByTab( QString aName );



    //  Selection
    float distanceThreshold();
    bool checkEntity ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE );
    bool checkGizmoT ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, bool& aUpdateGizmo );
    bool checkGizmoR ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, bool& aUpdateGizmo );
    bool checkGizmoS ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, bool& aUpdateGizmo );

    //  File Handling
    void  newFile ();
    void  loadFromFile( QString& aFile, int aMode );
    void  saveToFile  ( QString& aFile );
    void  exportToFile( QString& aFile );

    //  Object handling
    MeshInfo  infoMeshText();

    //  Rendering
    void  changeRenderMode();
    int   renderMode();
    bool  hasRenderModel();
    std::vector<EditableMeshModel*>         convertObject( ID3D10Device* aDev, D3DXVECTOR3 aCamPos );
    std::vector<EditableMeshModelProcTex*>  convertObjectProcTex( ID3D10Device* aDev, D3DXVECTOR3 aCamPos );
    GizmoTMeshModel* gizmoT( ID3D10Device* aDev, float aScale  );
    GizmoTMeshModel* gizmoR( ID3D10Device* aDev, float aScale  );
    GizmoTMeshModel* gizmoS( ID3D10Device* aDev, float aScale  );

    //  Gizmos
    void  createGizmoT();
    void  createGizmoR();
    void  createGizmoS();
    void  hideGizmoT( bool aUpdateViews = true );
    void  hideGizmoR( bool aUpdateViews = true );
    void  hideGizmoS( bool aUpdateViews = true );
    void  showGizmoT();
    void  showGizmoR();
    void  showGizmoS();

    //  Modes
    void  setMode   ( int aMode );
    void  setModeOp ( int aModeOp );
    int   modeOp    () const;
    int   mode      () const;

    //  Edition state-handling
    bool  allowsOperation();
    bool  allowsSelection();
    bool  allowsModifySelection();
    bool  allowsGizmoT();
    bool  allowsGizmoR();
    bool  allowsGizmoS();

    void  selectEntityHighlighted();
    void  unselectEntities();
    void  selectAddEntityHighlighted();
    void  selectRemoveEntityHighlighted();
    bool  isEntitySelected ( const ObjEd::Selection& aEntity );
    bool  hasSelectionOfEntities();

    //  primiteve operation
public:
    void addNewPrimitive();

    //  Regular operation
public:
    void  startOperation ();
    void  performOperation( float x, float y );
    void  endOperation();

    float stepOperation() const;
    float operationValue();

    //  Operation by gizmo - T
    void  startOperationGizmoT();
    void  performOperationGizmoT ( D3DXVECTOR3 aRayB,
                                   D3DXVECTOR3 aRayE,
                                   D3DXVECTOR3 aRayB2,
                                   D3DXVECTOR3 aRayE2,
                                   D3DXVECTOR3 aCamU,
                                   D3DXVECTOR3 aCamV );
    void  endOperationGizmoT();

    //  Operation by gizmo - R
    void  startOperationGizmoR();
    void  performOperationGizmoR ( D3DXVECTOR3 aRayB,
                                   D3DXVECTOR3 aRayE,
                                   D3DXVECTOR3 aRayB2,
                                   D3DXVECTOR3 aRayE2,
                                   D3DXVECTOR3 aCamU,
                                   D3DXVECTOR3 aCamV );
    void  endOperationGizmoR();

    //  Operation by gizmo - S
    void  startOperationGizmoS();
    void  performOperationGizmoS ( D3DXVECTOR3 aRayB,
                                   D3DXVECTOR3 aRayE,
                                   D3DXVECTOR3 aRayB2,
                                   D3DXVECTOR3 aRayE2,
                                   D3DXVECTOR3 aCamU,
                                   D3DXVECTOR3 aCamV );
    void  endOperationGizmoS();


    D3DXVECTOR3 highlightedSegmentPoint();
    D3DXVECTOR3 highlightedPoint();
    ObjEd::Selection entityHighlighted();


    //  Misc
    void  setBackgroundImageFileName( QString& aFileName, float aAlpha, float aSizeMult  );
    void  muteBackgroundImage();
    void  setGridMultiplier( float aMult );

    //  Undo functionality
    void  initEditorState();
    void  saveEditorState();
    void  undoEditorState();
    void  redoEditorState();
    bool  undoPossible();
    bool  redoPossible();

    //  Views update
    void    selectWorkView( QD3DWidgetObj* aView );
    void    updateWorkView();

    //  special operations
public:
    void  startOpKniveLine( const KniveLineInfo& aInfo );
    void  performOpKniveLine ( KniveLineInfo& aInfo, D3DXMATRIX& aView, D3DXVECTOR3 aCamPos  );
    void  endOpKniveLine();    

    void  initTexturing();
    void  newProcTex();
    void  setProcTex( const std::vector<ProcTex>& aProcTex );
    std::vector<ProcTex> procTex () const;
    bool  hasProcTex() const;
    void  setCurrentTex( const int aTexId );

    //  Operations
private:
    void  startOpExtrude();
    void  performOpExtrude ( float aX, float aY );
    void  endOpExtrude();

    void  startOpBevelV();
    void  performOpBevelV ( float aX, float aY );
    void  endOpBevelV();

    void  startOpBevelE();
    void  performOpBevelE ( float aX, float aY );
    void  endOpBevelE();

    void  startOpNorScale();
    void  performOpNorScale ( float aX, float aY );
    void  endOpNorScale();

public:
    void  startOpTexture();
    void  performOpTexture( const int aComId, const int aFaceId );
    void  endOpTexture();

    //  Operations Options
public:
    void  opExtOptions( int aMode );

    void updateOperationOptions(
        Options::ExtrudeOptions &opEo,
        Options::ExtInnerOptions &opEi,
        Options::ExtMatrixOptions &opEm);

    //  Internal
private:
    void  endOpGeneric();
    void  hideGizmo();

    void  createGizmoTVertices();
    void  createGizmoRVertices();
    void  createGizmoSVertices();

    void  createGizmoTEdges();
    void  createGizmoREdges();
    void  createGizmoSEdges();

    void  createGizmoTFaces();
    void  createGizmoRFaces();
    void  createGizmoSFaces();

    void  createGizmoTObject();
    void  createGizmoRObject();
    void  createGizmoSObject();

    float distanceThresholdSelectGizmo();
    float distanceThresholdSelectVertex();

private:    

private:
    QD3DWidgetObj*   mWorkingView;

public:
    QString    mFileName;
    Mesh*      mObject; //TODO: Just one mesh!
    Selection               mHighlighted;
    int                     mMode;
    int                     mOpMode;
    int                     mRenderMode;

    GizmoT     mGizmoT;
    GizmoR     mGizmoR;
    GizmoS     mGizmoS;

    bool  mHasRenderModel;
    bool  atLeastOneEntitySelected;
    bool  mAllowsSelect;
    bool  mAllowsGizmoT;
    bool  mAllowsGizmoR;
    bool  mAllowsGizmoS;
    bool  mInhibitRedraw;

    bool  mOperationUnfinished;
    bool  mOperationOnGizmoT;
    float mOperationStep;


    //  for operations
private:
    float mExtrudeValue;
    float mBevelVValue;
    float mBevelEValue;
    float mNorScaleValue;
    int   mProcTexId;

    D3DXVECTOR3 mHighlightedSegmentPoint;
    D3DXVECTOR3 mHighlightedPoint;



    //  misc configuration
private:
    float mDistanceThresholdSelectVertex;
    float mDistanceThresholdSelectGizmo;
    ColourMapping   mColours;

    int mFileLastKnowSize;

private:
    void  initMisc();

private:
    //  display image
    QString                   mBackgroundImageFileName;
    std::vector<D3DXVECTOR3>  mBackgroundImageVertices;
    int                       mBmph, mBmpw;
    std::vector<BYTE>         mBackgroundImageTexture;
    float       mDefaultAlphaForBackgroundImage;

    //  Booleans of state
    bool  mAdjustToGrid;

    //  Editor undo/redo
    std::vector<ObjEditorState*>   mUndoStates;
    unsigned int          mUndoPosition;
    bool                  mRedoDone;


    //  for operations, options
public:
    bool  mOptionKnifeHalf;
    int   mOptionKnifeHalfCut;
    bool  mOptionKnifeIsFirstPoint;
    int   mOptionExtrudeMode;    

    Options::ExtrudeOptions     mOptionsExtrude;
    Options::ExtMatrixOptions   mOptionsExtrudeMatrix;
    Options::ExtInnerOptions    mOptionsExtrudeInner;

public:
    std::vector<int> entityHighlightedDEBUG();

    //  for operations, options
public:
    void  optionsOpKnifeHalf( bool v );
    void  optionsOpKnifeHalf( int c );
    void  optionsOpKnifeFirstPoint( bool v );
signals:
    void  updateViews();

};

#endif // ObjectEditor
