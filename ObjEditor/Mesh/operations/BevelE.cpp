#include "OBjEditor/Mesh/MeshComponent.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_face.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_edge.hpp"
#include "OBjEditor/Mesh/HalfEdge/HE_vert.hpp"

class VertexOfEdgeSearch{
public:
  int   vId;
  bool operator () ( const Edge &e )
    {
    if( e.bId == vId || e.eId == vId )
      {
      return true;
      }
    return false;
    }
};

//  \param  aId   id of face to extrude
//  \return info to extrude face
BevelEInfo MeshComponent::bevelEStart ( std::vector<ObjEd::Selection>& aSel )
{
  BevelEInfo info;

  info.previousValue = 0;
  info.value = 0;
  info.numVertices = 0;


  int numEdges = aSel.size();

  SimpleGraph graphSelection;  
  for (int i = 0; i < numEdges; ++i )
  {
    Edge e = edge( aSel[i].idEntity );
    graphSelection.addEdge( aSel[i].idEntity, e.bId, e.eId  );
    //info.newVertices.push_back();
  }

  struct  EdgeBevelE{
     HE_edge* bevelEdge1;
     HE_edge* bevelEdge2;
     int edge1VertexDeg;
     int edge2VertexDeg;
  };
  std::vector<EdgeBevelE> edgesToBevel ( numEdges );
  for (int i = 0; i < numEdges; ++i )
  {
    edgesToBevel[i].bevelEdge1 = mMesh->hedge( aSel[i].idEntity );
    edgesToBevel[i].bevelEdge2 = edgesToBevel[i].bevelEdge1->mPair;
    int edge1vertexId   = edgesToBevel[i].bevelEdge1->mVert->mId;
    int edge2vertexId   = edgesToBevel[i].bevelEdge2->mVert->mId;
    edgesToBevel[i].edge1VertexDeg  = graphSelection.degreeOfVertex ( edge1vertexId );
    edgesToBevel[i].edge2VertexDeg  = graphSelection.degreeOfVertex ( edge2vertexId );
  }

  std::vector<int>  singleBorders     = graphSelection.verticesOfDegree( 1 );
  std::vector<int>  sharedBordersDeg2 = graphSelection.verticesOfDegree( 2 );
  std::vector<int>  sharedBordersDegN = graphSelection.verticesOfDegreeAbove( 2 );

  int numVerticesDegree1 = singleBorders.size();
  int numVerticesDegree2 = sharedBordersDeg2.size();
  int numVerticesDegreeN = sharedBordersDegN.size();

  //  Expand vertices of degree 1
  std::vector<BevelEIntermidiateSharedNEdge> infoSingle ( numVerticesDegree1 );
  for ( int i = 0; i < numVerticesDegree1 ; ++i )
  {
    int vertexId  = singleBorders[i];
    int edgeId    = (graphSelection.edgesOfVertex( singleBorders[i] ))[0];
    infoSingle[i] = bevelESplitSingleBorder( vertexId , edgeId, info );
  }

  //  Expand vertices of degree 2
  std::vector<BevelEIntermidiateSharedNEdge>  infoShared2;
  std::vector<BevelESharedNEdgeGroup>         shared2Groups ( numVerticesDegree2 );
  int currentIndex = 0;
  for ( int i = 0; i < numVerticesDegree2 ; ++i )
  {
    int vertexId  = sharedBordersDeg2[i];
    std::vector<int> edgesId  = graphSelection.edgesOfVertex( vertexId );
    shared2Groups[i].numEdges = edgesId.size();
    shared2Groups[i].index    = currentIndex;
    currentIndex              = currentIndex + shared2Groups[i].numEdges;

    bevelESplitSharedBorderDeg2( vertexId , edgesId, info, infoShared2 );
  }

  //  Expand vertices of degree N
  std::vector<BevelEIntermidiateSharedNEdge>  infoSharedN;
  std::vector<BevelESharedNEdgeGroup>         sharedNGroups ( numVerticesDegreeN );
  currentIndex = 0;
  for ( int i = 0; i < numVerticesDegreeN ; ++i )
  {
    int vertexId  = sharedBordersDegN[i];
    std::vector<int> edgesId  = graphSelection.edgesOfVertex( vertexId );
    sharedNGroups[i].numEdges = edgesId.size();
    sharedNGroups[i].index    = currentIndex;
    currentIndex              = currentIndex + sharedNGroups[i].numEdges;

    bevelESplitSharedBorderDegN( vertexId , edgesId, info, infoSharedN );
  }

  //  Extend each pair of edges into a face
  for ( int i = 0; i < numEdges ; ++i )
  {
    BevelEIntermidiateSharedNEdge& edge1 = bevelEGetEdge
        ( infoSingle, infoShared2, infoSharedN, edgesToBevel[i].bevelEdge1 );
    BevelEIntermidiateSharedNEdge& edge2 = bevelEGetEdge
        ( infoSingle, infoShared2, infoSharedN, edgesToBevel[i].bevelEdge2 );
    bevelECreateFace ( edge1, edge2 );
  }

  //  Complete any crossing that's left - 2
  for ( int i = 0; i < numVerticesDegree2 ; ++i )
  {
    int index     = shared2Groups[i].index;
    int numEdges  = shared2Groups[i].numEdges;
    for ( int j = 0; j < shared2Groups[i].numEdges ; ++j )
    {
      if ( !infoShared2[index + j].hasFirst )
      {
        HE_edge* first = infoShared2[index + ((j + 1) % numEdges)].top;
        HE_edge* nextOfF = infoShared2[index + j].nextOffirst;
        first->next ( nextOfF );        
      }
      if ( !infoShared2[index + j].hasLast )
      {
        HE_edge* last = infoShared2[index + ((j + numEdges - 1) % numEdges)].bottom;
        HE_edge* prevOfL = infoShared2[index + j].prevOflast;
        last->prev(prevOfL);        
      }
    }
  }

  //  Complete any crossing that's left - N
  for ( int i = 0; i < numVerticesDegreeN ; ++i )
  {
    int index     = sharedNGroups[i].index;
    int numEdges  = sharedNGroups[i].numEdges;
    for ( int j = 0; j < sharedNGroups[i].numEdges ; ++j )
    {
      if ( !infoSharedN[index + j].hasFirst )
      {
        HE_edge* first = infoSharedN[index + ((j + 1) % numEdges)].top;
        HE_edge* nextOfF = infoSharedN[index + j].nextOffirst;
        first->next ( nextOfF );        
      }
      if ( !infoSharedN[index + j].hasLast )
      {
        HE_edge* last = infoSharedN[index + ((j + numEdges - 1) % numEdges)].bottom;
        HE_edge* prevOfL = infoSharedN[index + j].prevOflast;
        last->prev(prevOfL);        
      }
    }
  }

  resizeSelectionVectors();
  checkIntegrity();
  return info;
}

