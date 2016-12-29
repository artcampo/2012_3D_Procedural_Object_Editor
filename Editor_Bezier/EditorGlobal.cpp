#include "Editor_Bezier/EditorGlobal.hpp"


namespace GlobalEditor{
MainWindow*     gMainWindow;
EditorBezier*   gEditor;
QD3DWidgetBezier*   gViewX;
QD3DWidgetBezier*   gViewY;
QD3DWidgetBezier*   gViewZ;
QD3DWidgetBezier*   gViewF;
ObjectTree*   gObjTree;
//D3DXCOLOR gColourPointSelected    = D3DXCOLOR(0.8f,0.8f,0.8f,1.0f);
D3DXCOLOR gColourPointSelected    = D3DXCOLOR(0.0f,0.0f,0.0f,1.0f);
D3DXCOLOR gColourPointNotSelected = D3DXCOLOR(0.3f,0.3f,0.3f,1.0f);
};
