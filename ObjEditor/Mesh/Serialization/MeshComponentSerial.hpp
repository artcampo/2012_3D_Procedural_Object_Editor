#ifndef _MESH_COMPONENT_SERIAL_H
#define _MESH_COMPONENT_SERIAL_H

#include <list>
#include <algorithm>
#include <string.h>   //wcscpy_s


#include "ObjEditor/ObjEditorSharedData.hpp"

#include "ObjEditor/Mesh/vertex.hpp"
#include "ObjEditor/Mesh/Edge.hpp"
#include "ObjEditor/Mesh/Face.hpp"
#include "ObjEditor/Mesh/OperationsInfo.hpp"
#include "ObjEditor/Mesh/OperationsOptions.hpp"
#include "ObjEditor/Mesh/OpStack.hpp"
#include "ObjEditor/Mesh/OpInfo.hpp"

#include "App/EditableMeshModel.h"
#include "App/EditableMeshModelProcTex.h"

#include "ObjEditor/Mesh/MeshHalfEdge.hpp"
#include "ObjEditor/Mesh/HalfEdge/HE_vert.hpp"
#include "Geometry/SimpleGraph.hpp"
#include "Geometry/DxMathHelpers.hpp"

#include "Shader/ShaderHelpers.hpp"

using namespace ObjEd;



class MeshComponentSerial
{

public:
  ~MeshComponentSerial();
  MeshComponentSerial( const MeshComponent& rhs );

private:
  HE_meshSerial*     mMesh;
  int          mComponentId;

private:  


  std::vector<bool> mSelV;
  std::vector<bool> mSelE;
  std::vector<bool> mSelF;

  std::vector<int>  mTextureIdPerFace;

private:
  friend class MeshSerial;
  friend class MeshComponent;

};

#endif // _MESH_COMPONENT_SERIAL_H
