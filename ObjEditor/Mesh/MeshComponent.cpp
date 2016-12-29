#include "MeshComponent.hpp"
#include "UI/MainWindow.hpp"
#include "ObjEditor/Mesh/HalfEdge/HE_edge.hpp"
#include "ObjEditor/Mesh/HalfEdge/HE_vert.hpp"
#include "ObjEditor/Mesh/HalfEdge/HE_face.hpp"

MeshComponent::~MeshComponent()
{
  delete mMesh;
  mSelV.clear();
  mSelE.clear();
  mSelF.clear();
  mTextureIdPerFace.clear();
}

MeshComponent::MeshComponent( const MeshComponentSerial& rhs )
{

  mHighlight.idComponent = -1;


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

  mMesh = new HE_mesh ( *rhs.mMesh );

}








int   MeshComponent::nVertices() const
{
  return mMesh->nVertices();
}

int   MeshComponent::nEdges() const
{
  return mMesh->nFullEdges();
}

int   MeshComponent::nFaces() const
{
  return mMesh->nFaces();
}

void MeshComponent::setMode(int aMode)
{
  mMode = aMode;
}

VertexObj   MeshComponent::vertex  ( int aId ) const
{
  return mMesh->vertex( aId );
}


Edge        MeshComponent::edge    ( int aId ) const
{  
  return mMesh->fullEdge(aId);
}

Face        MeshComponent::face    ( int aId ) const
{
  Face face = mMesh->face( aId );  
  return face;
}

EditableMeshModel* MeshComponent::convert( ID3D10Device* aDev, D3DXVECTOR3 aCamPos, ColourMapping* aColours )
{
  EditableMeshModel* meshDx;


  std::vector<VertexPosCol> edges;
  std::vector<Face> visibleFaces = facesFacingCamera( aCamPos );

  for ( int j = 0; j < visibleFaces.size() ; ++j )
  {
    for ( int i = 0; i < visibleFaces[j].nEdges(); ++i )
    {
      int edgeId = visibleFaces[j].eId(i);

      VertexPosCol v, u;
      v.pos = ( vertex(edge(edgeId).bId) ).pos;
      u.pos = ( vertex(edge(edgeId).eId) ).pos;

      D3DXCOLOR color;
      if ( mMode == eModeEdge && edgeId == mHighlight.idEntity )
        color = aColours->edgHigh;
      else
        if (mSelE[edgeId])
          color = aColours->edgSel;
        else
          color = aColours->edg;

      v.col = color;
      u.col = color;

      edges.push_back( v );
      edges.push_back( u );
    }
  }

  std::vector<VertexPosNorCol> triangles;
  for ( int i = 0; i < nFaces(); ++i )
  {
    Face f = face(i);

    Face::TriangleDecomposition t1 = f.decompTriangle(0);
    D3DXVECTOR3 normal = DxMath::normalOfTriangle( vertex( t1.v1 ).pos, vertex( t1.v2 ).pos, vertex( t1.v3 ).pos );

    VertexPosNorCol v;
    v.col = aColours->fac;
    if ( mMode == eModeFace && i == mHighlight.idEntity )
      v.col = aColours->facHigh;
    else
      if (mSelF[i])
        v.col = aColours->facSel;

    for (int k = 0; k < f.nDecompositionTriangles() ; ++k)
      {
      Face::TriangleDecomposition triangle = f.decompTriangle(k);
      D3DXVECTOR3 p1 = vertex( f.vId( triangle.v1 )).pos;
      D3DXVECTOR3 p2 = vertex( f.vId( triangle.v2 )).pos;
      D3DXVECTOR3 p3 = vertex( f.vId( triangle.v3 )).pos;

      v.pos = p1;
      v.nor = normal;
      triangles.push_back( v );
      v.pos = p2;
      v.nor = normal;
      triangles.push_back( v );
      v.pos = p3;
      v.nor = normal;
      triangles.push_back( v );
      }

  }


  std::vector<int> verticesId = uniqueVerticesOfFaces( visibleFaces );

  std::vector<VertexPointPSC> points;
  bool hasVertices = ( mMode == eModeVertex );
  PointListModel* p;
  if (hasVertices)
  {
    for ( int i = 0; i < verticesId.size(); ++i )
    {
      VertexPointPSC v;
      v.centerW = vertex( verticesId[i] ).pos;
      v.sizeW = D3DXVECTOR2( 0.1f,0.1f);
      if ( i == mHighlight.idEntity )
        v.color = aColours->verHigh;
      else
        if (mSelV[ verticesId[i] ])
          v.color = aColours->verSel;
        else
          v.color = aColours->ver;

      points.push_back( v );
    }

    p = new PointListModel( points, aDev );
  }



  meshDx = new EditableMeshModel( triangles, edges, p,
                                  hasVertices,
                                  D3DXCOLOR(1,0,0,1), aDev );
  return meshDx;
}