void MeshComponent::bevelE( ObjEd::BevelEInfo &aExt )
{
  int n = aExt.numVertices;
  for (int i = 0 ; i < n ; ++i )
  {
    D3DXVECTOR3 pos = aExt.center1[i]
                    + aExt.direction1[i]*aExt.value
                    + aExt.direction2[i]*aExt.value;
    VertexObj v;
    v.pos = pos;
    aExt.newVertices[i]->modifyVertex( v );
  }

  aExt.previousValue = aExt.value;

}

void MeshComponent::bevelESplitSharedBorderDeg2(const int aVId, const std::vector<int>& aEdgesId,
                                            BevelEInfo& aInfo,
                                            std::vector<BevelEIntermidiateSharedNEdge>& aEdgeInfo)
{

  HE_vert* v  = mMesh->hvert( aVId );
  HE_edge* eb1  = mMesh->hedgeIncidentToVertex( aEdgesId[0], v );
  HE_edge* eb2  = mMesh->hedgeIncidentToVertex( aEdgesId[1], v );
  std::vector<HE_edge*> incidentEdges = v->edgesSorted( eb1 );

  int degreeVSel  = aEdgesId.size();
  int degreeV     = incidentEdges.size();

  //  Count number of incident edges above and below, store them apart
  std::vector<HE_edge*> eTop;
  std::vector<HE_edge*> eBot;
  int incidentEdgesBelow = 0;
  int index = 1;
  while ( incidentEdges[index] != eb2 )
  {
    eBot.push_back( incidentEdges[index] );
    ++index;
    ++incidentEdgesBelow;
  }
  int incidentEdgesAbove = incidentEdges.size() - index - 1;
  ++index;
  while ( incidentEdges[(index % degreeV)] != eb1 )
  {
    eTop.push_back( incidentEdges[index] );
    ++index;
  }

  bool caseTreated = false;

  //  Case 1: no extra face added
  if ( incidentEdgesAbove <= 1 &&
       incidentEdgesBelow <= 1 &&
       degreeV == 3)
  {
    caseTreated = true;
    std::vector<BevelEIntermidiateSharedNEdge> shared2EdgeInfo ( 2 );
    HE_vert* newV =  mMesh->newVertex( v->mVertex ); ;

    HE_edge* i1;
    for ( int i = 0; i < 3; ++i )
      if ( eb1 != incidentEdges[i] && eb2 != incidentEdges[i])
        i1 = incidentEdges[i];

    bool edgeIncidentAbove = ( eb1->mPair->mPrev == eb2 );

    //  put geometric data
    aInfo.numVertices += 2;

    aInfo.center1.push_back( v->mVertex.pos );
    aInfo.center1.push_back( v->mVertex.pos );
    aInfo.newVertices.push_back( v );
    aInfo.newVertices.push_back( newV );

    if (!edgeIncidentAbove)
    {
      aInfo.direction1.push_back( i1->mPair->mVert->mVertex.pos - v->mVertex.pos );
      aInfo.direction2.push_back( D3DXVECTOR3(0,0,0) );
      aInfo.direction1.push_back( eb2->mPair->mVert->mVertex.pos - v->mVertex.pos );
      aInfo.direction2.push_back( eb1->mPair->mVert->mVertex.pos - v->mVertex.pos );
    }
    else
    {
      aInfo.direction1.push_back( eb2->mPair->mVert->mVertex.pos - v->mVertex.pos );
      aInfo.direction2.push_back( eb1->mPair->mVert->mVertex.pos - v->mVertex.pos );
      aInfo.direction1.push_back( i1->mPair->mVert->mVertex.pos - v->mVertex.pos );
      aInfo.direction2.push_back( D3DXVECTOR3(0,0,0) );
    }


    HE_edge* ip1  = i1->mPair;
    HE_edge* exp1 = mMesh->bevelEnewHalfEdge( newV );
    HE_edge* exp2 = mMesh->bevelEnewHalfEdge( v );
    exp1->pair( exp2 );
    mMesh->newFullEdge( exp1, exp2 );
    eb2->mVert = newV;

    //  TODO COMP: remove shared code from each of the three branches

    //  Detect where is the third edge
    if ( incidentEdgesAbove == 1 &&
         incidentEdgesBelow == 0 )
    {
      caseTreated = true;
      i1->mVert = newV;
      eb1->next  ( exp1 );
      exp1->next( ip1);

      shared2EdgeInfo[0].bevelEdge    = eb1;
      shared2EdgeInfo[0].expandedEdge = exp1;
      shared2EdgeInfo[0].face         = eb1->mPair->mFace;
      shared2EdgeInfo[0].hasFirst     = false;
      shared2EdgeInfo[0].hasLast      = true;
      shared2EdgeInfo[0].last         = ip1;

      shared2EdgeInfo[1].bevelEdge    = eb2;
      shared2EdgeInfo[1].expandedEdge = exp2;
      shared2EdgeInfo[1].face         = eb2->mPair->mFace;
      shared2EdgeInfo[1].hasFirst     = true;
      shared2EdgeInfo[1].hasLast      = false;
      shared2EdgeInfo[1].first        = i1;

    }

    if ( incidentEdgesAbove == 0 &&
         incidentEdgesBelow == 1 )
    {
      eb2->next  ( exp2 );
      exp2->next( ip1);

      shared2EdgeInfo[0].bevelEdge    = eb1;
      shared2EdgeInfo[0].expandedEdge = exp1;
      shared2EdgeInfo[0].face         = eb1->mPair->mFace;
      shared2EdgeInfo[0].hasFirst     = true;
      shared2EdgeInfo[0].hasLast      = false;
      shared2EdgeInfo[0].first        = i1;

      shared2EdgeInfo[1].bevelEdge    = eb2;
      shared2EdgeInfo[1].expandedEdge = exp2;
      shared2EdgeInfo[1].face         = eb2->mPair->mFace;
      shared2EdgeInfo[1].hasFirst     = false;
      shared2EdgeInfo[1].hasLast      = true;
      shared2EdgeInfo[1].last         = ip1;
    }

    aEdgeInfo.push_back( shared2EdgeInfo[0] );
    aEdgeInfo.push_back( shared2EdgeInfo[1] );
  }

  //  Case 1_1: no extra face added (but incident edges at both up and down
  if ( incidentEdgesAbove == 1 &&
       incidentEdgesBelow == 1  )
  {
    caseTreated = true;
    std::vector<BevelEIntermidiateSharedNEdge> shared2EdgeInfo ( 2 );
    HE_vert* newV =  mMesh->newVertex( v->mVertex ); ;

    HE_edge* i1 = incidentEdges[1];
    HE_edge* i2 = incidentEdges[3];

    //  put geometric data
    aInfo.numVertices += 2;

    aInfo.center1.push_back( v->mVertex.pos );
    aInfo.center1.push_back( v->mVertex.pos );
    aInfo.newVertices.push_back( v );
    aInfo.newVertices.push_back( newV );

    aInfo.direction1.push_back( i1->mPair->mVert->mVertex.pos - v->mVertex.pos );
    aInfo.direction2.push_back( D3DXVECTOR3(0,0,0) );
    aInfo.direction1.push_back( i2->mPair->mVert->mVertex.pos - v->mVertex.pos );
    aInfo.direction2.push_back( D3DXVECTOR3(0,0,0) );

    HE_edge* ip1  = i1->mPair;
    HE_edge* ip2  = i2->mPair;
    HE_edge* exp1 = mMesh->bevelEnewHalfEdge( newV );
    HE_edge* exp2 = mMesh->bevelEnewHalfEdge( v );

    exp1->pair( exp2 );
    mMesh->newFullEdge( exp1, exp2 );
    eb2->mVert = newV;

    i2->mVert = newV;
    eb1->next  ( exp1 );
    exp1->next( ip2);

    eb2->next  ( exp2 );
    exp2->next( ip1);

    shared2EdgeInfo[0].bevelEdge    = eb1;
    shared2EdgeInfo[0].expandedEdge = exp1;
    shared2EdgeInfo[0].face         = eb1->mPair->mFace;
    shared2EdgeInfo[0].hasFirst     = true;
    shared2EdgeInfo[0].hasLast      = true;
    shared2EdgeInfo[0].first        = i1;
    shared2EdgeInfo[0].last         = ip2;

    shared2EdgeInfo[1].bevelEdge    = eb2;
    shared2EdgeInfo[1].expandedEdge = exp2;
    shared2EdgeInfo[1].face         = eb2->mPair->mFace;
    shared2EdgeInfo[1].hasFirst     = true;
    shared2EdgeInfo[1].hasLast      = true;
    shared2EdgeInfo[1].first        = i2;
    shared2EdgeInfo[1].last         = ip1;

    aEdgeInfo.push_back( shared2EdgeInfo[0] );
    aEdgeInfo.push_back( shared2EdgeInfo[1] );
  }


  //  Case 2: one extra face added
  if ( !caseTreated )
  {
    int kTop = eTop.size();
    int kBot = eBot.size();

    std::vector<BevelEIntermidiateSharedNEdge> shared2EdgeInfo ( 2 );
    std::vector<HE_vert*> vTop ( kTop );
    std::vector<HE_vert*> vBot ( kBot );
    std::vector<HE_edge*> iTop;
    std::vector<HE_edge*> oTop;
    std::vector<HE_edge*> iBot;
    std::vector<HE_edge*> oBot;

    if ( kTop != 0 )
    {
      iTop.resize( kTop - 1 );
      oTop.resize( kTop - 1 );
    }
    else
    {
      vTop.resize(1);
      //kTop = 0;
    }
    if ( kBot != 0 )
    {
      iBot.resize( kBot - 1 );
      oBot.resize( kBot - 1 );
    }
    else
    {
      vBot.resize(1);
      //kBot = 0;
    }

    HE_vert* newV = mMesh->newVertex( v->mVertex );
    eb2->mVert = newV;

    //  Create bottom and top chains

    vTop[0] = newV;
    for ( int i = 1; i < kTop ; ++i )
      vTop[i] = mMesh->newVertex( v->mVertex );

    vBot[0] = v;
    for ( int i = 1; i < kBot ; ++i )
      vBot[i] = mMesh->newVertex( v->mVertex );

    HE_face* finner = mMesh->newFace();

    for ( int i = 0; i < ( kTop - 1 ) ; ++i )
    {
      oTop[i] = mMesh->newHalfEdge( vTop[i], eTop[i]->mPair->mFace );
      iTop[i] = mMesh->newHalfEdge( vTop[i + 1], finner );

      oTop[i]->pair( iTop[i] );
      mMesh->newFullEdge( oTop[i], iTop[i] );

      oTop[i]->next( eTop[i]->mPair );
      oTop[i]->prev( eTop[i + 1] );
    }
    for ( int i = 0; i < ( kTop - 2 ) ; ++i )
      iTop[i]->next( iTop[i + 1] );
    for ( int i = 0; i < kTop ; ++i )
      eTop[i]->mVert = vTop[i];


    for ( int i = 0; i < ( kBot - 1 ) ; ++i )
    {
      oBot[i] = mMesh->newHalfEdge( vBot[i], eBot[i]->mPair->mFace );
      iBot[i] = mMesh->newHalfEdge( vBot[i + 1], finner );

      oBot[i]->pair( iBot[i] );
      mMesh->newFullEdge( oBot[i], iBot[i] );

      oBot[i]->next( eBot[i]->mPair );
      oBot[i]->prev( eBot[i + 1] );
    }
    for ( int i = 0; i < ( kBot - 2 ) ; ++i )
      iBot[i]->next( iBot[i + 1] );
    for ( int i = 0; i < kBot  ; ++i )
      eBot[i]->mVert = vBot[i];


    //Handle the three cases, given the lack of interior edges
    //in either top or bottom

    HE_edge* exp1;
    HE_edge* exp2;

    if ( kTop != 0)
      exp1 = mMesh->bevelEnewHalfEdge( vTop[( kTop - 1 )] );
    else
      exp1 = mMesh->bevelEnewHalfEdge( vTop[0] );

    if ( kBot != 0)
      exp2 = mMesh->bevelEnewHalfEdge( vBot[( kBot - 1 )] );
    else
      exp2 = mMesh->bevelEnewHalfEdge( vBot[0] );

    HE_edge* ep1 = mMesh->newHalfEdge( vBot[0], finner );
    HE_edge* ep2 = mMesh->newHalfEdge( vTop[0], finner );

    exp1->pair( ep1 );
    mMesh->newFullEdge( exp1, ep1 );

    exp2->pair( ep2 );
    mMesh->newFullEdge( exp2, ep2 );

    //  common topological information
    shared2EdgeInfo[0].bevelEdge    = eb1;
    shared2EdgeInfo[0].expandedEdge = exp1;
    shared2EdgeInfo[0].face         = eb1->mPair->mFace;
    shared2EdgeInfo[1].bevelEdge    = eb2;
    shared2EdgeInfo[1].expandedEdge = exp2;
    shared2EdgeInfo[1].face         = eb2->mPair->mFace;


    if ( kBot <= 1 )
    {
      //  Join inner face
      if ( kTop != 0)
        ep1->prev( iTop[kTop - 2] );
      else
        ep1->prev( iTop[0] );
      ep1->next( ep2 );
      ep2->next( iTop[0] );

      //  outer face
      if ( kTop != 0)
        exp1->next( eTop[kTop - 1]->mPair );

      if ( kBot == 1)
      {
        shared2EdgeInfo[0].hasFirst     = true;
        shared2EdgeInfo[0].first        = eBot[0];
      }
      else
        shared2EdgeInfo[0].hasFirst     = false;
      shared2EdgeInfo[0].hasLast      = true;
      shared2EdgeInfo[0].last         = eTop[kTop - 1]->mPair;

      shared2EdgeInfo[1].hasFirst     = true;
      shared2EdgeInfo[1].first        = eTop[0];
      if ( kBot == 1)
      {
        shared2EdgeInfo[1].hasLast      = true;
        shared2EdgeInfo[1].last         = eBot[0]->mPair;
      }
      else
        shared2EdgeInfo[1].hasLast      = false;

    }

    if ( kTop <= 1 )
    {
      //  Join inner face
      if ( kBot != 0)
        ep2->prev( iBot[kBot - 2]);
      else
        ep2->prev( iBot[0]);
      ep2->next( ep1 );
      ep1->next( iBot[0] );

      //  outer face
      if ( kBot != 0)
        exp2->next( eBot[kBot - 1]->mPair);

      if ( kTop == 1)
      {
        shared2EdgeInfo[0].hasLast      = true;
        shared2EdgeInfo[0].last         = eTop[0]->mPair;
      }
      else
        shared2EdgeInfo[0].hasLast     = false;

      shared2EdgeInfo[0].hasFirst     = true;
      shared2EdgeInfo[0].first        = eBot[0];

      if ( kTop == 1)
      {
        shared2EdgeInfo[1].hasFirst     = true;
        shared2EdgeInfo[1].first        = eTop[0];
      }
      else
        shared2EdgeInfo[1].hasFirst     = false;
      shared2EdgeInfo[1].hasLast      = true;
      shared2EdgeInfo[1].last         = eBot[kBot - 1]->mPair;
    }

    if ( kBot > 1 && kTop > 1 )
    {
      //  Join inner face
      if ( kBot != 0)
        ep2->prev( iBot[kBot - 2] );
      else
        ep2->prev( iBot[0] );
      ep2->next( iTop[0] );
      if ( kTop != 0)
        ep1->prev( iTop[kTop - 2] );
      else
        ep1->prev( iTop[0] );
      ep1->next( iBot[0] );

      //  outer face
      if ( kTop != 0 )
        exp1->next( eTop[kTop - 1]->mPair );
      if ( kBot != 0 )
        exp2->next( eBot[kBot - 1]->mPair );

      shared2EdgeInfo[0].hasFirst     = true;
      shared2EdgeInfo[0].hasLast      = true;
      shared2EdgeInfo[0].first        = eBot[0];
      shared2EdgeInfo[0].last         = eTop[kTop - 1]->mPair;

      shared2EdgeInfo[1].hasFirst     = true;
      shared2EdgeInfo[1].hasLast      = true;
      shared2EdgeInfo[1].first        = eTop[0];
      shared2EdgeInfo[1].last         = eBot[kBot - 1]->mPair;

    }

    //  Store topological information
    aEdgeInfo.push_back( shared2EdgeInfo[0] );
    aEdgeInfo.push_back( shared2EdgeInfo[1] );

    //  Store geometrical information
    aInfo.numVertices += 2;

    aInfo.center1.push_back( v->mVertex.pos );
    aInfo.center1.push_back( v->mVertex.pos );
    aInfo.newVertices.push_back( v );
    aInfo.newVertices.push_back( newV );

    aInfo.direction1.push_back( DxMath::unitVector( eb1->mPair->mVert->mVertex.pos, v->mVertex.pos ) );
    aInfo.direction1.push_back( DxMath::unitVector( eb2->mPair->mVert->mVertex.pos , v->mVertex.pos ));

    if ( kBot != 0 )
      aInfo.direction2.push_back( DxMath::unitVector( eBot[0]->mPair->mVert->mVertex.pos , v->mVertex.pos ));
    else
      aInfo.direction2.push_back( DxMath::unitVector( eb2->mPair->mVert->mVertex.pos , v->mVertex.pos ));
    if ( kTop != 0 )
      aInfo.direction2.push_back( DxMath::unitVector( eTop[0]->mPair->mVert->mVertex.pos , v->mVertex.pos ));
    else
      aInfo.direction2.push_back( DxMath::unitVector( eb1->mPair->mVert->mVertex.pos , v->mVertex.pos ));

    for ( int i = 1; i < kTop ; ++i )
    {
      aInfo.numVertices += 1;
      aInfo.center1.push_back( v->mVertex.pos );
      aInfo.newVertices.push_back( vTop[i] );

      if ( i == (kTop - 1) )
      {
        aInfo.direction1.push_back( DxMath::unitVector( eb1->mPair->mVert->mVertex.pos , v->mVertex.pos) );
        aInfo.direction2.push_back( DxMath::unitVector( eTop[i]->mPair->mVert->mVertex.pos , v->mVertex.pos) );
      }
      else
      {
        aInfo.direction1.push_back( DxMath::unitVector( eTop[i]->mPair->mVert->mVertex.pos, v->mVertex.pos) );
        aInfo.direction2.push_back( D3DXVECTOR3(0,0,0) );
      }
    }

    for ( int i = 1; i < kBot  ; ++i )
    {
      aInfo.numVertices += 1;
      aInfo.center1.push_back( v->mVertex.pos );
      aInfo.newVertices.push_back( vBot[i] );

      if (  i == (kBot - 1) )
      {
        aInfo.direction1.push_back( DxMath::unitVector( eb2->mPair->mVert->mVertex.pos, v->mVertex.pos) );
        aInfo.direction2.push_back( DxMath::unitVector( eBot[i]->mPair->mVert->mVertex.pos, v->mVertex.pos) );
      }
      else
      {
        aInfo.direction1.push_back( DxMath::unitVector( eBot[i]->mPair->mVert->mVertex.pos, v->mVertex.pos) );
        aInfo.direction2.push_back( D3DXVECTOR3(0,0,0) );
      }
    }

  }

}

