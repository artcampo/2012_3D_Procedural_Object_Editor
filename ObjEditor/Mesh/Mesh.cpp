#include "Mesh.hpp"

Mesh::~Mesh()
{
  release();
}

Mesh::Mesh(  const MeshSerial& rhs )
{
  mMeshObject.resize( rhs.mMeshObject.size() );
  mSelectedComponent = rhs.mSelectedComponent;
  mStack = new OpStack( *rhs.mStack );

  for ( unsigned int j = 0; j < rhs.mMeshObject.size() ; ++j)
  {
    mMeshObject[j] = new MeshComponent( *rhs.mMeshObject[j] );
  }

  mSelO.resize( rhs.mMeshObject.size() );
  mTextures = rhs.mTextures;

}

std::vector<EditableMeshModel*> Mesh::convert( ID3D10Device* aDev, D3DXVECTOR3 aCamPos  , ColourMapping* aColours )
{
  std::vector<EditableMeshModel*> models;
  for (int i = 0; i < nComponents() ; ++i )
    models.push_back( mMeshObject[i]->convert(aDev, aCamPos, aColours) );
  return models;
}

std::vector<EditableMeshModelProcTex*> Mesh::convertProcTex( ID3D10Device* aDev, D3DXVECTOR3 aCamPos  , ColourMapping* aColours )
{
  std::vector<EditableMeshModelProcTex*> models;
  if ( hasProcTex() )
    for (int i = 0; i < nComponents() ; ++i )
      models.push_back( mMeshObject[i]->convertProcTex(aDev, aCamPos, aColours, mTextures.size(), mTextures) );
  return models;
}

Mesh::Mesh()
{
  init();
  mSelectedComponent = 0;
  mTransValue = D3DXVECTOR3(0,0,0);  
}

void Mesh::setMode(int aMode)
{
  mMode = aMode;
  for ( unsigned int i = 0 ; i < mMeshObject.size(); ++i )
    mMeshObject[i]->setMode(aMode);
}

//    \return   -1 if no entity, or its id
Selection Mesh::checkEntity(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, float aDistThreshold,
                            D3DXMATRIX& aView, D3DXVECTOR3 aCamPos,
                            D3DXVECTOR3& aPointInSegment, bool atHalf, int aWhichHalf, D3DXVECTOR3& aPoint )
{
  Selection id;

  //TODO: only initial
  id.idEntity = -1;
  id.idComponent = -1;

  switch( mMode )
  {
  case eModeVertex:
    id = checkVertex(aRayB,aRayE,aDistThreshold, aPoint, aCamPos );
    break;
  case eModeEdge:
    id = checkEdge(aRayB,aRayE,aDistThreshold, aPointInSegment, atHalf, aWhichHalf,
                   aView, aCamPos );
    break;
  case eModeFace:
    id = checkFace(aRayB,aRayE, aView, aCamPos );
    break;
  case eModeObject:
    id = checkObject(aRayB,aRayE, aView, aCamPos );
    break;
  }
  return id;
}

Selection Mesh::checkVertex(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, float aDistThreshold,
                            D3DXVECTOR3& aPoint, D3DXVECTOR3 aCamPos )
{
  Selection s;

  s.idComponent = -1;
  s.typeEntity = eModeVertex;
  s.idEntity = -1;

  for ( unsigned int i = 0 ; i < mMeshObject.size(); ++i )
  {
    bool  existsMin = false;
    float distanceMin;
    std::vector<Face> checkFaces    = mMeshObject[i]->facesFacingCamera( aCamPos );
    std::vector<int> uniqueVertices = verticesOfFaces( checkFaces );

    for ( unsigned int j = 0 ; j < uniqueVertices.size() ; ++j )
    {
      int vertexId = uniqueVertices[j];
      D3DXVECTOR3 pos = mMeshObject[i]->vertex(vertexId).pos;

      float distance = Intersections::distancePointToLine ( aRayB, aRayE, pos );
      bool update = false;
      if (!existsMin && distance < aDistThreshold ) update = true;
      if ( existsMin && distance < aDistThreshold && distance<distanceMin ) update = true;

      if (update)
      {
        distanceMin = distance;
        s.idComponent = i;
        s.idEntity = vertexId;
        aPoint = pos;
      }
      mMeshObject[i]->mHighlight.idEntity = -1;

    }

  }

  //  Write the highlighted one
  if (s.idComponent != -1)
    mMeshObject[ s.idComponent ]->mHighlight = s;

  return s;
}