EditableMeshModelProcTex* MeshComponent::convertProcTex( ID3D10Device* aDev, D3DXVECTOR3 aCamPos, ColourMapping* aColours, int aNumTextures, std::vector<ProcTex>& aTextures )
{

  std::vector< std::vector<VertexPosNor> > triangles;
  std::vector< Shader* > shaders;

  for ( int j = 0; j < aNumTextures ; ++j)
  {
    std::vector<int> facesOfPartition;
    for ( int fi = 0; fi < nFaces() ; ++fi )
      if (mTextureIdPerFace[fi] == j)
        facesOfPartition.push_back( fi );

    if ( !facesOfPartition.empty() )
    {
      int currentIndex = triangles.size();

      //  Put all the triangles
      triangles.resize( currentIndex + 1 );
      for ( int i = 0; i < facesOfPartition.size(); ++i )
      {
        Face f = face( facesOfPartition[i] );

        Face::TriangleDecomposition t1 = f.decompTriangle(0);
        D3DXVECTOR3 normal = DxMath::normalOfTriangle( vertex( t1.v1 ).pos, vertex( t1.v2 ).pos, vertex( t1.v3 ).pos );

        VertexPosNor v;

        for (int k = 0; k < f.nDecompositionTriangles() ; ++k)
          {
          Face::TriangleDecomposition triangle = f.decompTriangle(k);
          D3DXVECTOR3 p1 = vertex( f.vId( triangle.v1 )).pos;
          D3DXVECTOR3 p2 = vertex( f.vId( triangle.v2 )).pos;
          D3DXVECTOR3 p3 = vertex( f.vId( triangle.v3 )).pos;

          v.pos = p1;
          v.nor = normal;
          triangles[currentIndex].push_back( v );
          v.pos = p2;
          v.nor = normal;
          triangles[currentIndex].push_back( v );
          v.pos = p3;
          v.nor = normal;
          triangles[currentIndex].push_back( v );
          }
      }
      //  Store the shader
      shaders.resize( currentIndex + 1);
      shaders[currentIndex] = new Shader();

      ShaderClassData data = ShaderHelpers::basicShader();


      wchar_t pathShader[2048];
      wcscpy_s ( pathShader, aTextures[j].mPath );

      data.mFileName = (LPCWSTR)&pathShader;
      //data.mFileName = &data.mFileName;
      shaders[currentIndex]->Init( &data, aDev );
      shaders[currentIndex]->Compile();
    }
  }

  EditableMeshModelProcTex* meshDx = new EditableMeshModelProcTex( triangles, shaders, aDev);

  return meshDx;
}


void MeshComponent::selectVertex(int aVertexId)
{
  mSelV[aVertexId] = true;
}

MeshComponent::MeshComponent( int id )
{
  mHighlight.idEntity = -1;
  mHighlight.idComponent = -1;

  mComponentId = id;
  mMesh = new HE_mesh();  
}

MeshComponent::MeshComponent(  )
{
  mHighlight.idEntity = -1;
  mHighlight.idComponent = -1;
  mMesh = new HE_mesh();
}


//  TODO: problematic??
bool MeshComponent::isEntitySelected(int aEntityId)
{
  bool ret;
  if ( mMode == ObjEd::eModeVertex )
    ret =  mSelV[aEntityId];
  if ( mMode == ObjEd::eModeEdge )
    ret =  mSelE[aEntityId];
  if ( mMode == ObjEd::eModeFace )
    ret =  mSelF[aEntityId];
  if ( mMode == ObjEd::eModeObject )
    ret = (aEntityId == mComponentId );

  return ret;
}

