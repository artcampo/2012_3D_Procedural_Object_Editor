#ifndef MESHHALFEDGE_SERIAL_HPP
#define MESHHALFEDGE_SERIAL_HPP

#include "ObjEditor/Mesh/vertex.hpp"
#include "ObjEditor/Mesh/Face.hpp"
#include "ObjEditor/Mesh/Edge.hpp"
#include <vector>
#include <algorithm>

#include "ObjEditor/Mesh/MeshHalfEdge.hpp"
#include "ObjEditor/Mesh/HalfEdge/HE_edge.hpp"
#include "ObjEditor/Mesh/HalfEdge/HE_vert.hpp"
#include "ObjEditor/Mesh/HalfEdge/HE_face.hpp"

class HE_meshSerial{
public:
  //~HE_meshSerial();
  HE_meshSerial( const HE_mesh& rhs );


private:
  std::vector<HE_edge>  mHalfEdges;
  std::vector<HE_face>  mFaces;
  std::vector<HE_vert>  mVertices;

  std::vector<int>  mHalfEdgeOfFullEdge;   //given an edge, one of its half-edges
  std::vector<int>  mFullEdgeofHalfEdge;   //given any half edge, its edge

public:
  friend class HE_mesh;
  friend class MeshSerial;
  friend class MeshComponentSerial;


};

#endif // MESHHALFEDGE_SERIAL_HPP
