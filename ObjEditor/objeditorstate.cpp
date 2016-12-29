#include "ObjEditorState.h"

ObjEditorState::ObjEditorState( const Mesh& aMesh )
{
  mMesh = new MeshSerial(aMesh);
}


Mesh* ObjEditorState::restoreState()
{
  return (new Mesh( *mMesh ));
}


