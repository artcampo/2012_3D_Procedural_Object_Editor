#ifndef			DX_DEF_MESH_MODEL_BASIC
#define			DX_DEF_MESH_MODEL_BASIC

#include    "dx_misc.hpp"
#include    <vector>



/////////////////////////////////////////////////////////////////////////////
//		DxMeshModel. templatizated over VertexLayout. 
//
//    A bare container that must be merged into a Batch to be renderable.
//
//    Each vertex is made 
/////////////////////////////////////////////////////////////////////////////

template <class VertexLayout>
class DxMeshModel {

public:

  DxMeshModel           ( int vertexLayoutSize, int nVertices, 
                          VertexLayout* aVertices  );
  
private:
  std::vector<VertexLayout>	mVertices;
  int                       mNumVertices;
  int                       mVertexLayoutSize;

  template <class VertexLayout> friend class DxMeshModelBatch;
};

template <class VertexLayout>
class DxMeshModelBatch {

public:

  ~DxMeshModelBatch();
  DxMeshModelBatch        ( int vertexLayoutSize, 
                            D3D10_PRIMITIVE_TOPOLOGY aTopology,
                            ID3D10Device* aMd3dDevice);
  void addMeshModel       ( DxMeshModel<VertexLayout>* aMesh );

	bool createVB   	      ( );
	void Display		        ( );
  void DisplaySansShader  ( );

private:
  ID3D10Device* md3dDevice;

	std::vector<VertexLayout> mVertices;
  int                       mNumVertices;
  int                       mVertexLayoutSize;
  D3D10_PRIMITIVE_TOPOLOGY  mTopology;

  ID3D10Buffer*             mVertexBuffer;

  friend class DxAnimModel;

};

#include    "DxMeshModel.cpp"

#endif //DX_DEF_MESH_MODEL_BASIC
