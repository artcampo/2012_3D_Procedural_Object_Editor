#ifndef _DIRGRAPH_DATA_TYPES_HPP
#define _DIRGRAPH_DATA_TYPES_HPP

//#include <algorithm>
#include <vector>

#include "GameBall/TrackBp.hpp"

namespace DirectedGraph {
class Edge{
  public:
    int       vb;
    int       ve;
    TrackBp::EdgeData  edgeData;
    Edge (){};
    Edge ( const int aIdv1, const int aIdv2, const TrackBp::EdgeData& aEdge )
    {
      vb        = aIdv1;
      ve        = aIdv2;
      edgeData  = aEdge;
    }
    bool operator ==(const Edge& rhs) const
    {
      if ( this->vb == rhs.vb && this->ve == rhs.ve)
        return true;
      else 
        return false;
    }
  };


  class Vertex{
  public:
    std::vector<TrackBp::EdgeData> edgeData;
    std::vector<int>      desc;
    std::vector<int>      par;
  };
};

#endif
