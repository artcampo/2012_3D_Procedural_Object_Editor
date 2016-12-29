#ifndef EDITORBEZIER_H
#define EDITORBEZIER_H

#include <vector>
#include <iostream>
#include <sstream>

#include "Editor_Bezier/EditorGlobal.hpp"
#include "Editor_Bezier/bezier.h"
#include "editorbezierstate.h"

#include "App/dx_misc.hpp"
#include "App/beziermodel.h"
#include "App/pairtangentsmodel.h"

#include "UI_Bezier/QD3DWidgetBezier.hpp"
#include "UI_Bezier/objecttree.h"

#include "Exterior/bmp.hpp"


class EditorBezier
{
public:    
    EditorBezier();
    ~EditorBezier();
    void init();
    void updateStatusBarByTab( QString aName );

    //  File Handling
    void  loadFromFile( QFile& aFile );
    void  saveToFile  ( QFile& aFile );
    void  exportToFile( QFile& aFile );
    void  exportToFileLastPoint(QFile &aFile);

    //  Object handling
    void  createNewSpline();
    void  copyCurrentSpline();
    void  selectBezierCurve ( const std::string& aName );
    void  deleteObject();
    void  closeAllSplines();

    //  Modes
    void  modeSelect();
    void  modeCreate();
    void  modeInsert();
    void  modeMove();
    void  toggleAdjustToGrid();

    //  Misc
    void  setBackgroundImageFileName( QString& aFileName, float aAlpha, float aSizeMult  );
    void  muteBackgroundImage();
    void  setGridMultiplier( float aMult );

    //  Undo functionality
    void  saveEditorState();
    void  undoEditorState();
    void  redoEditorState();
    bool  undoPossible();
    bool  redoPossible();

    //  Logic
    bool  allowsAppend();
    bool  allowsPointDragging();
    bool  allowsTangentDragging();
    bool  allowsSelection();
    bool  selectedPointHasTangents();
    bool  adjustPointToGrid();

    //  Managing one spline
    void appendPoint( const D3DXVECTOR3& aP );

    bool hasPointNear(const D3DXVECTOR3 &aP);
    bool hasTangentNear(const D3DXVECTOR3 &aP);
    bool selectPointIfNear( const D3DXVECTOR3& aP );
    bool selectTangentIfNear(const D3DXVECTOR3 &aP);

    void dragSelectedPoint ( const D3DXVECTOR3& aP );
    void dragSelectedTangent ( const D3DXVECTOR3& aP );
    void dragSelectedTangentCorrectOpposite ( const D3DXVECTOR3& aP );
    void dragFinished();

    bool  hasPointSelected();
    int   getIndexPointSelected();

    void setMovingPointTangentsForceContinous(const bool aValue);

    //  Views update
    void    selectWorkView( QD3DWidgetBezier* aView );
    void    updateWorkView();

private:
    void    updateStateAndViews();
    void    updateViews();

    void    deleteSelectedSpline();
    void    deleteSelectedPoint();

    void    modeClearPrevious();
    void    selectBezierCurveAndPoint( int aBezIndex, int aPointIndex );
    void    showTangents();
    void    unshowTangents();
    D3DXVECTOR3 applyMask( const D3DXVECTOR3& aPoint, const D3DXVECTOR3& aMask );

    int           getUniqueId();
    std::string   getUniqueName();
    std::string   getSplineName( int i );

    bool  objectNameExists ( const std::string& aName );
    int   idObjectViaName ( const std::string& aName );
    std::vector<std::string> getObjectsNames();

private:
    QD3DWidgetBezier*   mWorkingView;

    std::vector<Bezier>   mBezSplines;

    //  display
    QString                   mBackgroundImageFileName;
    std::vector<D3DXVECTOR3>  mBackgroundImageVertices;
    int                       mBmph, mBmpw;
    std::vector<BYTE>         mBackgroundImageTexture;

    //  Selection
    int   mSelectedSpline;
    int   mSelectedPoint;

    //  Booleans of state
    bool  mAtLeastOnePointExists;
    bool  mOnePointIsSelected;
    bool  mTangentsBeingDisplayed;
    bool  mTangentSelected;
    bool  mTangentPrevSelected;    

    bool  mModeSelect;
    bool  mModeCreate;
    bool  mModeInsert;
    bool  mModeMove;

    bool  mAdjustToGrid;

    //  Colours
    D3DXCOLOR   mBezSelectedColour;
    D3DXCOLOR   mBezNotSelectedColour;

    float       mDistanceThresholdSelect;
    float       mDistanceThresholdSelectOnDrag;

    bool  mOptionWhenMovingPointTangentsShouldBeContinous;

    float       mDefaultAlphaForBackgroundImage;

    //  Editor
    int         mFirstFreeUniqueId;

    std::vector<EditorBezierState*>   mUndoStates;
    int         mUndoPosition;

};

#endif // EDITORBEZIER_H