Selection Mesh::checkFace(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXMATRIX& aView, D3DXVECTOR3 aCamPos )
{
  Selection s;

  s.idComponent = -1;
  s.typeEntity = eModeFace;
  s.idEntity = -1;

  for ( unsigned int i = 0 ; i < mMeshObject.size(); ++i )
  {
    bool  existsMin = false;
    float distanceMin;
    for ( int j = 0 ; j < mMeshObject[i]->nFaces(); ++j )
    {
    Face f = mMeshObject[i]->face(j);

    bool update = false;
    bool hit = false;
/*
  TODO: collinear points
    int b = f.nDecompositionTriangles();
    if ( b == 4)
    {
      int a = b;
      ++a;

    }*/

    for (int k = 0; k < f.nDecompositionTriangles() ; ++k)
    {
      Face::TriangleDecomposition triangle = f.decompTriangle(k);
      int p1Id = f.vId( triangle.v1 );
      int p2Id = f.vId( triangle.v2 );
      int p3Id = f.vId( triangle.v3 );
      D3DXVECTOR3 p1 = mMeshObject[i]->vertex( p1Id  ).pos;
      D3DXVECTOR3 p2 = mMeshObject[i]->vertex( p2Id ).pos;
      D3DXVECTOR3 p3 = mMeshObject[i]->vertex( p3Id ).pos;
      D3DXVECTOR3 p1CS, p2CS, p3CS;
      D3DXVec3TransformCoord( &p1CS, &p1, &aView );
      D3DXVec3TransformCoord( &p2CS, &p2, &aView );
      D3DXVec3TransformCoord( &p3CS, &p3, &aView );
      //  Avoid collinear points in a face's edge
      if ( ! Intersections::areCollinear(p1,p2,p3) )
        if ( Intersections::frontFacing(p1,p2,p3,aCamPos) )
          hit |= D3DXIntersectTri( &p1, &p2, &p3, &aRayB, &aRayE, NULL, NULL, NULL );
    }

    float distance;
    if (hit)
    {
      D3DXVECTOR3 posCS;
      D3DXVec3TransformCoord( &posCS, &(mMeshObject[i]->center(f)), &aView );
      distance = posCS.z;
    }

    if (!existsMin && hit  ) update = true;
    if ( existsMin && hit && distance<distanceMin ) update = true;

    if (update)
    {
      distanceMin = distance;
      s.idComponent = i;
      s.idEntity = j;
    }
    mMeshObject[i]->mHighlight.idEntity = -1;

    }

  }

  //  Write the highlighted one
  if (s.idComponent != -1)
  mMeshObject[ s.idComponent ]->mHighlight = s;

  return s;
}

void Mesh::selectEntity(ObjEd::Selection aSel)
{
  mSelectedComponent = aSel.idComponent;
  switch( mMode )
  {
  case eModeVertex:
    selectVertex( aSel );
    break;
  case eModeEdge:
    selectEdge( aSel );
    break;
  case eModeFace:
    selectFace( aSel );
    break;
  case eModeObject:
    if ( aSel.typeEntity == eModeObject)
      selectObject( aSel.idComponent );
    break;
  }
}

void Mesh::selectVertex(ObjEd::Selection aSel)
{
  mMeshObject[ aSel.idComponent ]->selectVertex( aSel.idEntity );
}

void Mesh::selectEdge(ObjEd::Selection aSel)
{
  mMeshObject[ aSel.idComponent ]->selectEdge( aSel.idEntity );
}

void Mesh::selectFace(ObjEd::Selection aSel)
{
  mMeshObject[ aSel.idComponent ]->selectFace( aSel.idEntity );
}

//  Perform translate on all selected entities
void Mesh::opTranslate(D3DXVECTOR3 aValue,
                       const std::vector<ObjEd::Selection>& aSel )
{     
  mTransSel = aSel;
  mTransValue += aValue;

  switch( mMode )
  {
  case eModeVertex:
    opTranslateVertex( aValue, mTransSel );
    break;
  case eModeEdge:
    //TODO
    opTranslateEdges( aValue, mTransSel );
    break;
  case eModeFace:
    opTranslateFaces( aValue, mTransSel );
    break;
  case eModeObject:
    opTranslateObject( aValue, mTransSel );
    break;
  }
}

void  Mesh::opTranslate( const OpInfo::OpTranslate& aOp )
{
  switch( aOp.sel[0].typeEntity )
  {
  case eModeVertex:
    opTranslateVertex( aOp.value, aOp.sel );
    break;
  case eModeEdge:
    //TODO
    opTranslateEdges( aOp.value, aOp.sel);
    break;
  case eModeFace:
    opTranslateFaces( aOp.value, aOp.sel );
  case eModeObject:
    opTranslateObject( aOp.value, aOp.sel );
    break;
  }
}

void Mesh::endOpTranslate()
{
  mStack->queueOperation( OpInfo::OpTranslate( mTransValue, mTransSel, storeComponentFlag() ) );
  mTransValue = D3DXVECTOR3(0,0,0);
}

void  Mesh::opStartScale()
{  
  mScaleSel = getCurrentSelection();
  mScaleInfo.value = D3DXVECTOR3(1,1,1);
  if ( ! mScaleSel.empty() )
  {
    int entityType = mScaleSel[0].typeEntity;
    mScaleInfo.center = centerMassSelectedEntities( entityType );
  }
}

void Mesh::opScale(D3DXVECTOR3 aValue,
                   const std::vector<ObjEd::Selection>& aSel)
{
  D3DXVECTOR3 value = aValue + D3DXVECTOR3(1,1,1);
  mScaleInfo.value.x *= value.x;
  mScaleInfo.value.y *= value.y;
  mScaleInfo.value.z *= value.z;
  switch( mMode )
  {
  case eModeVertex:
    opScaleVertices( mScaleInfo.center, value, aSel );
    break;
  case eModeEdge:
    //TODO
    opScaleEdges( mScaleInfo.center, value, aSel );
    break;
  case eModeFace:
    opScaleFaces( mScaleInfo.center, value, aSel );
    break;
  case eModeObject:
    opScaleObjects( mScaleInfo.center, value, aSel );
    break;

  }
}

void  Mesh::opScale( const OpInfo::OpScale& aOp )
{  

  switch( aOp.sel[0].typeEntity )
  {
  case eModeVertex:
    opScaleVertices( aOp.center, aOp.value, aOp.sel );
    break;
  case eModeEdge:
    //TODO
    opScaleEdges( aOp.center, aOp.value, aOp.sel );
    break;
  case eModeFace:
    opScaleFaces( aOp.center, aOp.value, aOp.sel );
  case eModeObject:
    opScaleObjects( aOp.center, aOp.value, aOp.sel );
    break;
  }
}