bool MeshComponent::isEntitySelected( const ObjEd::Selection &aEntity)
{
  bool ret;
  if ( aEntity.typeEntity == ObjEd::eModeVertex )
    ret =  mSelV[ aEntity.idEntity ];
  if ( aEntity.typeEntity == ObjEd::eModeEdge )
    ret =  mSelE[ aEntity.idEntity ];
  if ( aEntity.typeEntity == ObjEd::eModeFace )
    ret =  mSelF[ aEntity.idEntity ];
  if ( aEntity.typeEntity == ObjEd::eModeObject )
    ret = (aEntity.idComponent == mComponentId );

  return ret;
}

void MeshComponent::opTranslateVertex(const int aId, D3DXVECTOR3 aValue)
{
  VertexObj v = vertex(aId);
  v.pos = v.pos + aValue;
  modifyVertex( aId, v );
}

void MeshComponent::opScaleVertices(const int aId, D3DXVECTOR3 aCenter, D3DXVECTOR3 aValue)
{

  VertexObj v = vertex(aId);
  v.pos -= aCenter;
  v.pos.x = v.pos.x * aValue.x;
  v.pos.y = v.pos.y * aValue.y;
  v.pos.z = v.pos.z * aValue.z;
  v.pos += aCenter;
  modifyVertex( aId, v );
}

void MeshComponent::unselectVertices()
{
  for ( int i = 0; i < mSelV.size(); ++i )
    mSelV[i] = false;
}

bool MeshComponent::selectRemoveVertex(int aVertexId)
{
  mSelV[aVertexId] = false;
  bool atLeastOneSelected = false;
  for ( int i = 0; i < mSelV.size(); ++i )
    atLeastOneSelected |= mSelV[i];

  return atLeastOneSelected;
}

D3DXVECTOR3 MeshComponent::centerMassSelectedVertices()
{
  D3DXVECTOR3 ret = D3DXVECTOR3(0,0,0);
  int n = 0;
  for ( int i = 0; i < mSelV.size(); ++i )
    if( mSelV[i])
    {
      VertexObj v = vertex(i);
      ret += v.pos;
      ++n;
    }
  ret *= (1.0f/float(n));
  return ret;
}

void MeshComponent::unselectEdges()
{
  for ( int i = 0; i < mSelE.size(); ++i )
    mSelE[i] = false;
}

void MeshComponent::selectEdge( int aId )
{
  mSelE[aId] = true;
}

bool MeshComponent::selectRemoveEdge(int aId)
{
  mSelE[aId] = false;
  bool atLeastOneSelected = false;
  for ( int i = 0; i < mSelE.size(); ++i )
    atLeastOneSelected |= mSelE[i];

  return atLeastOneSelected;
}

void MeshComponent::modifyVertex(const int aId, const VertexObj &v)
{
  mMesh->modifyVertex(aId, v);
}


void MeshComponent::unselectFaces()
{
  for ( unsigned int i = 0; i < mSelF.size(); ++i )
    mSelF[i] = false;
}

void MeshComponent::selectFace(int aId)
{
  mSelF[aId] = true;
}

bool MeshComponent::selectRemoveFace(int aId)
{
  mSelF[aId] = false;
  bool atLeastOneSelected = false;
  for ( unsigned int i = 0; i < mSelF.size(); ++i )
    atLeastOneSelected |= mSelF[i];

  return atLeastOneSelected;
}

std::vector<ObjEd::Selection> MeshComponent::getCurrentSelection() const
{
  std::vector<ObjEd::Selection> sel;

  switch( mMode )
  {
  case eModeVertex:
    for ( unsigned int i = 0; i < mSelV.size(); ++i)
      if (mSelV[i])
      {
        ObjEd::Selection s;
        s.idComponent = mComponentId;
        s.typeEntity = eModeVertex;
        s.idEntity = i;
        sel.push_back( s );
      }
    break;
  case eModeEdge:
    for ( unsigned int i = 0; i < mSelE.size(); ++i)
      if (mSelE[i])
      {
        ObjEd::Selection s;
        s.idComponent = mComponentId;
        s.typeEntity = eModeEdge;
        s.idEntity = i;
        sel.push_back( s );
      }
    break;
  case eModeFace:
    for ( unsigned int i = 0; i < mSelF.size(); ++i)
      if (mSelF[i])
      {
        ObjEd::Selection s;
        s.idComponent = mComponentId;
        s.typeEntity = eModeFace;
        s.idEntity = i;
        sel.push_back( s );
      }
    break;
  }

  return sel;
}

