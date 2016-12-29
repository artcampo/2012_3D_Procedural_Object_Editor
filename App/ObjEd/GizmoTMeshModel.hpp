#ifndef _GIZMO_T_MESH_MODEL_H
#define _GIZMO_T_MESH_MODEL_H


#include "App/pointlistmodel.h"
#include "App/linelistmodel.h"

class GizmoTMeshModel
{
public:
  ~GizmoTMeshModel();
  GizmoTMeshModel( std::vector<VertexPosCol>& mVertices, ID3D10Device* aMd3dDevice );

  void display();

private:

  DxMeshModelBatch<VertexPosCol>*  mAxis;

};

#endif // _GIZMO_T_MESH_MODEL_H