void Mesh::endOpScale()
{
  mStack->queueOperation( OpInfo::OpScale( mScaleInfo.center, mScaleInfo.value, mScaleSel, storeComponentFlag() ) );
}

void Mesh::opTranslateVertex(D3DXVECTOR3 aValue,
                             const std::vector<ObjEd::Selection>& aSel)
{
  for ( unsigned int i = 0; i < mTransSel.size() ; ++i)
    mMeshObject[ mTransSel[i].idComponent ]->opTranslateVertex( mTransSel[i].idEntity, aValue );

}

void Mesh::opScaleVertices(D3DXVECTOR3 aCenter, D3DXVECTOR3 aValue,
                         const std::vector<ObjEd::Selection>& aSel)
{
  for ( unsigned int i = 0; i < aSel.size() ; ++i)
    mMeshObject[ aSel[0].idComponent ]->opScaleVertices( aSel[i].idEntity, aCenter, aValue );

}

void Mesh::unselectEntities()
{
  mTransValue = D3DXVECTOR3(0,0,0);

  switch( mMode )
  {
  case eModeVertex:
    mMeshObject[mSelectedComponent]->unselectVertices();
    break;
  case eModeEdge:
    mMeshObject[mSelectedComponent]->unselectEdges();
    break;
  case eModeFace:
    mMeshObject[mSelectedComponent]->unselectFaces();
    break;
  case eModeObject:
    unselectObjects();
    break;
  }

}

void Mesh::selectAddEntity(ObjEd::Selection aSel)
{
  if ( aSel.idComponent == mSelectedComponent )
  {
    switch( mMode )
    {
    case eModeVertex:
      mMeshObject[mSelectedComponent]->selectVertex( aSel.idEntity );
      break;
    case eModeEdge:
      mMeshObject[mSelectedComponent]->selectEdge( aSel.idEntity );
      break;
    case eModeFace:
      mMeshObject[mSelectedComponent]->selectFace( aSel.idEntity );
      break;
    case eModeObject:
      if ( aSel.typeEntity == eModeObject )
        selectObject( aSel.idComponent );
      break;
    }
  }
}

//  \return   true<=> at least one entity selected
bool Mesh::selectRemoveEntity(ObjEd::Selection aSel)
{
  bool anyLeft;
  switch( mMode )
  {
  case eModeVertex:
    anyLeft = mMeshObject[mSelectedComponent]->selectRemoveVertex( aSel.idEntity );
    break;
  case eModeEdge:
    anyLeft = mMeshObject[mSelectedComponent]->selectRemoveEdge( aSel.idEntity );
    break;
  case eModeFace:
    anyLeft = mMeshObject[mSelectedComponent]->selectRemoveFace( aSel.idEntity );
    break;
  case eModeObject:
    if ( aSel.typeEntity == eModeObject )
      unselectObject( aSel.idComponent );
    break;
  }
  return anyLeft;
}

D3DXVECTOR3 Mesh::centerMassSelectedVertices()
{
  return mMeshObject[mSelectedComponent]->centerMassSelectedVertices();
}

ObjEd::Selection Mesh::checkEdge(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE,
                                 float aDistThreshold, D3DXVECTOR3& aPointInSegment,
                                 bool atHalf, int aWhichHalf,
                                 D3DXMATRIX& aView, D3DXVECTOR3 aCamPos )
{
  Selection s;

  s.idComponent = -1;
  s.typeEntity = eModeEdge;
  s.idEntity = -1;

  for ( unsigned int i = 0 ; i < mMeshObject.size(); ++i )
  {
    bool  existsMin = false;
    float distanceMin;
    std::vector<int> edgesId = mMeshObject[i]->edgesOfFacesFacingCamera( aCamPos );
    for ( unsigned int j = 0 ; j < edgesId.size(); ++j )
    {
      int edgeIdentifier = edgesId[j];
      Edge e = mMeshObject[i]->edge( edgeIdentifier );
      D3DXVECTOR3 vb = mMeshObject[i]->vertex(e.bId).pos;
      D3DXVECTOR3 ve = mMeshObject[i]->vertex(e.eId).pos;

      D3DXVECTOR3 pointInSegment;
      float distance = Intersections::distanceSegmentToLine(  aRayB, aRayE, vb, ve, pointInSegment );
      bool update = false;
      if (!existsMin && distance < aDistThreshold ) update = true;
      if ( existsMin && distance < aDistThreshold && distance<distanceMin ) update = true;

      if (update)
      {
        distanceMin = distance;
        s.idComponent = i;
        s.idEntity = edgeIdentifier;
        if ( !atHalf )
          aPointInSegment = pointInSegment;
        else
        {
          if ( aWhichHalf == 0 )
            aPointInSegment = 0.5f * (vb + ve);
          if ( aWhichHalf == 1 )
            aPointInSegment = 0.33333f*(ve - vb) + vb;
          if ( aWhichHalf == 2 )
            aPointInSegment = 0.66666f*(ve - vb) + vb;
        }
      }
      mMeshObject[i]->mHighlight.idEntity = -1;

    }

  }

  //  Write the highlighted one
  if (s.idComponent != -1)
    mMeshObject[ s.idComponent ]->mHighlight = s;

  return s;
}

std::vector<ObjEd::Selection> Mesh::getCurrentComponentSelection() const
{
  std::vector<ObjEd::Selection> sel;
  for ( unsigned int i = 0; i < mSelO.size(); ++i)
    if(mSelO[i])
    {
      ObjEd::Selection s;
      s.idComponent = i;
      s.typeEntity  = eModeObject;
      s.idEntity  = i;
      sel.push_back( s );
    }

 return sel;
}

