#ifndef HE_VERT_HPP
#define HE_VERT_HPP

#include <vector>
#include "ObjEditor/Mesh/vertex.hpp"
class HE_edge;
class HE_face;

class HE_vert{
public:
  HE_vert(){};
  HE_vert(const HE_vert& rhs);
  HE_vert( const VertexObj& av, const int aId );  

  void  setEdge( const HE_edge* e );
  void  modifyVertex ( const VertexObj &v );


  std::vector<HE_face*> faces () const;
  std::vector<HE_edge*> edges () const;  
  std::vector<HE_edge*> edgesSorted ( HE_edge* firstIncidentEdge ) const;

  std::vector<HE_edge *> edgesClockWise() const;

public:
  VertexObj   mVertex;
  int         mId;
private:  
  HE_edge*    mEdge;

  friend class HE_mesh; //for serialization
  friend class HE_meshSerial;
  //HE_vert& operator=( const HE_vert& rhs ){};
};

#endif // HE_VERT_HPP
