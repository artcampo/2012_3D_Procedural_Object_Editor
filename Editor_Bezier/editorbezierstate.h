#ifndef EDITORBEZIERSTATE_H
#define EDITORBEZIERSTATE_H

#include "Editor_Bezier/bezier.h"

class EditorBezierState
{
public:
    EditorBezierState( const std::vector<Bezier>& aBezierObjects, const int& aFirstFreeUniqueId );
    ~EditorBezierState();

    void restoreBezierState( std::vector<Bezier>& aBezierObjects, int& aFirstFreeUniqueId );
private:
    std::vector<Bezier> mBezierObjects;
    int                 mFirstFreeUniqueId;
};

#endif // EDITORBEZIERSTATE_H