std::vector<ObjEd::Selection> Mesh::getCurrentSelection() const
{
  if ( mMode != eModeObject )
    return mMeshObject[mSelectedComponent]->getCurrentSelection();
  else
    return getCurrentComponentSelection();
}

//////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////

void Mesh::opStartExtrude( const std::vector<ObjEd::Selection>& aSel,
                           Options::ExtrudeOptions& aOptions )
{
  mExtrudeSel = aSel;
  mExtrudeInfo.resize( mExtrudeSel.size() );
  mExtrudeCurrentOptions = aOptions;
  if ( eModeFace )
  {
    for ( unsigned int i = 0 ; i < mExtrudeSel.size(); ++i )
    {
      if ( mExtrudeSel[i].typeEntity == eModeFace )
      {
        mExtrudeInfo[i] = mMeshObject[ mExtrudeSel[i].idComponent ]->
            extrudeStart( mExtrudeSel[i].idEntity, aOptions );

      }
    }

    opExtrude( 0.0f );
  }
}


void  Mesh::opExtrude ( float aValue )
{
  for ( unsigned int i = 0 ; i < mExtrudeSel.size(); ++i )
  {
    if ( mExtrudeSel[i].typeEntity == eModeFace )
    {
      mExtrudeInfo[i].value = aValue;
      mMeshObject[ mExtrudeSel[i].idComponent ]->extrude( mExtrudeSel[i].idEntity, mExtrudeInfo[i] );
    }
  }

  //OpInfo::OpExtrude opInfo ( aValue, mExtrudeSel );
}

void Mesh::endOpExtrude()
{
  mStack->queueOperation( OpInfo::OpExtrude ( mExtrudeInfo[0].value,
                                              mExtrudeSel,
                                              mExtrudeCurrentOptions,
                                              storeComponentFlag()) );

  mExtrudeInfo.clear();
  mExtrudeSel.clear();
}

void  Mesh::opExtrude ( const OpInfo::OpExtrude& aOp )
{
  mExtrudeInfo.resize( aOp.sel.size() );
  mExtrudeSel = aOp.sel;
  for ( unsigned int i = 0 ; i < aOp.sel.size(); ++i )
  {
    mExtrudeInfo[i] = mMeshObject[ aOp.sel[i].idComponent ]->
        extrudeStart( aOp.sel[i].idEntity, aOp.options );
  }
  opExtrude( aOp.value );
}

////////////


void Mesh::opStartExtrudeInner( const std::vector<ObjEd::Selection>& aSel,
                                Options::ExtInnerOptions& aOptions)
{
  mExtrudeInnerSel = aSel;
  mExtrudeInnerInfo.resize( mExtrudeInnerSel.size() );
  mExtInnerCurrentOptions = aOptions;
  if ( eModeFace )
  {
    for ( unsigned int i = 0 ; i < mExtrudeInnerSel.size(); ++i )
    {
      if ( mExtrudeInnerSel[i].typeEntity == eModeFace )
      {
        mExtrudeInnerInfo[i] = mMeshObject[ mExtrudeInnerSel[i].idComponent ]->
            extrudeInnerStart( mExtrudeInnerSel[i].idEntity, aOptions );

      }
    }

    opExtrudeInner( 0.1f );
  }
}


void  Mesh::opExtrudeInner ( float aValue )
{
  for ( unsigned int i = 0 ; i < mExtrudeInnerSel.size(); ++i )
  {
    if ( mExtrudeInnerSel[i].typeEntity == eModeFace )
    {
      mExtrudeInnerInfo[i].value = aValue;
      mMeshObject[ mExtrudeInnerSel[i].idComponent ]->extrudeInner( mExtrudeInnerSel[i].idEntity, mExtrudeInnerInfo[i] );
    }
  }

  //OpInfo::OpExtrudeInner opInfo ( aValue, mExtrudeInnerSel );
}

void Mesh::endOpExtrudeInner()
{
  mStack->queueOperation( OpInfo::OpExtrudeInner ( mExtrudeInnerInfo[0].value,
                                                   mExtrudeInnerSel,
                                                   mExtInnerCurrentOptions,
                                                   storeComponentFlag()) );
  mExtrudeInnerInfo.clear();
  mExtrudeInnerSel.clear();
}

void  Mesh::opExtrudeInner ( const OpInfo::OpExtrudeInner& aOp )
{
  mExtrudeInnerInfo.resize( aOp.sel.size() );
  mExtrudeInnerSel = aOp.sel;
  for ( unsigned int i = 0 ; i < aOp.sel.size(); ++i )
  {
    mExtrudeInnerInfo[i] = mMeshObject[ aOp.sel[i].idComponent ]->
        extrudeInnerStart( aOp.sel[i].idEntity, aOp.options );
  }
  opExtrudeInner( aOp.value );
}

//////


void Mesh::opStartExtrudeMatrix( const std::vector<ObjEd::Selection>& aSel,
                                 Options::ExtMatrixOptions& aOptions)
{
  mExtrudeMatrixSel = aSel;
  mExtrudeMatrixInfo.resize( mExtrudeMatrixSel.size() );
  mExtMatrixCurrentOptions = aOptions;
  if ( eModeFace )
  {
    for ( unsigned int i = 0 ; i < mExtrudeMatrixSel.size(); ++i )
    {
      if ( mExtrudeMatrixSel[i].typeEntity == eModeFace )
      {
        mExtrudeMatrixInfo[i] = mMeshObject[ mExtrudeMatrixSel[i].idComponent ]->
            extrudeMatrixStart( mExtrudeMatrixSel[i].idEntity,
                                aOptions);

      }
    }

    opExtrudeMatrix( 0.0f );
  }
}