void MeshComponent::bevelESplitSharedBorderDegN(const int aVId, const std::vector<int>& aEdgesId,
                                            BevelEInfo& aInfo,
                                            std::vector<BevelEIntermidiateSharedNEdge>& aEdgeInfo)
{

  HE_vert* v  = mMesh->hvert( aVId );
  HE_edge* e  = mMesh->hedgeIncidentToVertex( aEdgesId[0], v );
  std::vector<HE_edge*> incidentEdges = v->edgesSorted( e );

  int degreeVSel  = aEdgesId.size();
  int degreeV     = incidentEdges.size();

  std::vector<int>      degreeInBetween ( degreeVSel );
  std::vector<HE_edge*> eb;
  std::vector< std::vector<HE_edge*> > incidentEdgesBetween;
  incidentEdgesBetween.resize( degreeVSel );

  for ( int i = 0; i < degreeV ; ++i )
  {
    bool found = false;
    for ( int j = 0; j < degreeVSel && !found ; ++j )
      if ( incidentEdges[i] == mMesh->hedgeIncidentToVertex( aEdgesId[j], v ) )
      {
        found = true;
        eb.push_back( mMesh->hedgeIncidentToVertex( aEdgesId[j], v ) );
      }
  }

  int index = 1;
  for ( int i = 0; i < degreeVSel ; ++i )
  {
    degreeInBetween[i] = 0;
    while ( incidentEdges[(index % degreeV)] != eb[(i + 1) % degreeVSel] )
    {
      incidentEdgesBetween[i].push_back ( incidentEdges[(index % degreeV)] );
      ++index;
      ++degreeInBetween[i];
    }
    ++index;
  }


  //  Process


  int numNewVertices = degreeVSel - 1;

  std::vector<BevelEIntermidiateSharedNEdge> sharedNEdgeInfo ( degreeVSel );
  std::vector<HE_vert*> newV (degreeVSel );

  newV[0] = v;
  for ( int i = 0; i < numNewVertices ; ++i )
    newV[i + 1] = mMesh->newVertex( v->mVertex );

  //  put geometric data
  aInfo.numVertices += (numNewVertices + 1);

  for ( int i = 0; i < degreeVSel ; ++i )
  {
    aInfo.center1.push_back( v->mVertex.pos );
    aInfo.direction2.push_back( eb[i]->mPair->mVert->mVertex.pos - v->mVertex.pos );
    aInfo.direction1.push_back( eb[(i + 1) % degreeVSel]->mPair->mVert->mVertex.pos - v->mVertex.pos );
    aInfo.newVertices.push_back( newV[i] );
  }

  //  Connect
  std::vector<HE_edge*> interiorEdges ( degreeVSel );
  HE_face* finner = mMesh->newFace();

  for ( int i = 0; i < degreeVSel; ++i )
  {
    sharedNEdgeInfo[i].bevelEdge     = eb[i];
    sharedNEdgeInfo[i].expandedEdge  = mMesh->bevelEnewHalfEdge( newV[(i + degreeVSel - 1) % degreeVSel] );

    if ( degreeInBetween[i] == 0)
      sharedNEdgeInfo[i].face          = eb[(i + 1) % degreeVSel]->mFace;
    else
    {
      sharedNEdgeInfo[i].face          = incidentEdgesBetween[i][0]->mFace;
    }

    // New full edge
    interiorEdges[i]                 = mMesh->newHalfEdge( newV[i], finner );
    interiorEdges[i]->pair( sharedNEdgeInfo[i].expandedEdge );
    mMesh->newFullEdge( interiorEdges[i], sharedNEdgeInfo[i].expandedEdge );

    if ( degreeInBetween[i] == 0)
      sharedNEdgeInfo[i].hasFirst = false;
    else
    {
      sharedNEdgeInfo[i].hasFirst = true;
      sharedNEdgeInfo[i].first    = incidentEdgesBetween[i][0];
    }

    if ( degreeInBetween[(i + degreeVSel - 1) % degreeVSel] == 0)
      sharedNEdgeInfo[i].hasLast  = false;
    else
    {
      sharedNEdgeInfo[i].hasLast  = true;
      int k = degreeInBetween[(i + degreeVSel - 1) % degreeVSel] - 1;
      sharedNEdgeInfo[i].last     = incidentEdgesBetween[(i + degreeVSel - 1) % degreeVSel][k]->mPair;
    }
  }

  for ( int i = 0; i < degreeVSel; ++i )
  {
    interiorEdges[i]->next( interiorEdges[(i + 1) % degreeVSel] );
    eb[i]->mVert = newV[i];
    eb[i]->next( sharedNEdgeInfo[i].expandedEdge );
    for ( int j = 0; j < degreeInBetween[i]; ++j )
      incidentEdgesBetween[i][j]->mVert = newV[i];

  }


  for ( int i = 0; i < degreeVSel ; ++i )
    aEdgeInfo.push_back( sharedNEdgeInfo[i] );


}


