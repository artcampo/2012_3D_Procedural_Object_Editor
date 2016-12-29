#include "MeshHalfEdge.hpp"
#include "ObjEditor/Mesh/Serialization/MeshHalfEdgeSerial.hpp"
#include "ObjEditor/Mesh/HalfEdge/HE_edge.hpp"
#include "ObjEditor/Mesh/HalfEdge/HE_vert.hpp"
#include "ObjEditor/Mesh/HalfEdge/HE_face.hpp"

HE_mesh::~HE_mesh()
{
  for ( int i = 0; i < mHalfEdges.size() ; ++i )
    delete mHalfEdges[i];

  for ( int i = 0; i < mFaces.size() ; ++i )
    delete mFaces[i];

  for ( int i = 0; i < mVertices.size() ; ++i )
    delete mVertices[i];

  mHalfEdges.clear();
  mFaces.clear();
  mVertices.clear();

  mFullEdgeofHalfEdge.clear();
  mHalfEdgeOfFullEdge.clear();
}

HE_mesh::HE_mesh(  const HE_meshSerial& rhs  )
{
  mHalfEdges.resize( rhs.mHalfEdges.size() );
  for ( int i = 0; i < rhs.mHalfEdges.size() ; ++i)
  {
    mHalfEdges[i] = new HE_edge( rhs.mHalfEdges[i] );
  }

  mFaces.resize( rhs.mFaces.size() );
  for ( int i = 0; i < rhs.mFaces.size() ; ++i)
  {
    mFaces[i] = new HE_face( rhs.mFaces[i] );
  }

  mVertices.resize( rhs.mVertices.size() );
  for ( int i = 0; i < rhs.mVertices.size() ; ++i)
  {
    mVertices[i] = new HE_vert( rhs.mVertices[i] );
  }

  mHalfEdgeOfFullEdge.resize( rhs.mHalfEdgeOfFullEdge.size() );
  for ( int i = 0; i < rhs.mHalfEdgeOfFullEdge.size() ; ++i)
    mHalfEdgeOfFullEdge[i] = (HE_edge*)rhs.mHalfEdgeOfFullEdge[i];

  mFullEdgeofHalfEdge.resize( rhs.mFullEdgeofHalfEdge.size() );
  for ( int i = 0; i < rhs.mFullEdgeofHalfEdge.size() ; ++i)
    mFullEdgeofHalfEdge[i] = rhs.mFullEdgeofHalfEdge[i];

  for (int i = 0; i < mHalfEdgeOfFullEdge.size(); ++i)
    mHalfEdgeOfFullEdge[i] = unserialIde(  mHalfEdgeOfFullEdge[i] );

  for ( int i = 0; i < mHalfEdges.size() ; ++i)
  {
    mHalfEdges[i]->mVert = unserialIdv( mHalfEdges[i]->mVert );
    mHalfEdges[i]->mFace = unserialIdf( mHalfEdges[i]->mFace );
    mHalfEdges[i]->mPair = unserialIde( mHalfEdges[i]->mPair );
    mHalfEdges[i]->mNext = unserialIde( mHalfEdges[i]->mNext );
    mHalfEdges[i]->mPrev = unserialIde( mHalfEdges[i]->mPrev );
  }

  for ( int i = 0; i < mFaces.size() ; ++i)
    mFaces[i]->mEdge = unserialIde( mFaces[i]->mEdge );

  for ( int i = 0; i < mVertices.size() ; ++i)
    mVertices[i]->mEdge = unserialIde( mVertices[i]->mEdge );
}


HE_vert* HE_mesh::newVertex(const VertexObj &av)
{
  int index  = mVertices.size();
  mVertices.resize ( index + 1 );
  mVertices[index] = new HE_vert(av, index);
  return mVertices[ index ];
}

HE_edge * HE_mesh::newHalfEdge( HE_vert* v, HE_face* e )
{
  int index  = mHalfEdges.size();
  mHalfEdges.resize ( index + 1 );
  mHalfEdges[index] = new HE_edge(v, e, index );
  mFullEdgeofHalfEdge.resize( mHalfEdges.size() );
  return mHalfEdges[ index ];
}

HE_face * HE_mesh::newFace()
{
  int index  = mFaces.size();
  mFaces.resize ( index + 1 );
  mFaces[index] = new HE_face( index );
  return mFaces[ index ];
}

