#include "OBjEditor/Mesh/MeshComponent.hpp"

class FacesOfEdgeSearch{
public:
  int   face;
  bool operator () ( const FacesOfEdge &f )
    {
    if( f.fl == face )
      {
      return true;
      }
    if( f.fr == face )
      {
      return true;
      }
    return false;
    }
};

bool  MeshComponent::knifeEdgeNotInBorders ( const ObjEd::KniveLineInfo &aInfo, const int aEdgeId ) const
{
  bool inBorder = false;

  if( aInfo.entityB.typeEntity == eModeEdge )
    if ( aInfo.entityB.idEntity == aEdgeId ) inBorder = true;
  if( aInfo.entityE.typeEntity == eModeEdge )
    if ( aInfo.entityE.idEntity == aEdgeId ) inBorder = true;

  return inBorder;
}

OpInfo::OpKnifeLine MeshComponent::kniveLine ( ObjEd::KniveLineInfo &aInfo, D3DXMATRIX& aView, D3DXVECTOR3 aCamPos )
{
  OpInfo::OpKnifeLine opInfo;

  //  Precompute inverse
  D3DXMATRIX mInvView;
  D3DXMatrixInverse( &mInvView, NULL, &aView );  

  //  Get all faces, and their vertices
  //  transform all vertices to camera space
  std::vector<Face> faces ( nFaces() );
  for (  int i = 0 ; i < nFaces(); ++i )
    faces[i] = face(i);
  std::vector<VertexObj> verticesCS ( nVertices() );
  std::vector<VertexObj> verticesWS ( nVertices() );
  for ( unsigned int i = 0 ; i < nVertices(); ++i )
  {
    verticesWS[i] = vertex(i);
    verticesCS[i] = vertex(i);
    D3DXVec3TransformCoord( &verticesCS[i].pos, &verticesCS[i].pos, &aView );
  }

  //  Transform knife-line to camera space
  D3DXVECTOR3 klb, kle;
  D3DXVec3TransformCoord( &klb, &( aInfo.segmentPointB ), &aView );
  D3DXVec3TransformCoord( &kle, &( aInfo.segmentPointE ), &aView );
  D3DXVECTOR2 knifeSegB = D3DXVECTOR2( klb.x, klb.y );
  D3DXVECTOR2 knifeSegE = D3DXVECTOR2( kle.x, kle.y );

  std::vector<int> edgesIntersected;
  std::vector<D3DXVECTOR3>  edgesIntersectedPoint;

  // get initial face
  std::vector<int> searchFaces = facesOfEntity( aInfo.entityB );

  //  Select all the faces that are visible
  bool  finished = false;
  while ( !finished )
  {
    unsigned int currentSearchFace = 0;
    while( currentSearchFace < searchFaces.size() )
    {
      int faceIndex = searchFaces[currentSearchFace];
      bool frontFacing = Intersections::frontFacing(
            verticesWS[ faces[faceIndex].vId(0) ].pos,
            verticesWS[ faces[faceIndex].vId(1) ].pos,
            verticesWS[ faces[faceIndex].vId(2) ].pos,
            aCamPos);
      bool faceChecked = true;
      if ( frontFacing )
      {
        //  Check intersections of knife-line and faces

        bool intersectedEdgeOnCurrentFace = false;
        for ( unsigned int j = 0 ;
              !intersectedEdgeOnCurrentFace &&
              j < faces[faceIndex].nEdges() ; ++j )
        {
          int edgeId      = faces[faceIndex].eId(j);

          //  only check edge if it has not been previously intersected
          //  -> avoid adding the same edge twice (from both of its faces)
          std::vector<int>::iterator it = std::find( edgesIntersected.begin(),
                                                     edgesIntersected.end(),
                                                     edgeId );
          if ( it == edgesIntersected.end() &&
               !knifeEdgeNotInBorders( aInfo, edgeId ) )
          {
            Edge edgeOfFace = edge( edgeId );

            D3DXVECTOR2 edgeB = D3DXVECTOR2( verticesCS[ edgeOfFace.bId].pos.x,
                                             verticesCS[ edgeOfFace.bId].pos.y );
            D3DXVECTOR2 edgeE = D3DXVECTOR2( verticesCS[ edgeOfFace.eId].pos.x,
                                             verticesCS[ edgeOfFace.eId].pos.y );

            bool intersect = Intersections::segmentIntersectionExclusive( knifeSegB,
                                                                          knifeSegE,
                                                                          edgeB,
                                                                          edgeE );
            if ( intersect )
            {
              edgesIntersected.push_back( edgeId );

              //facesOfEdge( edgeId );    //what was there for?
              //initialFaces = facesOfEdgeList( edgeId );

              D3DXVECTOR2 point2d = Intersections::pointOfIntersectingSegments(knifeSegB, knifeSegE, edgeB, edgeE );

              //  now unproject this point to the 3d-space
              D3DXVECTOR3 rayb = D3DXVECTOR3( point2d.x, point2d.y, 0 );
              D3DXVECTOR3 raye = D3DXVECTOR3( point2d.x, point2d.y, 1 );
              D3DXVECTOR3 point3dCS, point3dWS;
              Intersections::distanceSegmentToLine( rayb, raye,
                                     verticesCS[ edgeOfFace.bId].pos,
                                     verticesCS[ edgeOfFace.eId].pos,
                                     point3dCS );

              //  Now convert it from CS to WS
              D3DXVec3TransformCoord( &point3dWS, &point3dCS, &mInvView );
              edgesIntersectedPoint.push_back( point3dWS );

              //Stop searching in this face
              intersectedEdgeOnCurrentFace = true;

              //Now search the opposite face
              int nextFaceId;
              knifeEdgeContainsFace( edgeId, searchFaces, nextFaceId );
              searchFaces.resize(1);
              searchFaces[0] = nextFaceId;
              currentSearchFace = 0;
              faceChecked = false;

            }
          }

        } //for check edges

      }

      if (faceChecked)
        ++currentSearchFace;
    } // end of for, searchFaces

    //  finish once all current faces have been explored
    if (currentSearchFace >= searchFaces.size() )
      finished = true;

  }

  if ( edgesIntersected.size() > 0)
  {
    std::vector<EdgeIntersection> intersections ( edgesIntersected.size() );
    for ( unsigned int i = 0; i < edgesIntersected.size(); ++i )
    {
      intersections[i].idEntity = edgesIntersected[i];
      intersections[i].p        = edgesIntersectedPoint[i];
    }
    std::vector<int> edgesIntersected;
    std::vector<D3DXVECTOR3>  edgesIntersectedPoint;

    opInfo = OpInfo::OpKnifeLine ( aInfo, intersections, this->mComponentId );
  }
  else
  {
    opInfo = OpInfo::OpKnifeLine ( aInfo, this->mComponentId );
  }

  return opInfo;
}


