#include "Face.hpp"

Face::Face( std::vector<int>& aVertices, std::vector<int>& aFullEdges )
{
  mVerticesId = aVertices;
  mEdgesId = aFullEdges;
}

int Face::nVertices() const
{
  return mVerticesId.size();
}

int Face::vId(const int i) const
{
  return mVerticesId[i];
}

int Face::nDecompositionTriangles() const
{
  return mVerticesId.size() - 2;
}

Face::TriangleDecomposition Face::decompTriangle( const int i) const
{
  Face::TriangleDecomposition decomp;

  decomp.v1 = 0;
  decomp.v2 = i + 1;
  decomp.v3 = i + 2;

  return decomp;
}

int Face::nEdges() const
{
  return mEdgesId.size();
}

int Face::eId(const int i) const
{
  return mEdgesId[i];
}

void Face::convertHalfEdgesToFullEdges(const std::vector<int> &aHalfEdgeToFullEdge)
{
  for (int i = 0; i < mEdgesId.size(); ++i)
    mEdgesId[i] = aHalfEdgeToFullEdge[ mEdgesId[i] ];
}

