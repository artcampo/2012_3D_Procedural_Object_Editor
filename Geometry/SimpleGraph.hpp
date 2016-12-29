#ifndef SIMPLEGRAPH_HPP
#define SIMPLEGRAPH_HPP

#include <algorithm>
#include <vector>

class SimpleGraph{

public:
  SimpleGraph();
  void  addEdge ( const int eId, const int v1, const int v2 );

  std::vector<int>  verticesOfDegree      ( const int degree ) const;
  std::vector<int>  verticesOfDegreeAbove ( const int degree ) const;

  std::vector<int>  edgesOfVertex ( const int v1 ) const;
  int               degreeOfVertex( const int v1 ) const;

private:
  std::vector< std::vector<int> >  v;
  int vMaxId;
};

#endif // SIMPLEGRAPH_HPP
