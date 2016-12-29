#ifndef			DX_MESH_MODEL_INDEXED_BASIC
#define			DX_MESH_MODEL_INDEXED_BASIC

#include <vector>

#include    "App\dx_misc.hpp"
#include    "Shader\Shader.hpp"

/////////////////////////////////////////////////////////////////////////////
//		DxMeshModelIndexed
//
//    A bare container that must be merged into a Batch to be renderable.
//
//    Each vertex is made 
/////////////////////////////////////////////////////////////////////////////

class DxMeshModelIndexed {

public:
  DxMeshModelIndexed           ( int vertexLayoutSize, int nVertices,
                          VertexTextEd* aVertices  );
  ~DxMeshModelIndexed          ( );
  void setIndices       (DWORD* aIndices, DWORD aNumFaces);  
  bool isSetIndices     ( );  

private:
  int                         mVertexLayoutSize;
  bool                        mIsSetIndices;  
  int                         mNumVertices;
  std::vector<VertexTextEd> 	mVertices;
  std::vector<DWORD>          mIndices;
  DWORD                       mNumTriangles;  

  friend class DxMeshModelIndexedBatch;
};

class DxMeshModelIndexedBatch {

public:

  DxMeshModelIndexedBatch        ( int vertexLayoutSize,
                            D3D10_PRIMITIVE_TOPOLOGY aTopology,
                            ID3D10Device*           ad3dDevice);
  ~DxMeshModelIndexedBatch       ( );
  void addMeshModel       ( DxMeshModelIndexed* aMesh );
	bool createVB   	      ( );
  bool createIB           ( );  
	void display		        (Shader* afxBox);
  UINT getNumIndices      ( );

private:
  std::vector<VertexTextEd> 	    mVertices;
  UINT                            mVertexLayoutSize;
  UINT                            mNumObjects;
  D3D10_PRIMITIVE_TOPOLOGY        mTopology;
  ID3D10Buffer*                   mVertexBuffer;
  ID3D10Buffer*                   mIndexBuffer;
  DWORD                           mNumIndices;
  int                             mNumVertices;
  std::vector<DWORD>              mIndices;
  ID3D10Device*                   md3dDevice;

  //friend class DxAnimModel;

};

#endif //DX_MESH_MODEL_BASIC  