void  Mesh::opExtrudeMatrix ( float aValue )
{
  for ( unsigned int i = 0 ; i < mExtrudeMatrixSel.size(); ++i )
  {
    if ( mExtrudeMatrixSel[i].typeEntity == eModeFace )
    {
      mExtrudeMatrixInfo[i].value = aValue;
      mMeshObject[ mExtrudeMatrixSel[i].idComponent ]->extrudeMatrix( mExtrudeMatrixSel[i].idEntity, mExtrudeMatrixInfo[i] );
    }
  }

  //OpInfo::OpExtrudeMatrix opInfo ( aValue, mExtrudeMatrixSel );
}

void Mesh::endOpExtrudeMatrix()
{
  mStack->queueOperation( OpInfo::OpExtrudeMatrix ( mExtrudeMatrixInfo[0].value,
                                                    mExtrudeMatrixSel,
                                                    mExtMatrixCurrentOptions,
                                                    storeComponentFlag()) );
  mExtrudeMatrixInfo.clear();
  mExtrudeMatrixSel.clear();
}

void  Mesh::opExtrudeMatrix ( const OpInfo::OpExtrudeMatrix& aOp )
{
  mExtrudeMatrixInfo.resize( aOp.sel.size() );
  mExtrudeMatrixSel = aOp.sel;
  for ( unsigned int i = 0 ; i < aOp.sel.size(); ++i )
  {
    mExtrudeMatrixInfo[i] = mMeshObject[ aOp.sel[i].idComponent ]->
        extrudeMatrixStart( aOp.sel[i].idEntity, aOp.options );
  }
  opExtrudeMatrix( aOp.value );
}


void Mesh::opStartBevelV( const std::vector<ObjEd::Selection>& aSel )
{
  mBevelVSel = aSel;
  mBevelVInfo.resize( mBevelVSel.size() );

  for ( unsigned int i = 0 ; i < mBevelVSel.size(); ++i )
  {
    // TODOCOMP: should this go?
    if ( mBevelVSel[i].typeEntity == eModeVertex )
    {
      mBevelVInfo[i] = mMeshObject[ mBevelVSel[i].idComponent ]->
          bevelVStart( mBevelVSel[i].idEntity );

    }
  }
}

void  Mesh::opBevelV ( float aValue )
{
  for ( unsigned int i = 0 ; i < mBevelVSel.size(); ++i )
  {
    if ( mBevelVSel[i].typeEntity == eModeVertex )
    {
      mBevelVInfo[i].value = aValue;
      mMeshObject[ mBevelVSel[i].idComponent ]->bevelV( mBevelVSel[i].idEntity, mBevelVInfo[i] );
    }
  }
}

void  Mesh::opBevelV ( const OpInfo::OpBevelV& aOp )
{
  mBevelVSel = aOp.sel;
  mBevelVInfo.resize( mBevelVSel.size() );

  for ( unsigned int i = 0 ; i < mBevelVSel.size(); ++i )
  {
    // TODOCOMP: should this go?
    if ( mBevelVSel[i].typeEntity == eModeVertex )
    {
      mBevelVInfo[i] = mMeshObject[ mBevelVSel[i].idComponent ]->
          bevelVStart( mBevelVSel[i].idEntity );
      mBevelVInfo[i].value = aOp.value;
      mMeshObject[ mBevelVSel[i].idComponent ]->bevelV( mBevelVSel[i].idEntity, mBevelVInfo[i] );
    }
  }
}

///////////////////////////////////////////////////////////////////////
//    BevelE

void Mesh::opStartBevelE( const std::vector<ObjEd::Selection>& aSel )
{

  mBevelESel = aSel;

  mBevelEInfo = mMeshObject[ mBevelESel[0].idComponent ]->
          bevelEStart( mBevelESel );

}

void  Mesh::opBevelE ( float aValue )
{
  mBevelEInfo.value = aValue;
  mMeshObject[ mBevelESel[0].idComponent ]->
          bevelE( mBevelEInfo );
}

void  Mesh::opBevelE ( const OpInfo::OpBevelE& aOp )
{

  mBevelESel = aOp.sel;

  mBevelEInfo = mMeshObject[ mBevelESel[0].idComponent ]->
      bevelEStart( mBevelESel );
  mBevelEInfo.value = aOp.value;
  mMeshObject[ mBevelESel[0].idComponent ]->bevelE( mBevelEInfo );
}

void Mesh::opStartNorScale( const std::vector<ObjEd::Selection>& aSel )
{
  mNorScaleSel = aSel;
  mNorScaleInfo.resize( mNorScaleSel.size() );

  for ( unsigned int i = 0 ; i < mNorScaleSel.size(); ++i )
  {
    mNorScaleInfo[i] = mMeshObject[ mNorScaleSel[i].idComponent ]->
        norScaleStart( mNorScaleSel[i].idEntity );
  }
}

void  Mesh::opNorScale ( float aValue )
{
  for ( unsigned int i = 0 ; i < mNorScaleSel.size(); ++i )
  {
    mNorScaleInfo[i].value = aValue;
    mMeshObject[ mNorScaleSel[i].idComponent ]->norScale( mNorScaleSel[i].idEntity, mNorScaleInfo[i] );
  }
}

void Mesh::opNorScale ( const OpInfo::OpNormScale& aOp )
{
  mNorScaleSel = aOp.sel;
  mNorScaleInfo.resize( mNorScaleSel.size() );

  for ( unsigned int i = 0 ; i < mNorScaleSel.size(); ++i )
  {
    mNorScaleInfo[i] = mMeshObject[ mNorScaleSel[i].idComponent ]->
        norScaleStart( mNorScaleSel[i].idEntity );
  }

  opNorScale( aOp.value );

}