void MeshComponent::kniveLine ( const OpInfo::OpKnifeLine& aOp )
{
  if ( aOp.mCuttedEdges.empty() )
    kniveLineCutOneFace( aOp );
  else
    kniveLineCutManyFaces( aOp );

  resizeSelectionVectors();
  checkIntegrity();
}

void MeshComponent::kniveLineCutOneFace   ( const OpInfo::OpKnifeLine& aOp )
{
  //  Just one face to cut
  HE_face* cutFace;

  if ( aOp.mKnifeInfo.entityB.typeEntity == eModeVertex && aOp.mKnifeInfo.entityE.typeEntity == eModeVertex  )
  {
    cutFace = mMesh->sharedFace( mMesh->hvert( aOp.mKnifeInfo.entityB.idEntity),
                                 mMesh->hvert( aOp.mKnifeInfo.entityE.idEntity) );
    mMesh->splitFace( cutFace,
                      mMesh->hvert( aOp.mKnifeInfo.entityB.idEntity),
                      mMesh->hvert( aOp.mKnifeInfo.entityE.idEntity));
  }

  if ( aOp.mKnifeInfo.entityB.typeEntity == eModeEdge && aOp.mKnifeInfo.entityE.typeEntity == eModeEdge  )
  {
    cutFace = mMesh->sharedFace( mMesh->hedge( aOp.mKnifeInfo.entityB.idEntity),
                                 mMesh->hedge( aOp.mKnifeInfo.entityE.idEntity) );

    VertexObj newVertexData1, newVertexData2;
    newVertexData1.pos = aOp.mKnifeInfo.segmentPointB;
    newVertexData2.pos = aOp.mKnifeInfo.segmentPointE;

    HE_vert* newVertex1 = mMesh->splitEdge( mMesh->hedge( aOp.mKnifeInfo.entityB.idEntity), newVertexData1 );
    HE_vert* newVertex2 = mMesh->splitEdge( mMesh->hedge( aOp.mKnifeInfo.entityE.idEntity), newVertexData2 );

    mMesh->splitFace( cutFace,
                      newVertex1,
                      newVertex2);


  }

  if ( aOp.mKnifeInfo.entityB.typeEntity == eModeVertex && aOp.mKnifeInfo.entityE.typeEntity == eModeEdge  )
  {
    cutFace = mMesh->sharedFace( mMesh->hvert( aOp.mKnifeInfo.entityB.idEntity),
                                 mMesh->hedge( aOp.mKnifeInfo.entityE.idEntity) );

    VertexObj newVertexData;
    newVertexData.pos = aOp.mKnifeInfo.segmentPointE;
    HE_vert* newVertex = mMesh->splitEdge( mMesh->hedge( aOp.mKnifeInfo.entityE.idEntity), newVertexData );

    mMesh->splitFace( cutFace,
                      mMesh->hvert( aOp.mKnifeInfo.entityB.idEntity),
                      newVertex);
  }

  if ( aOp.mKnifeInfo.entityB.typeEntity == eModeEdge && aOp.mKnifeInfo.entityE.typeEntity == eModeVertex  )
  {
    cutFace = mMesh->sharedFace( mMesh->hedge( aOp.mKnifeInfo.entityB.idEntity),
                                 mMesh->hvert( aOp.mKnifeInfo.entityE.idEntity) );

    VertexObj newVertexData;
    newVertexData.pos = aOp.mKnifeInfo.segmentPointB;
    HE_vert* newVertex = mMesh->splitEdge( mMesh->hedge( aOp.mKnifeInfo.entityB.idEntity), newVertexData );

    mMesh->splitFace( cutFace,
                      newVertex,
                      mMesh->hvert( aOp.mKnifeInfo.entityE.idEntity));
  }
}

