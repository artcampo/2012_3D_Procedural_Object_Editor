#include "editorbezierstate.h"

EditorBezierState::EditorBezierState( const std::vector<Bezier>& aBezierObjects, const int& aFirstFreeUniqueId )
{
  mBezierObjects.resize( aBezierObjects.size() );
  for ( unsigned int i = 0 ; i < aBezierObjects.size(); ++i )
  {
    mBezierObjects[i] = aBezierObjects[i];
  }

  mFirstFreeUniqueId = aFirstFreeUniqueId;
}

EditorBezierState::~EditorBezierState()
{
}

void EditorBezierState::restoreBezierState(std::vector<Bezier> &aBezierObjects, int& aFirstFreeUniqueId )
{
  aFirstFreeUniqueId = mFirstFreeUniqueId;
  aBezierObjects.resize( mBezierObjects.size() );
  for ( unsigned int i = 0 ; i < aBezierObjects.size(); ++i )
  {
    aBezierObjects[i] = mBezierObjects[i];
  }
}