D3DXVECTOR3 Mesh::centerMassSelectedEdges()
{
  std::list<int> vertices = verticesOfEdgeSelection( getCurrentSelection() );

  D3DXVECTOR3 center = D3DXVECTOR3(0,0,0);
  int n = 0;
  for (std::list<int>::iterator it=vertices.begin(); it!=vertices.end(); ++it)
  {
    center += ( mMeshObject[mSelectedComponent]->vertex(*it) ).pos;
    ++n;
  }
  center *= 1.0f / float(n);
  return center;
}

D3DXVECTOR3 Mesh::centerMassSelectedFaces()
{
  std::list<int> vertices = verticesOfFaceSelection( getCurrentSelection() );

  D3DXVECTOR3 center = D3DXVECTOR3(0,0,0);
  int n = 0;
  for (std::list<int>::iterator it=vertices.begin(); it!=vertices.end(); ++it)
  {
    center += ( mMeshObject[mSelectedComponent]->vertex(*it) ).pos;
    ++n;
  }
  center *= 1.0f / float(n);

  return center;
}

D3DXVECTOR3 Mesh::centerMassSelectedObjects()
{
  D3DXVECTOR3 center = D3DXVECTOR3(0,0,0);
  std::vector<ObjEd::Selection> compId =  getCurrentSelection();

  int n = 0;

  for ( unsigned int i = 0; i < compId.size() ; ++i)
    if ( compId[i].typeEntity == eModeObject )
    {
      int componentId = compId[i].idComponent;
      Intersections::BoundingSphere b = mMeshObject[ componentId ]->boundingSphere();
      center += b.center;
      ++n;
    }
  center *= 1.0f / float(n);

  return center;
}

void Mesh::opTranslateEdges(D3DXVECTOR3 aValue,
                            const std::vector<ObjEd::Selection>& aSel)
{
  std::list<int> vertices = verticesOfEdgeSelection( aSel );

  D3DXVECTOR3 value = aValue;
  /*
  if ( hasBasisOfSelection( aSel ) )
  {
    D3DXMATRIX m = DxMath::basisMatrix( basisOfSelection( aSel) );
    D3DXVec3TransformCoord( &value, &value, &m );
  }
  */

  for (std::list<int>::iterator it=vertices.begin(); it!=vertices.end(); ++it)
    mMeshObject[ aSel[0].idComponent ]->opTranslateVertex( *it, value );
}

void Mesh::opTranslateFaces(D3DXVECTOR3 aValue,
                            const std::vector<ObjEd::Selection>& aSel)
{
  std::list<int> vertices = verticesOfFaceSelection( aSel  );

  D3DXVECTOR3 value = aValue;
  /*
  if ( hasBasisOfSelection( aSel ) )
  {
    D3DXMATRIX m = DxMath::basisMatrix( basisOfSelection( aSel) );
    D3DXVec3TransformCoord( &value, &value, &m );
  }
  */

  for (std::list<int>::iterator it=vertices.begin(); it!=vertices.end(); ++it)
    mMeshObject[ aSel[0].idComponent ]->opTranslateVertex( *it, value );
}

void Mesh::opScaleEdges(D3DXVECTOR3 aCenter, D3DXVECTOR3 aValue,
                        const std::vector<ObjEd::Selection>& aSel)
{
  std::list<int> vertices = verticesOfEdgeSelection( aSel );

  for (std::list<int>::iterator it=vertices.begin(); it!=vertices.end(); ++it)
    mMeshObject[ aSel[0].idComponent ]->opScaleVertices( *it, aCenter, aValue );
}

void Mesh::opScaleFaces(D3DXVECTOR3 aCenter, D3DXVECTOR3 aValue,
                        const std::vector<ObjEd::Selection>& aSel)
{
  std::list<int> vertices = verticesOfFaceSelection( aSel );

  for (std::list<int>::iterator it=vertices.begin(); it!=vertices.end(); ++it)
    mMeshObject[ aSel[0].idComponent ]->opScaleVertices( *it, aCenter,aValue );
}


std::list<int> Mesh::verticesOfEdgeSelection( const std::vector<ObjEd::Selection>& aSel)
{
  std::list<int> vertices;
  for ( unsigned int i = 0; i < aSel.size() ; ++i)
  {
    if ( aSel[i].typeEntity == eModeEdge )
    {
      Edge e = mMeshObject[ aSel[0].idComponent ]->edge( aSel[i].idEntity );
      vertices.push_back( e.bId );
      vertices.push_back( e.eId );
    }
  }
  vertices.sort();
  vertices.unique();
  return vertices;
}

std::list<int>  Mesh::verticesOfFaceSelection( const std::vector<ObjEd::Selection>& aSel )
{
  std::list<int> vertices;
  for ( unsigned int i = 0; i < aSel.size() ; ++i)
  {
    if ( aSel[i].typeEntity == eModeFace )
    {
      Face f = mMeshObject[ aSel[0].idComponent ]->face( aSel[i].idEntity );
      for ( int j = 0; j < f.nVertices() ; ++j)
        vertices.push_back( f.vId(j) );

    }
  }

  vertices.sort();
  vertices.unique();
  return vertices;
}



