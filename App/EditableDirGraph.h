#ifndef _EditableDirGraph_H
#define _EditableDirGraph_H

#include <vector>
#include "App/dx_misc.hpp"

#include "pointlistmodel.h"
#include "linelistmodel.h"
#include "App/pointlistmodel.h"
#include "Geometry/DirGraph/DirGraph.hpp"

class EditableDirGraph
{
public:
    ~EditableDirGraph();
    EditableDirGraph(  std::vector<VertexPosCol>& aEdgePoints,
                       PointListModel* aVertices,                       
                       D3DXCOLOR& aColour,
                       ID3D10Device* aMd3dDevice);

    static EditableDirGraph*  convert ( const DirectedGraph::DirGraph& aDirGraph,
                                        const int aSelectedVertex,
                                        const int aHighlightedVertex,
                                        const int aHighlightedEdge,
                                        ID3D10Device* aMd3dDevice);

    void displayEdges();
    void displayVertices();    
private:  
  DxMeshModelBatch<VertexPosCol>* mMeshEdges;
  PointListModel*                 mMeshVertices;
  bool mHasEdges;
};

#endif // _EditableDirGraph_H