VertexObj HE_mesh::vertex(int id) const
{
  return mVertices[id]->mVertex;
}

Face HE_mesh::face(int id) const
{
  std::vector<HE_vert*> v = mFaces[id]->vertices();
  std::vector<HE_edge*> e = mFaces[id]->edges();

  std::vector<int> vert (v.size());
  for ( unsigned int i = 0; i < v.size() ; ++i)
    vert[i] = v[i]->mId;

  std::vector<int> edges (e.size());
  for ( unsigned int i = 0; i < e.size() ; ++i)
    edges[i] = mFullEdgeofHalfEdge[ e[i]->mId ];

  Face f( vert, edges );
  return f;
}

void HE_mesh::modifyVertex(const int aId, const VertexObj &v)
{
  mVertices[aId]->modifyVertex( v );
}

HE_face * HE_mesh::hface(int id) const
{
  return mFaces[id];
}

int HE_mesh::nVertices() const
{
  return mVertices.size();
}

int HE_mesh::nFaces() const
{
  return mFaces.size();
}

HE_vert * HE_mesh::hvert(int id) const
{
  return mVertices[id];
}

HE_edge*    HE_mesh::hedge ( int id ) const
{
  return mHalfEdgeOfFullEdge[id];
}






HE_vert* HE_mesh::serialIdv (HE_vert* v) const
{
  for ( int i = 0; i < mVertices.size() ; ++i)
    if ( v == mVertices[i] )
      return (HE_vert*)i;
}

HE_face* HE_mesh::serialIdf (HE_face* v) const
{
  for ( int i = 0; i < mFaces.size() ; ++i)
    if ( v == mFaces[i] )
      return (HE_face*)i;
}

HE_edge* HE_mesh::serialIde (HE_edge* v) const
{
  for ( int i = 0; i < mHalfEdges.size() ; ++i)
    if ( v == mHalfEdges[i] )
      return (HE_edge*)i;
}

HE_vert* HE_mesh::unserialIdv (HE_vert* v) const
{
  return mVertices[int(v)];
}

HE_face* HE_mesh::unserialIdf (HE_face* v) const
{
  return mFaces[int(v)];
}

HE_edge* HE_mesh::unserialIde (HE_edge* v) const
{
  return mHalfEdges[int(v)];
}

int HE_mesh::nHalfEdges() const
{
  return mHalfEdges.size();
}

int HE_mesh::nFullEdges() const
{
  return mHalfEdgeOfFullEdge.size();
}


bool HE_mesh::faceContainsHalfEdge(const HE_face* aFace, const HE_edge* aEdge) const
{
  bool contained = false;
  std::vector<HE_edge*> e = aFace->edges();
  for ( int i = 0; i < e.size(); ++i )
    if ( e[i] == aEdge )
      contained = true;
  return contained;
}

HE_vert * HE_mesh::splitEdge(HE_edge *aSplitEdge, const VertexObj& av )
{
  int fullEdgeId = mFullEdgeofHalfEdge[ aSplitEdge->mId ];
  HE_edge* e1 = aSplitEdge;
  HE_edge* e2 = aSplitEdge->mPair;

  HE_face* fa = e2->mFace;
  HE_face* fb = e1->mFace;

  HE_edge* e1n = e1->mNext;
  HE_edge* e1p = e1->mPrev;
  HE_edge* e2n = e2->mNext;
  HE_edge* e2p = e2->mPrev;

  HE_vert* v3 = newVertex( av );
  HE_edge* e3 = newHalfEdge( v3, fb );
  HE_edge* e4 = newHalfEdge( v3, fa );

  e1p->next( e3 );
  e3->next( e1 );
  e1->next( e1n );

  e2p->next( e4 );
  e4->next( e2 );
  e2->next( e2n );

  e1->pair( e4 );
  e3->pair( e2 );

  newFullEdge( e2, e3 );
  recycleFullEdge ( fullEdgeId, e1, e4 );

  return v3;
}

//  TODO: should new edge inmediatly call e1->pair( e2) ?
void HE_mesh::newFullEdge( const HE_edge* e1, const HE_edge* e2 )
{
  int index = mHalfEdgeOfFullEdge.size();
  mHalfEdgeOfFullEdge.push_back( const_cast<HE_edge*> (e1) );

  mFullEdgeofHalfEdge.resize( nHalfEdges() );
  mFullEdgeofHalfEdge[ e1->mId ] = index;
  mFullEdgeofHalfEdge[ e2->mId ] = index;
}

