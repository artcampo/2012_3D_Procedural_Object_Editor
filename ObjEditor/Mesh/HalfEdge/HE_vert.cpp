#include "HE_vert.hpp"
#include "HE_face.hpp"
#include "HE_edge.hpp"

HE_vert::HE_vert(const HE_vert &rhs)
{
  mVertex = rhs.mVertex;
  mId = rhs.mId;
  mEdge = rhs.mEdge;
}


HE_vert::HE_vert( const VertexObj& av, const int aId  )
{
  mVertex = av;
  mId     = aId;
}

void HE_vert::setEdge(const HE_edge *e)
{
  mEdge = const_cast<HE_edge*> (e);
}

void HE_vert::modifyVertex(const VertexObj &v)
{
  mVertex = v;
}




//  \return all incident half edges to the vertex
std::vector<HE_edge *> HE_vert::edges() const
{
  HE_edge*  initial = mEdge;
  HE_edge*  current = mEdge;

  std::vector<HE_edge*> edges;

  do{
    //counter-clockwise
    edges.push_back( current->mPair->mPrev );
    current = current->mPair->mPrev;
  }while ( current != initial );

  return edges;
}


std::vector<HE_edge *> HE_vert::edgesSorted ( HE_edge* firstIncidentEdge ) const
{
  HE_edge*  initial = firstIncidentEdge;
  HE_edge*  current = firstIncidentEdge;

  std::vector<HE_edge*> edges;

  do{
    //counter-clockwise
    edges.push_back( current );
    current = current->mPair->mPrev;
  }while ( current != initial );

  return edges;
}

std::vector<HE_edge *> HE_vert::edgesClockWise() const
{
  HE_edge*  initial = mEdge;
  HE_edge*  current = mEdge;

  std::vector<HE_edge*> edges;

  do{
    //clockwise
    edges.push_back( current->mNext->mPair );
    current = current->mNext->mPair;
  }while ( current != initial );

  return edges;
}

std::vector<HE_face *> HE_vert::faces() const
{
  HE_edge*  initial = mEdge;
  HE_edge*  current = mEdge;

  std::vector<HE_face*> faces;

  do{
    faces.push_back( current->mNext->mPair->mFace );
    current = current->mNext->mPair;
  }while ( current != initial );

  return faces;
}

