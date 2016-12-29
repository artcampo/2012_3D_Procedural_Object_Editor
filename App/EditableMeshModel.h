#ifndef _EditableMeshModel_H
#define _EditableMeshModel_H

#include <vector>
#include "App/dx_misc.hpp"

#include "pointlistmodel.h"
#include "linelistmodel.h"
#include "Editor_Bezier/EditorGlobal.hpp"
#include "App/pointlistmodel.h"

class EditableMeshModel
{
public:
    ~EditableMeshModel();
    EditableMeshModel( std::vector<VertexPosNorCol>& aTrianglePoints,
                       std::vector<VertexPosCol>& aEdgePoints,
                       PointListModel* aVertices,
                       bool aHasVertices,
                 D3DXCOLOR& aColour,
                 ID3D10Device* aMd3dDevice);

    void displayTriangles();
    void displayEdges();
    void displayVertices();
    bool hasVertices();
private:
  DxMeshModelBatch<VertexPosNorCol>* mMeshTriangles;
  DxMeshModelBatch<VertexPosCol>* mMeshEdges;
  PointListModel*                 mMeshVertices;
  bool  mHasVertices;
};

#endif // _EditableMeshModel_H
