#include "HE_face.hpp"
#include "HE_edge.hpp"
#include "HE_vert.hpp"

HE_face::HE_face(const HE_face &rhs)
{
  mEdge = rhs.mEdge;
  mId   = rhs.mId;
}

void HE_face::setEdge( const HE_edge* e )
{
  mEdge = const_cast<HE_edge*> (e);
}

std::vector<HE_vert*> HE_face::vertices() const
{
  HE_edge*  initial = mEdge;
  HE_edge*  current = mEdge;

  std::vector<HE_vert*> vert;

  do{
    vert.push_back( current->mVert );
    current = current->mNext;
  }while ( current != initial );

  return vert;
}

std::vector<HE_edge *> HE_face::edges() const
{
  HE_edge*  initial = mEdge;
  HE_edge*  current = mEdge;

  std::vector<HE_edge*> edges;

  do{
    edges.push_back( current );
    current = current->mNext;
  }while ( current != initial );

  return edges;
}

//  \param    aV1   vertex of the face
//  \param    aV2   vertex of the face (different from the previous)
//  \returns  ordered list of edges going from aV1 to aV2
std::vector<HE_edge*> HE_face::edgeChain( const HE_vert* aV1, const HE_vert* aV2 ) const
{
  std::vector<HE_edge*> allEdges = edges();

  unsigned int i;
  bool found = false;
  HE_edge* e;
  for( i = 0; !found ; ++i)
    if ( allEdges[i]->mVert == aV1 )
    {
      e = allEdges[i]->mNext;
      found = true;
    }

  std::vector<HE_edge*> edges;
  found = false;
  while (!found)
  {
    edges.push_back( e );
    if ( e->mVert == aV2 )
      found = true;
    else
      e = e->mNext;
  }

  return edges;
}

HE_edge * HE_face::edgeIncidentToVertex(HE_vert *aVertex) const
{
  std::vector<HE_edge *> e = edges();

  for( unsigned int i = 0; i < e.size(); ++i)
    if ( e[i]->mVert == aVertex )
      return e[i];

  return NULL;
}