void MeshComponent::kniveLineCutManyFaces ( const OpInfo::OpKnifeLine& aOp )
{
  std::vector<int>          edgesIntersected      ( aOp.mCuttedEdges.size() );
  std::vector<D3DXVECTOR3>  edgesIntersectedPoint ( aOp.mCuttedEdges.size() );

  for ( unsigned int i = 0; i < aOp.mCuttedEdges.size(); ++i )
  {
    edgesIntersected[i]       = aOp.mCuttedEdges[i].idEntity;
    edgesIntersectedPoint[i]  = aOp.mCuttedEdges[i].p;
  }

  std::vector<int>  facesToCut;
  std::list<FacesOfEdge>  facesList;

  for ( int i = 0; i < edgesIntersected.size(); ++i)
  {
    FacesOfEdge flr = facesOfEdge( edgesIntersected[i] );
    facesList.push_back( flr );
    facesToCut.push_back( flr.fl );
    facesToCut.push_back( flr.fr );
  }

  bool  uniqueFound = false;
  int   index = 0;
  while ( !uniqueFound)
  {
    int ocurrences = count( facesToCut.begin(), facesToCut.end(), facesToCut[index] );
    if ( ocurrences == 1)
      uniqueFound = true;
    else
      index++;
  }

  //  Now make a sorted list of faces traversal
  std::vector<int> sortedFacesTraversal;
  sortedFacesTraversal.push_back( facesToCut[index] );
  int previousFace = facesToCut[index];
  while ( facesList.size() > 0 )
  {
    FacesOfEdgeSearch fs;
    fs.face = previousFace;
    std::list<FacesOfEdge>::iterator found = find_if( facesList.begin(), facesList.end(), fs );
    if ( (*found).fl == previousFace )
      previousFace = (*found).fr;
    else
      previousFace = (*found).fl;
    sortedFacesTraversal.push_back( previousFace );
    facesList.erase( found );
  }

  //  Ensure order of "sortedFacesTraversal", the first face must contain
  //  the first entity
  if ( !faceContainsSelectedEntity( sortedFacesTraversal[0], aOp.mKnifeInfo.entityB ) )
  {
    //  Reverse it
    std::reverse(sortedFacesTraversal.begin(), sortedFacesTraversal.end());
    std::reverse(edgesIntersected.begin(), edgesIntersected.end());
    std::reverse(edgesIntersectedPoint.begin(), edgesIntersectedPoint.end());
  }

  //  To sum it up, we have N intersections, and N+1 faces
  //  (N+1)   sortedFacesTraversal
  //  N       edgesIntersected
  //  N       edgesIntersectedPoint

  HE_vert* vb;
  HE_vert* ve;

  bool  previousEntityIsVertex = (aOp.mKnifeInfo.entityB.typeEntity == eModeVertex );
  if ( previousEntityIsVertex )
    vb = mMesh->hvert( aOp.mKnifeInfo.entityB.idEntity );
  else
  {
    VertexObj newVertexData;
    newVertexData.pos = aOp.mKnifeInfo.segmentPointB;
    vb = mMesh->splitEdge( mMesh->hedge( aOp.mKnifeInfo.entityB.idEntity ) , newVertexData );
  }

  VertexObj newVertexData;
  newVertexData.pos = edgesIntersectedPoint[0];
  ve = mMesh->splitEdge( mMesh->hedge( edgesIntersected[0] ) , newVertexData );

  //  consecutively cut faces


  //  TODO: cut possible at non-edge (that is vertex)
  for ( unsigned int i = 0; i < sortedFacesTraversal.size(); ++i)
  {
    vb = mMesh->splitFace( mMesh->hface(sortedFacesTraversal[i]),
                      vb,
                      ve);

    if ( i <  (sortedFacesTraversal.size() - 2 ))
    {
      //  Edge intersection
      VertexObj newVertexData;
      newVertexData.pos = edgesIntersectedPoint[i + 1];
      ve = mMesh->splitEdge( mMesh->hedge( edgesIntersected[i + 1]) , newVertexData );
    }
    else
    {
      //  last face, could end in vertex or edge
      if ( i <  (sortedFacesTraversal.size() - 1 ))
      {
        if ( aOp.mKnifeInfo.entityE.typeEntity == eModeVertex )
          ve = mMesh->hvert( aOp.mKnifeInfo.entityE.idEntity );
        else
          {
            VertexObj newVertexData;
            newVertexData.pos = aOp.mKnifeInfo.segmentPointE;
            ve = mMesh->splitEdge( mMesh->hedge( aOp.mKnifeInfo.entityE.idEntity ) , newVertexData );
          }
      }
    }
  }
}

//  \param    aId               edge id
//  \param    aFaces            list of faces
//  \param    aOpposingFaceId   true=> (f1 = face(e) && f1 not exists in aFaces)
//  \returns  true<=> exist f in aFaces, such that face(e)=f
bool  MeshComponent::knifeEdgeContainsFace ( const int aId,
                                             const std::vector<int>& aFaces,
                                             int& aOpposingFaceId ) const
{
  FacesOfEdge faces = facesOfEdge( aId );
  bool oneFaceFound = false;

  for ( int i = 0; i < aFaces.size() && !oneFaceFound ; ++i)
  {
    if ( aFaces[i] == faces.fl )
    {
      oneFaceFound    = true;
      aOpposingFaceId = faces.fr;
    }
    if ( aFaces[i] == faces.fr )
    {
      oneFaceFound    = true;
      aOpposingFaceId = faces.fl;
    }

  }

  return oneFaceFound;
}
