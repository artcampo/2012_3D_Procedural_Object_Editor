#include "EditableDirGraph.h"

EditableDirGraph* EditableDirGraph::convert ( const DirectedGraph::DirGraph& aDirGraph,
                                              const int aSelectedVertex,
                            const int aHighlightedVertex,
                            const int aHighlightedEdge,
                            ID3D10Device* aMd3dDevice)
{
  EditableDirGraph* m;

  std::vector<VertexPosCol> edgePoints;

  std::vector<DirectedGraph::Edge> edges = aDirGraph.edges();
  for ( unsigned int i = 0 ; i < edges.size() ; ++i )
  {
    VertexPosCol v1, v2;
    v1.col = D3DXCOLOR(1,1,1,1);
    v2.col = D3DXCOLOR(1,1,1,1);
    if ( i == aHighlightedEdge )
    {
      v1.col = D3DXCOLOR(1,0,0,1);
      v2.col = D3DXCOLOR(1,0,0,1);
    }
    v1.pos = aDirGraph.vertex( edges[i].vb ).pos;
    v2.pos = aDirGraph.vertex( edges[i].ve ).pos;
    edgePoints.push_back( v1 );
    edgePoints.push_back( v2 );
  }

  std::vector<VertexPointPSC> verticesData;
  for ( unsigned int i = 0 ; i < aDirGraph.numVertices(); ++i )
  {
    VertexPointPSC v;
    v.centerW = aDirGraph.vertex(i).pos;
    v.sizeW   = D3DXVECTOR2(0.35,0.35);
    v.color   = D3DXCOLOR(1,1,1,1);
    if ( i == aSelectedVertex )
      v.color   = D3DXCOLOR(0,0,1,1);
    if ( i == aHighlightedVertex )
      v.color   = D3DXCOLOR(1,0,0,1);

    verticesData.push_back( v );
  }
  PointListModel* vertices = new PointListModel( verticesData , aMd3dDevice);



  m = new EditableDirGraph( edgePoints, vertices, D3DXCOLOR(1,0,0,1), aMd3dDevice );

  return m;
}

EditableDirGraph::EditableDirGraph( std::vector<VertexPosCol>& aEdgePoints,
                                    PointListModel* aVertices,
                                    D3DXCOLOR& aColour,
                                    ID3D10Device* aMd3dDevice)
{

  if ( aEdgePoints.size() > 0)
  {
    DxMeshModel<VertexPosCol>* mesh2;
    mesh2  = new DxMeshModel<VertexPosCol>( sizeof(VertexPosCol), aEdgePoints.size(), (VertexPosCol*) (&aEdgePoints[0]) );
    mMeshEdges = new DxMeshModelBatch<VertexPosCol>( sizeof(VertexPosCol), D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST, aMd3dDevice );
    mMeshEdges->addMeshModel( mesh2 );
    mMeshEdges->createVB( );
    mHasEdges = true;
  }
  else
    mHasEdges = false;


  mMeshVertices = aVertices;

}

EditableDirGraph::~EditableDirGraph()
{
  if (mHasEdges)
    delete mMeshEdges;
  delete mMeshVertices;
}



void EditableDirGraph::displayEdges()
{
  if (mHasEdges)
    mMeshEdges->DisplaySansShader();
}

void EditableDirGraph::displayVertices()
{  
  mMeshVertices->display();
}