void HE_mesh::recycleFullEdge ( const int aId, const HE_edge* e1, const HE_edge* e2 )
{
  mHalfEdgeOfFullEdge[aId] = const_cast<HE_edge*> (e1);
  mFullEdgeofHalfEdge[ e1->mId ] = aId;
  mFullEdgeofHalfEdge[ e2->mId ] = aId;
}

Edge HE_mesh::fullEdge(int id) const
{
  HE_edge* edgep = mHalfEdgeOfFullEdge[id];
  int b = edgep->mVert->mId;
  int e = edgep->mPair->mVert->mId;
  Edge edge( b, e );

  return edge;
}

HE_face * HE_mesh::sharedFace(HE_vert *aVb, HE_vert *aVe) const
{
  std::vector<HE_face *> f1 = aVb->faces();
  std::vector<HE_face *> f2 = aVe->faces();

  std::sort( f1.begin(), f1.end() );
  std::sort( f2.begin(), f2.end() );

  std::vector<HE_face *>::iterator intersection =
  std::set_intersection(  f1.begin(), f1.end(),
                          f2.begin(), f2.end(),
                          f2.begin()    );

  return *(intersection - 1);
}


HE_face * HE_mesh::sharedFace( HE_edge* aEb, HE_vert* aVe ) const
{
  std::vector<HE_face *> f1 = aEb->faces();
  std::vector<HE_face *> f2 = aVe->faces();

  std::sort( f1.begin(), f1.end() );
  std::sort( f2.begin(), f2.end() );

  std::vector<HE_face *>::iterator intersection =
  std::set_intersection(  f1.begin(), f1.end(),
                          f2.begin(), f2.end(),
                          f2.begin()    );

  return *(intersection - 1);
}

HE_face * HE_mesh::sharedFace( HE_edge* aEb, HE_edge* aEe ) const
{
  //TODOCOMP
  std::vector<HE_face *> f1 = aEb->faces();
  std::vector<HE_face *> f2 = aEe->faces();

  std::sort( f1.begin(), f1.end() );
  std::sort( f2.begin(), f2.end() );

  std::vector<HE_face *>::iterator intersection =
  std::set_intersection(  f1.begin(), f1.end(),
                          f2.begin(), f2.end(),
                          f2.begin()    );

  return *(intersection - 1);
}

HE_face * HE_mesh::sharedFace( HE_vert* aVb, HE_edge* aEe ) const
{
  return sharedFace( aEe, aVb );
}

HE_vert * HE_mesh::splitFace(HE_face *aFace, HE_vert *aVb, HE_vert *aVe)
{
  std::vector<HE_edge*> chain1 = aFace->edgeChain( aVb, aVe );
  std::vector<HE_edge*> chain2 = aFace->edgeChain( aVe, aVb );

  HE_edge* e1n = chain1[0];
  HE_edge* e2n = chain2[0];

  HE_edge* e1p = e1n->mPrev;
  HE_edge* e2p = e2n->mPrev;
  /*
  std::vector<HE_edge*> side1 = aFace->edges( aVe, aVb );

  HE_edge* e1p = side1[ side1.size() - 1 ];
  HE_edge* e1n = e1p->mNext;

  HE_edge* e2n = side1[0];
  HE_edge* e2p = e2n->mPrev;
*/
  HE_face* f2 = newFace();

  HE_edge* e1 = newHalfEdge( aVb, aFace );
  HE_edge* e2 = newHalfEdge( aVe, f2 );

  e1->next( e1n );
  e2p->next( e1 );
  e2->next( e2n );
  e1p->next( e2 );

  e1->pair( e2 );
  newFullEdge( e1, e2 );

  for ( int i = 0; i < chain2.size() ; ++i )
    chain2[i]->mFace = f2;

  return aVe;
}

void HE_mesh::checkIntegrity()
{
  int b = 0;
  int c = 0;
  for ( int i = 0; i < nHalfEdges() ; ++i )
  {
    HE_edge* edgeInitial = mHalfEdges[i];
    HE_edge* e = edgeInitial;
    HE_face* face = e->mFace;

    bool sameFace = true;
    bool allEdgesVisited = false;
    for ( int j = 0; j < nHalfEdges() && !allEdgesVisited ; ++j )
    {
      e = e->mNext;
      if (e->mFace != face)
        sameFace = false;
      if (e == edgeInitial)
        allEdgesVisited = true;

    }

    if ( !allEdgesVisited )
    {
      ++b;  // unclosed face
    }

    if( !sameFace )
    {
      ++c;  //  face not correctly updated
    }
  }

  int a = c;
  a +=b;
}