std::vector<int> Mesh::verticesOfFaces( const std::vector<Face>& aFaces )
{
  std::list<int> vertices;
  for ( unsigned int i = 0; i < aFaces.size() ; ++i)
  {    
    for ( int j = 0; j < aFaces[i].nVertices() ; ++j)
      vertices.push_back( aFaces[i].vId(j) );
  }

  vertices.unique();
  vertices.unique();

  std::vector<int> uniqueVertices ( vertices.begin(), vertices.end() );
  return uniqueVertices;
}


void Mesh::opKniveLine(ObjEd::KniveLineInfo &aInfo, D3DXMATRIX& aView, D3DXVECTOR3 aCamPos )
{
  int component = aInfo.entityB.idComponent;  
  mKnifeLineOp  = mMeshObject[component]->kniveLine( aInfo, aView, aCamPos );
  opKnifeLine( mKnifeLineOp );
}

void  Mesh::opKnifeLine ( const OpInfo::OpKnifeLine& aOp  )
{
  int component = aOp.mKnifeLineComponentId;
  mMeshObject[component]->kniveLine( aOp );
}


void Mesh::endOpBevelV()
{
  mStack->queueOperation( OpInfo::OpBevelV ( mBevelVInfo[0].value, mBevelVSel, storeComponentFlag() ) );
  mBevelVInfo.clear();
  mBevelVSel.clear();
}

void Mesh::endOpBevelE()
{
  mStack->queueOperation( OpInfo::OpBevelE ( mBevelEInfo.value, mBevelESel, storeComponentFlag() ) );
  mBevelESel.clear();
}

void Mesh::endOpNorScale()
{
  mStack->queueOperation( OpInfo::OpNormScale ( mNorScaleInfo[0].value, mNorScaleSel, storeComponentFlag() ) );
  mNorScaleInfo.clear();
  mNorScaleSel.clear();
}

void Mesh::endOpKniveLine()
{
  mStack->queueOperation( mKnifeLineOp );
}

void Mesh::opPrimCube( int aMeshComponentId )
{
  mMeshObject[aMeshComponentId]->createCubePrimitive();  
}


void Mesh::endOpPrimCube( int aMeshComponentId )
{
  mStack->queueOperation( OpInfo::OpPrimCube( aMeshComponentId, 1 ) );
}



void Mesh::opPrimCube( const OpInfo::OpPrimCube& aOp )
{
  int id = addComponent( new MeshComponent( nComponents() ));
  mMeshObject[id]->createCubePrimitive();

}


void Mesh::reconstructFromStack()
{
  mSelectedComponent = 0;
  mMeshObject.resize(0);
  mSelO.resize(0);

  mStack->prepareReconstruction();
  for (int i = 0; i < mStack->numOp(); ++i)
  {
    switch( mStack->opId(i) )
    {
    case OpId::ePrimCube:
      opPrimCube( mStack->opPrimCube(i) );
      break;

    case OpId::eExtrude:
      opExtrude( mStack->opExt(i) );
      break;
    case OpId::eExtrudeInner:
      opExtrudeInner( mStack->opExtI(i) );
      break;
    case OpId::eExtrudeMatrix:
      opExtrudeMatrix( mStack->opExtM(i) );
      break;
    case OpId::eNormalScale:
      opNorScale( mStack->opNorS(i) );
      break;

    case OpId::eKniveLine :
      opKnifeLine( mStack->opKniL(i) );
      break;

    case OpId::eBevelV:
      opBevelV( mStack->opBevV(i) );
      break;
    case OpId::eBevelE:
      opBevelE( mStack->opBevE(i) );
      break;

    case OpId::eTranslate:
      opTranslate( mStack->opTrans(i) );
      break;
    case OpId::eScale:
      opScale( mStack->opScale(i) );
      break;
    };
  }
}

MeshInfo Mesh::infoMeshText()
{
  MeshInfo m;
  m.nv = mMeshObject[0]->nVertices();
  m.ne = mMeshObject[0]->nEdges();
  m.nf = mMeshObject[0]->nFaces();

  return m;

}

D3DXVECTOR3 Mesh::centerMassSelectedEntities(const int aEntityType)
{
  switch (aEntityType)
  {
  case eModeVertex:
    return centerMassSelectedVertices();
  case eModeEdge:
    return centerMassSelectedEdges();
  case eModeFace:
    return centerMassSelectedFaces();
  case eModeObject:
    return centerMassSelectedObjects();
  }
}

void Mesh::selectObject( int aObjId )
{
  mSelO[aObjId] = true;
}

void Mesh::unselectObject( int aObjId )
{
  mSelO[aObjId] = false;
}

void Mesh::opTranslateObject(D3DXVECTOR3 aValue,
                             const std::vector<ObjEd::Selection>& aSel)
{  

  for ( unsigned int i = 0; i < aSel.size() ; ++i)
    if ( aSel[i].typeEntity == eModeObject )
    {
      int componentId = aSel[i].idComponent;
      for ( int j = 0; j < mMeshObject[ componentId ]->nVertices() ; ++j)
        mMeshObject[ componentId ]->opTranslateVertex( j, aValue );
    }
}

void Mesh::opScaleObjects(D3DXVECTOR3 aCenter, D3DXVECTOR3 aValue,
                         const std::vector<ObjEd::Selection>& aSel)
{
  std::vector<ObjEd::Selection> compId =  aSel;

  for ( int i = 0; i < compId.size() ; ++i)
    if ( compId[i].typeEntity == eModeObject )
    {
      int componentId = compId[i].idComponent;
      for ( int j = 0; j < mMeshObject[ componentId ]->nVertices() ; ++j)
        mMeshObject[ componentId ]->opScaleVertices( j, aCenter, aValue );
    }
}

void Mesh::unselectObjects()
{
  for (int i = 0; i< nComponents() ; ++i)
  {
    mSelO[i] = false;
  }
}