MeshComponent::BevelEIntermidiateSharedNEdge
MeshComponent::bevelESplitSingleBorder( const int aVId, const int aEId, BevelEInfo& aInfo )
{
  BevelEIntermidiateSharedNEdge info;


  HE_vert* v = mMesh->hvert( aVId );
  HE_edge* eb = mMesh->hedgeIncidentToVertex( aEId, v );
  std::vector<HE_edge *> incidentEdges = v->edgesSorted( eb );
  int degreeV = incidentEdges.size();

  if ( degreeV == 3)
  {
    HE_edge* e1   = incidentEdges[1];
    HE_edge* e2   = incidentEdges[2];
    HE_edge* ep1  = e1->mPair;
    HE_edge* ep2  = e2->mPair;
    HE_face* f    = ep1->mFace;
    HE_vert* v1   = mMesh->newVertex( v->mVertex );

    HE_edge* a  = mMesh->newHalfEdge( v, f );
    HE_edge* ap = mMesh->bevelEnewHalfEdge( v1 );

    a->pair( ap );
    mMesh->newFullEdge( a, ap );

    e2->mVert = v1;
    e2->next( a );
    a->next( ep1 );

    info.bevelEdge      = eb;
    info.first          = e1;
    info.last           = ep2;
    info.expandedEdge   = ap;
    info.face           = e1->mFace;
    info.hasFirst       = true;
    info.hasLast        = true;

    aInfo.numVertices += 2;
    aInfo.center1.push_back( v->mVertex.pos );
    aInfo.center1.push_back( v->mVertex.pos );

    aInfo.direction1.push_back( ep1->mVert->mVertex.pos - v->mVertex.pos );
    aInfo.direction1.push_back( ep2->mVert->mVertex.pos - v->mVertex.pos );

    aInfo.direction2.push_back( D3DXVECTOR3(0,0,0) );
    aInfo.direction2.push_back( D3DXVECTOR3(0,0,0) );

    aInfo.newVertices.push_back( v );
    aInfo.newVertices.push_back( v1 );
  }
  else
  {
    int n = degreeV - 1;
    std::vector<HE_edge*> in (n);
    std::vector<HE_edge*> o (n);
    std::vector<HE_edge*> e (n);
    std::vector<HE_vert*> vn (n);

    for ( int i = 0; i < n; ++i )
      e[i] = incidentEdges[i + 1];

    vn[0] = v;
    for ( int i = 1; i < n; ++i )
     vn[i] = mMesh->newVertex( v->mVertex );

    HE_face* finner  = mMesh->newFace();

    for ( int i = 0; i < n; ++i )
      in[i] = mMesh->newHalfEdge( vn[(i + 1)%n], finner );

    for ( int i = 0; i < (n - 1); ++i )
      o[i] = mMesh->newHalfEdge( vn[i], e[(i + 1)%n]->mFace );
    o[(n - 1)] = mMesh->newHalfEdge( vn[(n - 1)], eb->mFace );


    for ( int i = 0; i < n; ++i )
    {
      o[i]->pair( in[i] );
      mMesh->newFullEdge( o[i], in[i] );

      o[i]->next( e[i]->mPair );
      in[i]->next( in[(i + 1)%n] );
      //in[i]->prev( in[(i + n + 1)%n] );
    }
    for ( int i = 0; i < (n - 1); ++i )
      o[i]->prev( e[(i + 1)%n] );
    o[n - 1]->prev( eb );

    for ( int i = 1; i < n ; ++i )
      e[i]->mVert = vn[i];

    //  Complete operation information
    info.bevelEdge      = eb;
    info.first          = e[0];
    info.last           = e[n - 1]->mPair;
    info.expandedEdge   = o[n - 1];
    info.face           = e[0]->mFace;
    info.hasFirst       = true;
    info.hasLast        = true;

    //  Complete geometrical information

    aInfo.numVertices += n;

    aInfo.center1.push_back( v->mVertex.pos );
    aInfo.direction1.push_back( e[0]->mPair->mVert->mVertex.pos - v->mVertex.pos );
    //aInfo.direction2.push_back( eb->mPair->mVert->mVertex.pos - v->mVertex.pos );
    aInfo.direction2.push_back( D3DXVECTOR3(0,0,0) );
    aInfo.newVertices.push_back( vn[0] );

    aInfo.center1.push_back( v->mVertex.pos );
    aInfo.direction1.push_back( e[(n - 1)]->mPair->mVert->mVertex.pos - v->mVertex.pos );
    //aInfo.direction2.push_back( eb->mPair->mVert->mVertex.pos - v->mVertex.pos );
    aInfo.direction2.push_back( D3DXVECTOR3(0,0,0) );
    aInfo.newVertices.push_back( vn[(n - 1)] );

    for ( int i = 1; i < (n - 1); ++i )
    {
      aInfo.center1.push_back( v->mVertex.pos );
      aInfo.direction1.push_back( e[i]->mPair->mVert->mVertex.pos - v->mVertex.pos );
      aInfo.direction2.push_back( D3DXVECTOR3(0,0,0) );
      aInfo.newVertices.push_back( vn[i] );
    }

  }

  return info;
}

