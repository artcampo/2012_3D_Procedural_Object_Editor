#include "MeshSerial.hpp"

MeshSerial::MeshSerial( const Mesh& rhs )
{
  mMeshObject.resize( rhs.mMeshObject.size() );
  mSelectedComponent = rhs.mSelectedComponent;
  mStack = new OpStack( *rhs.mStack );

  for ( unsigned int j = 0; j < rhs.mMeshObject.size() ; ++j)
  {
    mMeshObject[j] = new MeshComponentSerial( *rhs.mMeshObject[j] );
  }

  mSelO.resize( rhs.mMeshObject.size() );
  mTextures = rhs.mTextures;
}