std::vector<int> HE_mesh::verticesOfEdgesOfCorner( const int vbId, const int veId ) const
{
  std::vector<HE_edge *> e = edgePairOfVerticesPair( hvert(vbId ), hvert(veId ) );

  std::vector<int> vertices;
  vertices.push_back(  e[0]->mNext->mVert->mId );
  vertices.push_back(  e[1]->mPrev->mPrev->mVert->mId  );

  return vertices;
}

//  \param      veId, id of one vertex of this fullEdge
//  \param      veId, id of one vertex of this fullEdge
//  \returns    vertices of new face: [0] -> vId, [1]-> other end of edge
//              [2][3], their copies
std::vector<HE_vert *> HE_mesh::bevelSingleEdgeHIGH(const int vbId, const int veId )
{
  std::vector<HE_edge *> initialHedges = edgePairOfVerticesPair( hvert(vbId ), hvert(veId ) );



  //  Set
  HE_edge* ea = initialHedges[0];
  HE_edge* eb = initialHedges[1];
  int eId = mFullEdgeofHalfEdge[ ea->mId ];

  HE_edge* ebp = eb->mPrev;
  HE_edge* es1p = ea->mNext->mPair;
  HE_edge* es1n = es1p->mNext;
  HE_edge* es2p = eb->mNext->mPair;
  HE_edge* es2n = es2p->mNext;

  HE_vert* v1 = ea->mVert;
  HE_vert* v2 = eb->mVert;

  HE_face* fs1 = es1p->mFace;
  HE_face* fs2 = es2p->mFace;

  //  Create
  HE_vert* vc1 = newVertex( v1->mVertex );
  HE_vert* vc2 = newVertex( v2->mVertex );

  HE_face* fc = newFace();

  HE_edge* ec1 = newHalfEdge( v2 , fc );
  HE_edge* ec2 = newHalfEdge( vc2 , fc );
  HE_edge* ec3 = newHalfEdge( vc1 , fc );
  HE_edge* ec4 = newHalfEdge( v1 , fc );

  HE_edge* es1c = newHalfEdge( vc1 , fs1 );
  HE_edge* es2c = newHalfEdge( v2 , fs2 );

  ec1->next( ec2 );
  ec2->next( ec3 );
  ec3->next( ec4 );
  ec4->next( ec1 );

  es1p->next( es1c );
  es1c->next( es1n );
  es2p->next( es2c );
  es2c->next( es2n );

  ec1->pair( ea );
  ec2->pair( es2c );
  ec3->pair( eb );
  ec4->pair( es1c );

  es2p->mVert = vc2;
  eb->mVert = vc2;
  ebp->mVert = vc1;

  newFullEdge( ec2, es2c );
  newFullEdge( ec3, eb );
  newFullEdge( ec4, es1c );
  recycleFullEdge( eId, ea, ec1 );

  checkIntegrity();

  std::vector<HE_vert *> v;
  v.push_back( v1 );
  v.push_back( v2 );
  v.push_back( vc1 );
  v.push_back( vc2 );

  return v;
}

void HE_mesh::bevelESingleCornerE(
    HE_vert*  v,
    HE_vert*  vprev,
    HE_vert** vc,
    HE_edge** el,
    HE_edge** er,
    HE_edge** eln,
    HE_edge** ern,
    D3DXVECTOR3* ein)
{
  std::vector<HE_edge *> pair = edgePairOfVerticesPair( v, vprev );
  HE_edge* edgeVVprev = pair[0];

  //  Locate in and out edges
  std::vector<HE_edge *> incidentEdges = v->edges();
  bool found = false;
  int index = 0;
  while (! found )
  {
    if ( incidentEdges[index] == edgeVVprev )
      found = true;
    else
      ++index;
  }
  HE_edge* e = edgeVVprev;
  HE_edge* e1 = incidentEdges[ (index + 1) % incidentEdges.size() ];
  HE_edge* e2 = incidentEdges[ (index + 2) % incidentEdges.size() ];
  HE_edge* e1p = e1->mPair;
  HE_edge* e2p = e2->mPair;
  HE_edge* ep = e->mPair;
  HE_face* f = e2->mFace;
  HE_face* fb = e1->mFace;

  *vc   = newVertex( v->mVertex );
  *er = newHalfEdge( *vc, f );
  *el = newHalfEdge( v, fb );

  (*el)->pair( *er );
  newFullEdge( *el, *er );

  e2->next( *er );
  (*er)->next( e1p );
  (*el)->next( ep );
  e1->next( *el );

  e1->mVert = *vc;

  *eln = e1;
  *ern = e1p;  
  *ein = e2p->mVert->mVertex.pos - (*vc)->mVertex.pos;

/*
  //  Extend vertex
  *vc   = newVertex( v->mVertex );
  HE_edge* er = newHalfEdge( *vc, f );
  *el = bevelEnewHalfEdge( v );

  (*el)->pair( er );
  newFullEdge( *el, er );

  e2->next( er );
  er->next( e1p );
  (*el)->next( ep );

  e1->mVert = *vc;
  *(eln) = e1p;
  */
}

