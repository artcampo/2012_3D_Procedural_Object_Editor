#ifndef OPERATIONSINFO_HPP
#define OPERATIONSINFO_HPP

#include "ObjEditor/ObjEditorSharedData.hpp"

namespace ObjEd{

//  Information per face
struct ExtrudeInfo{
  std::vector<D3DXVECTOR3> center;
  D3DXVECTOR3 normal;
  float       value;
  int         numParts;
  std::vector< std::vector<HE_vert*> > newVertices;
};

struct ExtrudeInnerInfo{
  std::vector<D3DXVECTOR3> center;
  std::vector<D3DXVECTOR3> u;
  std::vector<D3DXVECTOR3> v;
  std::vector< std::vector<HE_vert*> > newVertices;
  int         numParts;
  float       value;
};

struct ExtrudeMatrixInfo{
  std::vector<D3DXVECTOR3> center;
  D3DXVECTOR3 normal;
  float       value;
  int         numParts;
  std::vector< std::vector<HE_vert*> > newVertices;
};


struct BevelVInfo{
  D3DXVECTOR3               center;
  std::vector<D3DXVECTOR3>  direction;
  std::vector<HE_vert*>     newVertices;  
  float       value;
  float       previousValue;
};

struct ScaleInfo{
  D3DXVECTOR3       center;
  D3DXVECTOR3       value;
};

struct BevelEgroup{
};

struct BevelEInfo{
  int   numVertices;
  std::vector<D3DXVECTOR3>  center1;
  std::vector<D3DXVECTOR3>  direction1;
  std::vector<D3DXVECTOR3>  direction2;
  std::vector<HE_vert*>     newVertices;
  float       value;
  float       previousValue;
};


struct NorScaleInfo{
  D3DXVECTOR3               center;
  std::vector<D3DXVECTOR3>  initialVertex;
  std::vector<HE_vert*>     newVertices;
  float       value;
};

struct KniveLineInfo{
  D3DXVECTOR3 segmentPointB;
  D3DXVECTOR3 segmentPointE;
  Selection   entityB;
  Selection   entityE;
};

};

#endif // OPERATIONSINFO_HPP
