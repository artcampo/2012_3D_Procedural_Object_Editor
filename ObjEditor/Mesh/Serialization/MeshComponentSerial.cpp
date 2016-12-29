#include "MeshComponentSerial.hpp"
#include "UI/MainWindow.hpp"
#include "ObjEditor/Mesh/Serialization/MeshHalfEdgeSerial.hpp"

MeshComponentSerial::~MeshComponentSerial()
{
  delete mMesh;
}

MeshComponentSerial::MeshComponentSerial( const MeshComponent& rhs )
{
  mComponentId = rhs.mComponentId;

  mSelV.resize( rhs.mSelV.size() );
  for ( int i = 0; i < rhs.mSelV.size() ; ++i)
    mSelV[i] = rhs.mSelV[i];

  mSelE.resize( rhs.mSelE.size() );
  for ( int i = 0; i < rhs.mSelE.size() ; ++i)
    mSelE[i] = rhs.mSelE[i];

  mSelF.resize( rhs.mSelF.size() );
  for ( int i = 0; i < rhs.mSelF.size() ; ++i)
    mSelF[i] = rhs.mSelF[i];

  mTextureIdPerFace.resize( rhs.mTextureIdPerFace.size() );
  for ( int i = 0; i < rhs.mTextureIdPerFace.size() ; ++i)
    mTextureIdPerFace[i] = rhs.mTextureIdPerFace[i];

  mMesh = new HE_meshSerial( const_cast<HE_mesh&> (*rhs.mMesh) );
}