void HE_mesh::bevelESingleCornerB(
    HE_vert*  v,
    HE_vert*  vprev,
    HE_vert** vc,
    HE_edge** el,
    HE_edge** er,
    HE_edge** eln,
    HE_edge** ern,
    D3DXVECTOR3* ein)
{
  std::vector<HE_edge *> pair = edgePairOfVerticesPair( v, vprev );
  HE_edge* edgeVVprev = pair[0];

  //  Locate in and out edges
  std::vector<HE_edge *> incidentEdges = v->edges();
  bool found = false;
  int index = 0;
  while (! found )
  {
    if ( incidentEdges[index] == edgeVVprev )
      found = true;
    else
      ++index;
  }
  HE_edge* e = edgeVVprev;
  int i1 = (index + 1) % incidentEdges.size();
  int i2 = (index + 2) % incidentEdges.size();
  HE_edge* e1 = incidentEdges[ i1 ];
  HE_edge* e2 = incidentEdges[ i2 ];

  HE_edge* e1p = e1->mPair;
  HE_edge* ep = e->mPair;
  HE_edge* e2p = e2->mPair;
  HE_face* f = e2->mFace;
  HE_face* fbot = e->mFace;

  *vc   = newVertex( v->mVertex );
  *el = newHalfEdge( v, f );
  *er = newHalfEdge( *vc, fbot );

  (*er)->pair( *el );
  newFullEdge( *el, *er );

  e2->next( *el );
  (*el)->next( e1p );
  e->next( *er );
  (*er)->next( e2p);

  e2->mVert = *vc;

  *ern = e2p;
  *eln = e2;
  *ein = e1p->mVert->mVertex.pos - (*vc)->mVertex.pos;


/*
  //  Extend vertex
  *vc   = newVertex( v->mVertex );
  HE_edge* el = newHalfEdge( v, f );
  *er = bevelEnewHalfEdge( *vc );

  (*er)->pair( el );
  newFullEdge( el, *er );

  e2->next( el );
  el->next( e1p );
  e->next( *er );

  e2->mVert = *vc;
  *(ern) = e2p;
  */
}

void HE_mesh::bevelESharedCorner(
    HE_vert*  v,
    HE_edge*  e,
    HE_edge*  e2,
    HE_vert** vc,
    HE_edge** el,
    HE_edge** er,
    HE_edge** eln,
    HE_edge** ern )
{
  std::vector<HE_edge*> order = e->nonIncidentEdges();

  if ( order[0] == e2 || order[1] != e2)
  {
    bool fucked = true;   //reverse order
  }


  HE_edge* ep  = e->mPair;
  HE_edge* ep2 = e2->mPair;

  HE_face* fl = ep->mFace;
  HE_face* fr = ep2->mFace;

  (*eln) = ep->mPrev;
  (*ern) = ep2->mNext;

  *vc   = newVertex( v->mVertex );
  *er = newHalfEdge( *vc, fr );
  *el = newHalfEdge( v, fl );

  (*el)->pair( *er );
  newFullEdge( *el, *er );

  (*eln)->next( *el );
  (*el)->next ( ep );

  ep2->next( *er );
  (*er)->next( *ern);

  (*eln)->mVert = *vc;
  v->mEdge = e;

/*
  HE_edge* ep = e->mPair;
  HE_edge* ep2 = ep->mPair;

  (*eln)  = ep->mPrev->mPair;
  (*ern)  = (*eln)->mPair;  

  *vc   = newVertex( v->mVertex );
  *er = bevelEnewHalfEdge( *vc );
  *el = bevelEnewHalfEdge( v );

  (*el)->pair( *er );
  newFullEdge( *el, *er );

  ep2->next( *er );
  (*el)->next ( ep );

  (*eln)->mVert = *vc;
  *ein = ep;*/
}