D3DXVECTOR3 MeshComponent::centerMassSelectedEdges()
{
  D3DXVECTOR3 ret = D3DXVECTOR3(0,0,0);
  int n = 0;
  for ( int i = 0; i < mSelV.size(); ++i )
    if( mSelV[i])
    {
      VertexObj v = vertex(i);
      ret += v.pos;
      ++n;
    }
  ret *= (1.0f/float(n));
  return ret;
}

D3DXVECTOR3 MeshComponent::centerMassSelectedFaces()
{
  D3DXVECTOR3 ret = D3DXVECTOR3(0,0,0);
  int n = 0;
  for ( int i = 0; i < mSelV.size(); ++i )
    if( mSelV[i])
    {
      VertexObj v = vertex(i);
      ret += v.pos;
      ++n;
    }
  ret *= (1.0f/float(n));
  return ret;
}

D3DXVECTOR3 MeshComponent::center(Face &aF) const
{
  D3DXVECTOR3 res = D3DXVECTOR3(0,0,0);
  for (unsigned int i = 0; i < aF.nVertices(); ++i)
  {
    res +=  vertex(aF.vId(i)) .pos;
  }
  res *= 1.0f/float( aF.nVertices());

  return res;
}


FacesOfEdge MeshComponent::facesOfEdge(int aId) const
{
  FacesOfEdge f;

  HE_edge* edgep = mMesh->hedge(aId);

  //TODO: is left-right ordering correct?
  f.fl = edgep->mFace->mId;
  f.fr = edgep->mPair->mFace->mId;

  return f;
}

bool MeshComponent::faceContainsSelectedEntity( const int aId, const ObjEd::Selection &aSel) const
{
  bool contained = false;
  HE_face* face = mMesh->hface( aId );
  if ( aSel.typeEntity == eModeVertex )
  {
    std::vector<HE_vert*> v = face->vertices();
    for ( int i = 0; i < v.size(); ++i )
      if ( v[i]->mId == aSel.idEntity )
        contained = true;
  }
  if ( aSel.typeEntity == eModeEdge )
  {
    Face f = this->face( aId );
    for ( int i = 0; i < f.nEdges(); ++i )
      if ( f.eId(i) == aSel.idEntity )
        contained = true;
  }
  return contained;
}

//  \param    aFaceId   one face
//  \param    aEdgeId   one of its edges
//  \returns  HE_edge*  half edge contained in the face
HE_edge * MeshComponent::halfEdgeOfFace(int aFaceId, int aEdgeId)
{
  HE_face* f = mMesh->hface( aFaceId );
  HE_edge* e = mMesh->hedge( aEdgeId );

  bool found = false;
  std::vector<HE_edge*> edges = f->edges();
  for ( int i = 0; i < edges.size(); ++i )
    if ( edges[i] == e )
      found = true;

  if (!found)
    e = e->mPair;

  return e;
}

void MeshComponent::resizeSelectionVectors()
{
  mSelV.resize( mMesh->nVertices() );
  mSelF.resize( mMesh->nFaces() );
  mSelE.resize( mMesh->nFullEdges() );

  /*
  unselectVertices();
  unselectEdges();
  unselectFaces();
*/

  resizeTexIdSelection();
}

void MeshComponent::checkIntegrity( )
{
  mMesh->checkIntegrity();
}

std::vector<Face> MeshComponent::facesFacingCamera( D3DXVECTOR3 aCamPos )
{
  std::vector<Face> faces;
  for ( int i = 0; i < nFaces(); ++i )
  {
    Face f = face(i);
    bool frontFacing = faceVisible( f, aCamPos );
    if (frontFacing)
      faces.push_back( f );
  }
  return faces;
}

std::vector<int> MeshComponent::edgesOfFacesFacingCamera( D3DXVECTOR3 aCamPos )
{
  std::vector<Face> faces = facesFacingCamera( aCamPos );
  std::list<int> edges;
  for ( int i = 0; i < faces.size(); ++i )
  {
    for ( int j = 0; j < faces[i].nEdges() ; ++j )
      edges.push_back( faces[i].eId(j) );
  }
  edges.sort();
  edges.unique();

  std::vector<int> uniqueEdges( edges.begin(), edges.end() );

  return uniqueEdges;
}