MeshComponent::BevelEIntermidiateSharedNEdge& MeshComponent::bevelEGetEdge
      ( std::vector<BevelEIntermidiateSharedNEdge> &aInfo1,
        std::vector<BevelEIntermidiateSharedNEdge> &aInfo2,
        std::vector<BevelEIntermidiateSharedNEdge> &aInfoN,
        HE_edge *e)
{
  int index;
  index = bevelEGetIndex( aInfoN, e );
  if (index != -1)
    return aInfoN[index];
  index = bevelEGetIndex( aInfo2, e );
  if (index != -1)
    return aInfo2[index];
  index = bevelEGetIndex( aInfo1, e );
  if (index != -1)
    return aInfo1[index];

}

int MeshComponent::bevelEGetIndex(std::vector<BevelEIntermidiateSharedNEdge> &aInfo, HE_edge *e)
{
  int index = 0;
  bool found = false;
  while (!found && index < aInfo.size() )
  {
    if ( aInfo[index].bevelEdge == e )
      found = true;
    else
      ++index;
  }
  if (found)
    return index;
  return -1;
}

void  MeshComponent::
bevelECreateFace( BevelEIntermidiateSharedNEdge& aSharedN1,
                  BevelEIntermidiateSharedNEdge& aSharedN2 )
{

  HE_face* f1 = aSharedN1.face;
  HE_face* f2 = aSharedN2.face;

  HE_vert* v1 = aSharedN1.expandedEdge->mPair->mVert;
  HE_vert* v2 = aSharedN1.expandedEdge->mVert;
  HE_vert* v3 = aSharedN2.expandedEdge->mVert;
  HE_vert* v4 = aSharedN2.expandedEdge->mPair->mVert;

  HE_edge* ne1 = mMesh->newHalfEdge( v3, f1 );
  HE_edge* ne2 = mMesh->newHalfEdge( v2, f2 );

  int oldId = mMesh->fullOfhalf( aSharedN1.bevelEdge );
  ne1->pair( aSharedN1.bevelEdge );
  ne2->pair( aSharedN2.bevelEdge );
  mMesh->newFullEdge    ( aSharedN2.bevelEdge, ne2 );
  mMesh->recycleFullEdge( oldId, aSharedN1.bevelEdge, ne1 );

  aSharedN2.bevelEdge->next   ( aSharedN2.expandedEdge );
  aSharedN2.expandedEdge->next( aSharedN1.bevelEdge );
  aSharedN1.bevelEdge->next   ( aSharedN1.expandedEdge );
  aSharedN1.expandedEdge->next( aSharedN2.bevelEdge );


  if ( aSharedN2.hasFirst )
    aSharedN2.first->next( ne2 );
  else
    aSharedN2.nextOffirst = ne2;

  if ( aSharedN1.hasLast )
    ne2->next( aSharedN1.last );
  else
    aSharedN1.prevOflast = ne2;

  if ( aSharedN1.hasFirst )
    aSharedN1.first->next( ne1 );
  else
    aSharedN1.nextOffirst = ne1;

  if (aSharedN2.hasLast)
    ne1->next( aSharedN2.last );
  else
    aSharedN2.prevOflast = ne1;

  HE_face* f = mMesh->newFace();
  f->mEdge = aSharedN2.bevelEdge;

  aSharedN2.bevelEdge->mFace = f;
  aSharedN2.expandedEdge->mFace = f;
  aSharedN1.bevelEdge->mFace = f;
  aSharedN1.expandedEdge->mFace = f;

  aSharedN1.bevelEdge->mVert = v1;
  aSharedN2.bevelEdge->mVert = v4;

  f1->mEdge = ne1;
  f2->mEdge = ne2;

  aSharedN1.top     = ne2;
  aSharedN1.bottom  = ne1;
  aSharedN2.top     = ne1;
  aSharedN2.bottom  = ne2;

}
