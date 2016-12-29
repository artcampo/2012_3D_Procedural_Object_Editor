#include "MeshHalfEdgeSerial.hpp"
#include "ObjEditor/Mesh/HalfEdge/HE_edge.hpp"
#include "ObjEditor/Mesh/HalfEdge/HE_vert.hpp"
#include "ObjEditor/Mesh/HalfEdge/HE_face.hpp"

HE_meshSerial::HE_meshSerial( const HE_mesh& rhs )
{
  mHalfEdges.resize         ( rhs.mHalfEdges.size() );
  mFaces.resize             ( rhs.mFaces.size() );
  mVertices.resize          ( rhs.mVertices.size() );
  mHalfEdgeOfFullEdge.resize( rhs.mHalfEdgeOfFullEdge.size() );
  mFullEdgeofHalfEdge.resize( rhs.mFullEdgeofHalfEdge.size() );

  for ( int i = 0; i < mHalfEdges.size() ; ++i)
  {
    mHalfEdges[i].mVert = rhs.serialIdv( rhs.mHalfEdges[i]->mVert );
    mHalfEdges[i].mFace = rhs.serialIdf( rhs.mHalfEdges[i]->mFace );
    mHalfEdges[i].mPair = rhs.serialIde( rhs.mHalfEdges[i]->mPair );
    mHalfEdges[i].mNext = rhs.serialIde( rhs.mHalfEdges[i]->mNext );
    mHalfEdges[i].mPrev = rhs.serialIde( rhs.mHalfEdges[i]->mPrev );
    mHalfEdges[i].mId   = rhs.mHalfEdges[i]->mId;
  }

  for ( int i = 0; i < mFaces.size() ; ++i)
  {
    mFaces[i].mEdge = rhs.serialIde( rhs.mFaces[i]->mEdge );
    mFaces[i].mId   = rhs.mFaces[i]->mId;
  }

  for ( int i = 0; i < mVertices.size() ; ++i)
  {
    mVertices[i].mEdge    = rhs.serialIde( rhs.mVertices[i]->mEdge );
    mVertices[i].mVertex  = rhs.mVertices[i]->mVertex;
    mVertices[i].mId      = rhs.mVertices[i]->mId;
  }

  for ( int i = 0; i < mHalfEdgeOfFullEdge.size() ; ++i)
    mHalfEdgeOfFullEdge[i] = (int)rhs.serialIde( rhs.mHalfEdgeOfFullEdge[i] );

  for ( int i = 0; i < mFullEdgeofHalfEdge.size() ; ++i)
  {
    mFullEdgeofHalfEdge[i] = rhs.mFullEdgeofHalfEdge[i];
  }
}
