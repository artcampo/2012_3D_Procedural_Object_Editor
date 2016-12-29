#ifndef _HE_EDGE_HPP
#define _HE_EDGE_HPP

#include <vector>

class HE_vert;
class HE_face;
class HE_mesh;

#include "HE_vert.hpp"
#include "HE_face.hpp"

class HE_edge{
public:
  //HE_edge( const HE_vert* v );  
  HE_edge(){};
  HE_edge( const HE_edge& rhs )
  {
    mId   = rhs.mId;
    mVert = rhs.mVert;
    mPair= rhs.mPair;
    mNext=rhs.mNext;
    mPrev=rhs.mPrev;
    mFace =rhs.mFace;
  }
  HE_edge( HE_vert* v, HE_face* f, int aId )
  {
    mId   = aId;
    mVert = v;
    mFace = f;

    v->setEdge( this );
    f->setEdge( this );
  }


  void  pair( HE_edge* pair )
  {
    mPair = pair;
    pair->pairNoRec( this );
  }
  void  next( HE_edge* e )
  {
    mNext = e;
    e->prevNoRec( this );
  }
  void  prev( HE_edge* e )
  {
    mPrev = e;
    e->nextNoRec( this );
  }

  std::vector<HE_face*> faces() const
  {
    std::vector<HE_face*> f;

    f.push_back( mFace );
    f.push_back( mPair->mFace );

    return f;
  }

  std::vector<HE_edge*> nonIncidentEdges () const
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

public:
  int         mId;
  HE_vert*    mVert;
  HE_edge*    mPair;
  HE_edge*    mNext;
  HE_edge*    mPrev;
  HE_face*    mFace;
private:
  void  nextNoRec( HE_edge* e )
  {
    mNext = e;
    e->prevNoRec( this );
  }
  void  prevNoRec( HE_edge* e )
  {
    mPrev = e;
  }

  void  pairNoRec( HE_edge* pair )
  {
    mPair = pair;
  }
  //HE_edge& operator=( const HE_edge& rhs ); //forbidden
  friend class HE_mesh;

private:
  // this constructor is only for special cases such as BevelE
  HE_edge( HE_vert* v, int aId )
  {
    mId   = aId;
    mVert = v;
    v->setEdge( this );
  }


};

#endif // _HE_EDGE_HPP