void HE_mesh::bevelESingleEdge(
    HE_vert* v1,
    HE_vert* vc1,       
    HE_edge* er,
    HE_edge* ern,

    HE_edge* e1,

    HE_vert* v2,
    HE_vert* vc2,
    HE_edge* el,
    HE_edge* eln
    )
{
  //std::vector<HE_edge *> initialHedges = edgePairOfVerticesPair( hvert(vbId ), hvert(veId ) );

  //  gather initial structure
  int eId = mFullEdgeofHalfEdge[ e1->mId ];
  HE_edge* ep  = e1->mPair;
  HE_face* fbottom  = ep->mFace;

  //  Create new face
  HE_face* finner = newFace();
  HE_edge* eb     = newHalfEdge( vc2 , finner );
  HE_edge* ebp    = newHalfEdge( vc1 , fbottom );

  // Inner face
  ep->next( er );
  er->next( eb );
  eb->next( el );
  el->next( ep );

  // inner face edges
  el->mFace = finner;
  er->mFace = finner;
  ep->mFace = finner;

  //  bottom face
  eln->next( ebp );
  ebp->next( ern );

  ern->mFace = fbottom;
  eln->mFace = fbottom;

  fbottom->mEdge = ern;
  v1->mEdge   = ep;
  v2->mEdge   = el;
  vc1->mEdge  = ebp;
  vc2->mEdge  = eln;

  //  Create full edge
  eb->pair( ebp );
  newFullEdge( eb, ebp );

}

//  \return   e[0],e[1]:  such that vert(av1) = e[0], vert(av2) = e[1], pair(e[0])=e[1]
std::vector<HE_edge *> HE_mesh::edgePairOfVerticesPair(const HE_vert *aV1, const HE_vert *aV2) const
{
  std::vector<HE_edge *> pair;
  std::vector<HE_edge *> ei = aV1->edges();
  std::vector<HE_edge *> ej = aV2->edges();

  bool found = false;
  for ( int i = 0; !found && i < ei.size() ; ++i )
    for ( int j = 0; !found && j < ej.size() ; ++j )
    {
      if ( ei[i]->mPair == ej[j] )
      {
        pair.push_back( ei[i] );
        pair.push_back( ej[j] );
      }
    }
  return pair;
}

HE_edge * HE_mesh::bevelEnewHalfEdge( HE_vert* v )
{
  int index  = mHalfEdges.size();
  mHalfEdges.resize ( index + 1 );
  mHalfEdges[index] = new HE_edge(v, index );
  mFullEdgeofHalfEdge.resize( mHalfEdges.size() );
  return mHalfEdges[ index ];
}

HE_face* HE_mesh::bevelEexteriorFace( const HE_vert* v1, const HE_vert* v2 ) const
{
  std::vector<HE_edge*> initialHedges = edgePairOfVerticesPair( v1, v2 );

  //  Set
  HE_edge* ea = initialHedges[0];
  HE_edge* eb = initialHedges[1];

  HE_edge* es1p = ea->mNext->mPair;
  HE_edge* es2p = eb->mNext->mPair;

  HE_face* fs1 = es1p->mFace;
  HE_face* fs2 = es2p->mFace;

  return fs1;
}

//  gives the next edge to the exterior clockwise from e=(v1,v2)
HE_edge * HE_mesh::bevelEExteriorEdge(const HE_vert *v1, const HE_vert *v2) const
{
  std::vector<HE_edge *> pair = edgePairOfVerticesPair( v1, v2 );
  return pair[1]->mNext->mPair;
}

int HE_mesh::fullOfhalf(HE_edge *e)
{
  return mFullEdgeofHalfEdge[e->mId];
}

//  \param    id    id of edge
//  \param    v     vertex to which it is incident
HE_edge* HE_mesh::hedgeIncidentToVertex(int id, const HE_vert *v) const
{
  HE_edge* e = hedge( id );
  if ( e->mVert != v)
    e = e->mPair;
  return e;
}
