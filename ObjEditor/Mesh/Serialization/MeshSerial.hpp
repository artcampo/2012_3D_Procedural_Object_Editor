#ifndef _MESH_SERIAL_H
#define _MESH_SERIAL_H

#pragma once
#include <vector>
#include <list>

#include "ObjEditor/Mesh/Mesh.hpp"
#include "ObjEditor/Mesh/MeshComponent.hpp"
#include "App/EditableMeshModel.h"
#include "App/EditableMeshModelProcTex.h"

#include "Geometry/Intersections.hpp"
#include "App/pointlistmodel.h"
#include "ObjEditor/ObjEditorSharedData.hpp"
#include "ObjEditor/Mesh/OpStack.hpp"
#include "ObjEditor/Mesh/OpInfo.hpp"
#include "ObjEditor/Mesh/OperationsInfo.hpp"
#include "ObjEditor/Mesh/OperationsOptions.hpp"
#include "ObjEditor/Mesh/Serialization/MeshComponentSerial.hpp"

//io - qt
#include <QDataStream>
//passing text description
#include <sstream>
#include <iostream>

using namespace ObjEd;

class MeshSerial
{
public:
  ~MeshSerial();
  MeshSerial( const Mesh& rhs );

public:

  std::vector<MeshComponentSerial*>   mMeshObject;
  int               mSelectedComponent;
  OpStack*    mStack;
  std::vector<bool> mSelO;
  std::vector<ProcTex>   mTextures;

  friend class HE_mesh;
  friend class Mesh;

};

#endif // _MESH_SERIAL_H