ObjEd::Selection Mesh::checkObject(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXMATRIX &aView, D3DXVECTOR3 aCamPos)
{
  ObjEd::Selection s;

  s.idComponent = -1;
  s.typeEntity = eModeObject;
  s.idEntity = -1;

  bool  existsMin = false;
  float distanceMin, distance;

  for ( unsigned int i = 0 ; i < nComponents() ; ++i )
  {
    bool update = false;
    Intersections::BoundingSphere bSphere = mMeshObject[i]->boundingSphere();

    bool hit = raySphere( bSphere, aRayB, aRayE );
    if ( hit )
    {
      D3DXVECTOR3 posCS;
      D3DXVec3TransformCoord( &posCS, &(bSphere.center), &aView );
      distance = posCS.z;
    }

    if (!existsMin && hit  ) update = true;
    if ( existsMin && hit && distance<distanceMin ) update = true;

    if (update)
    {
      distanceMin   = distance;
      s.idComponent = i;
      s.idEntity    = i;
    }
    mHighlightedComponent.idEntity = -1;

  }


  //  Write the highlighted one
  if (s.idComponent != -1)
    mHighlightedComponent = s;

  return s;

}

std::vector<int> Mesh::DEBUGlistEntity(ObjEd::Selection sel)
{
  std::vector<int> v;
  if (sel.typeEntity == eModeEdge)
    v = mMeshObject[sel.idComponent]->DEBUGlistEdge( sel );
  if (sel.typeEntity == eModeFace)
    v = mMeshObject[sel.idComponent]->DEBUGlistFace( sel );

  return v;
}

//  \return   component id
int Mesh::addComponent( MeshComponent* aComp )
{
  int index = mMeshObject.size();
  mMeshObject.resize( index + 1);


  mMeshObject[index] = aComp;
  mMeshObject[index]->setId( index );

  mSelO.resize( index + 1 );
  mSelO[index] = false;

  return index;
}

void Mesh::newPrimitive(int aPrim)
{
  int id = addComponent( new MeshComponent( nComponents() ) );

  if ( aPrim == OpId::ePrimCube )
    opPrimCube(id);

  endOpPrimCube( id );
}

int Mesh::nComponents() const
{
  return mMeshObject.size();
}

bool Mesh::isEntitySelected( const ObjEd::Selection &aEntity )
{
  if ( aEntity.typeEntity == eModeObject )
    return mSelO[ aEntity.idComponent ];
  else
    return mMeshObject[aEntity.idComponent]->isEntitySelected( aEntity );
}

void Mesh::release()
{
  delete mStack;
  //mMeshObject.clear();

  for ( int i = 0; i < nComponents(); ++i)
    delete mMeshObject[i];
  mMeshObject.clear();


  mTransSel.clear();
  mScaleSel.clear();
  mBevelVSel.clear();
  mBevelVInfo.clear();
  mBevelESel.clear();
  mExtrudeSel.clear();
  mExtrudeInfo.clear();
  mExtrudeMatrixSel.clear();
  mExtrudeMatrixInfo.clear();
  mExtrudeInnerSel.clear();
  mExtrudeInnerInfo.clear();
  mNorScaleSel.clear();
  mNorScaleInfo.clear();

  mSelO.clear();
  mTextures.clear();
}

void Mesh::init()
{
  mStack = new OpStack();  
  setMode( eModeFace );

  mTextures.clear();
}

int Mesh::storeComponentFlag()
{
  return OpInfo::eIdComponentStored;
  /*
  if ( nComponents() <= 1)
    return OpInfo::eIdComponentNotStored;
  else
    return OpInfo::eIdComponentStored;
    */
}

bool Mesh::hasBasisOfSelection(std::vector<ObjEd::Selection> aSel)
{
  if ( aSel.size() != 1)
    return false;
  return ( aSel[0].typeEntity == eModeEdge || aSel[0].typeEntity == eModeFace );
}

std::vector<D3DXVECTOR3> Mesh::basisOfSelection(std::vector<ObjEd::Selection> aSel)
{
  std::vector<D3DXVECTOR3> basis;

  if( aSel[0].typeEntity == eModeEdge )
    basis = mMeshObject[ aSel[0].idComponent ]->basisOfEdge ( aSel[0].idEntity );

  if( aSel[0].typeEntity == eModeFace )
    basis = mMeshObject[ aSel[0].idComponent ]->basisOfFace ( aSel[0].idEntity );


  return basis;

}

bool Mesh::hasProcTex()
{
  return !mTextures.empty();
}

void Mesh::initProcTex()
{
  mTextures.resize(1);
  mTextures[0].name   = "default.fx";
  mTextures[0].scale  = 1.0f;
  mTextures[0].col    = D3DXCOLOR( 1,0,0,1 );
  for (int i = 0; i < mMeshObject.size(); ++i)
    mMeshObject[i]->initTex();
}

void Mesh::setProcTex(const std::vector<ObjEd::ProcTex> &aProcTex)
{
  mTextures = aProcTex;
}

std::vector<ObjEd::ProcTex> Mesh::procTex() const
{
  return mTextures;
}

void Mesh::opTexAddToSelection(const int aCompId, const int aFaceId, const int aTexId)
{
  mMeshObject[ aCompId ]->setTex ( aFaceId, aTexId );
}

void Mesh::newProcTex()
{
  int index = mTextures.size();
  mTextures.resize( index + 1);
  mTextures[index].name   = "default.fx";
  mTextures[index].scale  = 1.0f;
  mTextures[index].col    = D3DXCOLOR( 1,0,0,1 );
}