bool MeshComponent::faceVisible(const Face &f, const D3DXVECTOR3 aCamPos )
{
  bool facing = true;
  for (int k = 0; k < f.nDecompositionTriangles() ; ++k)
  {
    Face::TriangleDecomposition triangle = f.decompTriangle(k);
    D3DXVECTOR3 p1 = vertex( f.vId( triangle.v1 ) ).pos;
    D3DXVECTOR3 p2 = vertex( f.vId( triangle.v2 ) ).pos;
    D3DXVECTOR3 p3 = vertex( f.vId( triangle.v3 ) ).pos;
    if ( ! Intersections::areCollinear(p1,p2,p3) )
      facing &= Intersections::frontFacing(p1,p2,p3, aCamPos );
  }
  return facing;
}

std::vector<int> MeshComponent::DEBUGlistEdge(ObjEd::Selection sel)
{
  HE_edge* e = mMesh->hedge( sel.idEntity );

  std::vector<int> v;
  v.push_back( e->mId );
  v.push_back( e->mPair->mId );

  return v;
}

std::vector<int> MeshComponent::DEBUGlistFace(ObjEd::Selection sel)
{
  std::vector<int> v;
  Face f = mMesh->face( sel.idEntity );
  for( int i =0 ; i< f.nVertices(); ++i)
    v.push_back( f.vId(i));

  HE_face* fa = mMesh->hface( sel.idEntity );
  std::vector<HE_edge*> e = fa->edges();
  for( int i =0 ; i< e.size(); ++i)
  {
    v.push_back( e[i]->mId );
  }

  return v;
}

void MeshComponent::setId(int id)
{
  mComponentId = id;
}

Intersections::BoundingSphere MeshComponent::boundingSphere() const
{
  Intersections::BoundingSphere b;

  b.center = D3DXVECTOR3( 0,0,0 );
  for( int i =0 ; i< nVertices(); ++i)
  {
    b.center += vertex(i).pos;
  }
  b.center *= ( 1.0f / float(nVertices()) );

  float maxDistance = 0.0f;
  for( int i =0 ; i< nVertices(); ++i)
  {
    D3DXVECTOR3 v = b.center - vertex(i).pos;
    float distance = D3DXVec3Length( &v );
    if ( distance > maxDistance )
      maxDistance = distance;
  }
  b.radius = maxDistance;

  return b;
}

std::vector<D3DXVECTOR3> MeshComponent::basisOfEdge(const int edgeId) const
{
  std::vector<D3DXVECTOR3> basis(3);

  FacesOfEdge faces = facesOfEdge( edgeId );
  Face f1 = face( faces.fl );
  Face::TriangleDecomposition t1 = f1.decompTriangle( 0 );
  Face f2 = face( faces.fr );
  Face::TriangleDecomposition t2 = f2.decompTriangle( 0 );
  Edge e = edge( edgeId );

  basis[0] = DxMath::normalOfTriangle( vertex( f1.vId(t1.v1 )).pos,
                                       vertex( f1.vId(t1.v2 )).pos,
                                       vertex( f1.vId(t1.v3 )).pos )
           + DxMath::normalOfTriangle( vertex( f2.vId(t2.v1 )).pos,
                                       vertex( f2.vId(t2.v2 )).pos,
                                       vertex( f2.vId(t2.v3 )).pos );
  basis[0] *= -0.5f;
  basis[1] = DxMath::unitVector( vertex( e.bId ).pos, vertex( e.eId ).pos );
  basis[2] = DxMath::thirdVectorBasis ( basis[0], basis[1] );

  return basis;
}

std::vector<D3DXVECTOR3> MeshComponent::basisOfFace(const int faceId) const
{
  std::vector<D3DXVECTOR3> basis(3);
  Face f = face( faceId );
  Face::TriangleDecomposition t = f.decompTriangle( 0 );

  basis[0] = DxMath::normalOfTriangle( vertex( f.vId(t.v1) ).pos,
                                       vertex( f.vId(t.v2) ).pos,
                                       vertex( f.vId(t.v3) ).pos );
  basis[0] *= -1.0f;
  basis[1] = DxMath::unitVector( vertex( f.vId(t.v1) ).pos,
                                 vertex( f.vId(t.v2) ).pos );
  basis[2] = DxMath::thirdVectorBasis ( basis[0], basis[1] );

  return basis;
}

