#ifndef EDITORGLOBAL_HPP
#define EDITORGLOBAL_HPP

#include "App/dx_misc.hpp"

//#include "editor/editorbezier.h"
//#include "UI/QD3DWidgetBezier.hpp"

class MainWindow;
class EditorBezier;
class QD3DWidgetBezier;
class ObjectTree;

namespace GlobalEditor{
extern MainWindow*   gMainWindow;
extern EditorBezier* gEditor;
extern QD3DWidgetBezier*   gViewX;
extern QD3DWidgetBezier*   gViewY;
extern QD3DWidgetBezier*   gViewZ;
extern QD3DWidgetBezier*   gViewF;
extern ObjectTree*   gObjTree;


extern D3DXCOLOR gColourPointSelected;
extern D3DXCOLOR gColourPointNotSelected;
};

#endif // EDITORGLOBAL_HPP
