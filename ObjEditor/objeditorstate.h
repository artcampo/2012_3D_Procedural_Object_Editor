#ifndef OBJEDITORSTATE_H
#define OBJEDITORSTATE_H

#include "ObjEditor/Mesh/Mesh.hpp"
#include "ObjEditor/Mesh/Serialization/MeshSerial.hpp"

class ObjEditorState
{
public:
    ObjEditorState( const Mesh& aMesh );

    Mesh* restoreState( );
private:
    MeshSerial*  mMesh;

};

#endif // OBJEDITORSTATE_H
