#include "ObjEditor/Mesh/MeshComponent.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_face.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_edge.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_vert.hpp"

//  Given a new component it initalizes it to a cube
void   MeshComponent::createCubePrimitive()
{

  HE_vert* v1 = mMesh->newVertex ( VertexObj( D3DXVECTOR3( 0, 1, 0 ) ) );
  HE_vert* v2 = mMesh->newVertex ( VertexObj( D3DXVECTOR3( 1, 1, 0 ) ) );
  HE_vert* v3 = mMesh->newVertex ( VertexObj( D3DXVECTOR3( 1, 1, 1 ) ) );
  HE_vert* v4 = mMesh->newVertex ( VertexObj( D3DXVECTOR3( 0, 1, 1 ) ) );
  HE_vert* v5 = mMesh->newVertex ( VertexObj( D3DXVECTOR3( 0, 0, 0 ) ) );
  HE_vert* v6 = mMesh->newVertex ( VertexObj( D3DXVECTOR3( 1, 0, 0 ) ) );
  HE_vert* v7 = mMesh->newVertex ( VertexObj( D3DXVECTOR3( 1, 0, 1 ) ) );
  HE_vert* v8 = mMesh->newVertex ( VertexObj( D3DXVECTOR3( 0, 0, 1 ) ) );

  HE_face* fa = mMesh->newFace();
  HE_face* fb = mMesh->newFace();
  HE_face* fc = mMesh->newFace();
  HE_face* fd = mMesh->newFace();
  HE_face* fe = mMesh->newFace();
  HE_face* ff = mMesh->newFace();


  //face a
  HE_edge* ea1 = mMesh->newHalfEdge( v1, fa );
  HE_edge* ea2 = mMesh->newHalfEdge( v2, fa );
  HE_edge* ea3 = mMesh->newHalfEdge( v3, fa );
  HE_edge* ea4 = mMesh->newHalfEdge( v4, fa );

  ea1->next( ea2 );
  ea2->next( ea3 );
  ea3->next( ea4 );
  ea4->next( ea1 );

  //face b
  HE_edge* eb1 = mMesh->newHalfEdge( v1, fb );
  HE_edge* eb2 = mMesh->newHalfEdge( v5, fb );
  HE_edge* eb3 = mMesh->newHalfEdge( v6, fb );
  HE_edge* eb4 = mMesh->newHalfEdge( v2, fb );

  eb1->next( eb2 );
  eb2->next( eb3 );
  eb3->next( eb4 );
  eb4->next( eb1 );

  //face c
  HE_edge* ec1 = mMesh->newHalfEdge( v2, fc );
  HE_edge* ec2 = mMesh->newHalfEdge( v6, fc );
  HE_edge* ec3 = mMesh->newHalfEdge( v7, fc );
  HE_edge* ec4 = mMesh->newHalfEdge( v3, fc );

  ec1->next( ec2 );
  ec2->next( ec3 );
  ec3->next( ec4 );
  ec4->next( ec1 );

  //face d
  HE_edge* ed1 = mMesh->newHalfEdge( v3, fd );
  HE_edge* ed2 = mMesh->newHalfEdge( v7, fd );
  HE_edge* ed3 = mMesh->newHalfEdge( v8, fd );
  HE_edge* ed4 = mMesh->newHalfEdge( v4, fd );

  ed1->next( ed2 );
  ed2->next( ed3 );
  ed3->next( ed4 );
  ed4->next( ed1 );

  //face e
  HE_edge* ee1 = mMesh->newHalfEdge( v4, fe );
  HE_edge* ee2 = mMesh->newHalfEdge( v8, fe );
  HE_edge* ee3 = mMesh->newHalfEdge( v5, fe );
  HE_edge* ee4 = mMesh->newHalfEdge( v1, fe );

  ee1->next( ee2 );
  ee2->next( ee3 );
  ee3->next( ee4 );
  ee4->next( ee1 );

  //face f
  HE_edge* ef1 = mMesh->newHalfEdge( v6, ff );
  HE_edge* ef2 = mMesh->newHalfEdge( v5, ff );
  HE_edge* ef3 = mMesh->newHalfEdge( v8, ff );
  HE_edge* ef4 = mMesh->newHalfEdge( v7, ff );

  ef1->next( ef2 );
  ef2->next( ef3 );
  ef3->next( ef4 );
  ef4->next( ef1 );

  //now make pairs
  ea1->pair( ee1 );
  ea2->pair( eb1 );
  ea3->pair( ec1 );
  ea4->pair( ed1 );

  ef1->pair( ec3 );
  ef2->pair( eb3 );
  ef3->pair( ee3 );
  ef4->pair( ed3 );

  eb4->pair( ec2 );
  ec4->pair( ed2 );
  ed4->pair( ee2 );
  ee4->pair( eb2 );

  //  link to real edges
  mMesh->newFullEdge( ea1, ee1 );
  mMesh->newFullEdge( ea2, eb1 );
  mMesh->newFullEdge( ea3, ec1 );
  mMesh->newFullEdge( ea4, ed1 );

  mMesh->newFullEdge( eb4, ec2 );
  mMesh->newFullEdge( ec4, ed2 );
  mMesh->newFullEdge( ed4, ee2 );
  mMesh->newFullEdge( ee4, eb2 );

  mMesh->newFullEdge( ef1, ec3 );
  mMesh->newFullEdge( ef2, eb3 );
  mMesh->newFullEdge( ef3, ee3 );
  mMesh->newFullEdge( ef4, ed3 );

  //  set selection tables
  resizeSelectionVectors();

  checkIntegrity();
}
