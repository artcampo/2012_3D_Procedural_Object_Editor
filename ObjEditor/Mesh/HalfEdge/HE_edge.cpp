
#include "HE_vert.hpp"
#include "HE_face.hpp"

/*
HE_edge::HE_edge( HE_vert* v )
{
  mVert = const_cast<HE_vert*> (v);
}
*/
/*
HE_edge::HE_edge( const HE_edge& rhs )
{
  mId   = rhs.mId;
  mVert = rhs.mVert;
  mPair= rhs.mPair;
  mNext=rhs.mNext;
  mPrev=rhs.mPrev;
  mFace =rhs.mFace;
}

HE_edge::HE_edge( HE_vert* v, HE_face* f, int aId )
{
  mId   = aId;
  mVert = v;
  mFace = f;

  v->setEdge( this );
  f->setEdge( this );
}

HE_edge::HE_edge( HE_vert* v,  int aId )
{
  mId   = aId;
  mVert = v;
  v->setEdge( this );
}


void HE_edge::next(HE_edge* e)
{
  mNext = e;
  e->prevNoRec( this );
}

void HE_edge::nextNoRec(HE_edge* e)
{
  mNext = e;
  e->prevNoRec( this );
}

void HE_edge::prev(HE_edge *e)
{
  mPrev = e;
  e->nextNoRec( this );
}


void HE_edge::prevNoRec(HE_edge *e)
{
  mPrev = e;
}

void HE_edge::pair(HE_edge *pair)
{
  mPair = pair;
  pair->pairNoRec( this );
}

void HE_edge::pairNoRec(HE_edge *pair)
{
  mPair = pair;
}

std::vector<HE_face *> HE_edge::faces() const
{
  std::vector<HE_face*> f;

  f.push_back( mFace );
  f.push_back( mPair->mFace );

  return f;
}

//  v = vert(e)
//  \return   ei, such that vert(pair(ei))=v
std::vector<HE_edge *> HE_edge::nonIncidentEdges() const
{
  HE_edge*  initial = mPair;
  HE_edge*  current = initial;

  std::vector<HE_edge*> edges;
  current = current->mPrev->mPair;
  do
  {
    //counter-clockwise
    edges.push_back( current );
    current = current->mPrev->mPair;
  }while ( current != initial );

  return edges;
}

*/
