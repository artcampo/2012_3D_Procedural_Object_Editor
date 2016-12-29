#include "Geometry/SimpleGraph.hpp"

void  SimpleGraph::addEdge ( const int eId, const int v1, const int v2 )
{
  vMaxId = std::max( vMaxId, v1);
  vMaxId = std::max( vMaxId, v2);

  v.resize( vMaxId + 1 );

  v[v1].push_back( eId );
  v[v2].push_back( eId );
}


SimpleGraph::SimpleGraph()
{
  vMaxId = 0;
}

std::vector<int> SimpleGraph::verticesOfDegree(const int degree) const
{
  std::vector<int> vert;

  for (int i = 0; i <= vMaxId; ++i)
  {
    if (v[i].size() == degree)
      vert.push_back( i );
  }

  return vert;
}


std::vector<int> SimpleGraph::verticesOfDegreeAbove(const int degree) const
{
  std::vector<int> vert;

  for (int i = 0; i <= vMaxId; ++i)
  {
    if (v[i].size() > degree)
      vert.push_back( i );
  }

  return vert;
}


std::vector<int>  SimpleGraph::edgesOfVertex( const int v1 ) const
{
  return v[v1];
}

int SimpleGraph::degreeOfVertex(const int v1) const
{
  return v[v1].size();
}