std::vector<int> MeshComponent::uniqueVerticesOfFaces(std::vector<Face> &aFaces)
{
  std::list<int> vertices;
  for ( unsigned int i = 0; i < aFaces.size() ; ++i)
  {

    for ( int j = 0; j < aFaces[i].nVertices() ; ++j)
      vertices.push_back( aFaces[i].vId(j) );
  }

  vertices.sort();
  vertices.unique();

  std::vector<int> verticesVector ( vertices.begin(), vertices.end() );

  return verticesVector;
}

void MeshComponent::initTex()
{
  resizeTexIdSelection();
}

void MeshComponent::setTex(const int aFaceId, const int aTexId)
{
  resizeTexIdSelection();
  mTextureIdPerFace[ aFaceId ] = aTexId;
}

void MeshComponent::resizeTexIdSelection()
{
  int size = mTextureIdPerFace.size();
  if ( size != nFaces() )
  {
    mTextureIdPerFace.resize( nFaces() );
    for ( int i = size; i < nFaces(); ++i )
      mTextureIdPerFace[i] = 0;
  }

}
////////////////////// IO - SPLIT ME! - TODO
#ifdef _MESH_QT_IO

  void MeshComponent::saveTexture(QDataStream &outs)
  {
    outs << (int) mTextureIdPerFace.size();
    for ( int i = 0; i < mTextureIdPerFace.size(); ++i )
      outs << mTextureIdPerFace[i];
  }

  void MeshComponent::loadTexture(QDataStream &ins)
  {
    int size;
    ins >> size;
    mTextureIdPerFace.resize( size );
    for ( int i = 0; i < mTextureIdPerFace.size(); ++i )
      ins >> mTextureIdPerFace[i];
  }

  void MeshComponent::saveSelection(QDataStream &outs)
  {
    outs << (int) mSelF.size();
    for ( int i = 0; i < mSelF.size(); ++i )
      outs << mSelF[i];
    outs << (int) mSelE.size();
    for ( int i = 0; i < mSelE.size(); ++i )
      outs << mSelE[i];
    outs << (int) mSelV.size();
    for ( int i = 0; i < mSelV.size(); ++i )
      outs << mSelV[i];
  }

  void MeshComponent::loadSelection(QDataStream &ins)
  {
    int size;
    bool val;

    ins >> size;
    mSelF.resize( size );
    for ( int i = 0; i < mSelF.size(); ++i )
    {
      ins >> val;
      mSelF[i] = val;
    }

    ins >> size;
    mSelE.resize( size );
    for ( int i = 0; i < mSelE.size(); ++i )
    {
      ins >> val;
      mSelE[i] = val;
    }

    ins >> size;
    mSelV.resize( size );
    for ( int i = 0; i < mSelV.size(); ++i )
    {
      ins >> val;
      mSelV[i] = val;
    }

  }
#endif

#ifdef _MESH_CPP_IO
  void MeshComponent::saveTexture(std::ofstream &outs)
  {
    outs << (int) mTextureIdPerFace.size() << endl;
    for ( int i = 0; i < mTextureIdPerFace.size(); ++i )
      outs << mTextureIdPerFace[i] << endl;
  }

  void MeshComponent::loadTexture(std::fstream &ins)
  {
    int size;
    ins >> size;
    mTextureIdPerFace.resize( size );
    for ( int i = 0; i < mTextureIdPerFace.size(); ++i )
      ins >> mTextureIdPerFace[i];
  }

#endif

std::vector<int> MeshComponent::facesOfEntity( const ObjEd::Selection& aSel ) const
{
  std::vector<int> faces;
  if ( aSel.typeEntity == eModeVertex )
  {
    HE_vert* v = mMesh->hvert( aSel.idEntity );
    std::vector<HE_face*> f = v->faces();
    for ( int i = 0; i < f.size(); ++i )
      faces.push_back( f[i]->mId );
  }
  if ( aSel.typeEntity == eModeEdge )
  {
    faces = facesOfEdgeList( aSel.idEntity );
  }
  return faces;
}


std::vector<int> MeshComponent::facesOfEdgeList( const int aId ) const
{
  std::vector<int> faces;

  HE_edge* e = mMesh->hedge( aId );
  std::vector<HE_face*> f = e->faces();
  for ( int i = 0; i < f.size(); ++i )
    faces.push_back( f[i]->mId );

  return faces;
}

