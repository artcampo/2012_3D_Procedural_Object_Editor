#include "Geometry/DirGraph/Pattern.hpp"

namespace DirectedGraph {

void  initPatternOutEdges ( DirectedGraph::PatternOutNode* aPattern )
{
  aPattern->mEdgesCreated = false;

  for ( int i = 0; i < aPattern->mDesc.size() ; ++i )
    initPatternOutEdges( aPattern->mDesc[i] );
}

bool PatternOut::passesRequirements(  float aLength, 
                              const TrackBp::Vertex& aVertBeg,
                              const TrackBp::Vertex& aVertEnd  )
{
  bool valid = true;

  valid &= (aLength > mMinLength);
  valid &= (aLength < mMaxLength);

  return valid;
}

bool PatternIn::passesRequirements(  float aLength, 
                              const TrackBp::Vertex& aVertBeg,
                              const TrackBp::Vertex& aVertEnd )
{
  bool valid = true;

  valid &= (aLength > mMinLength);
  valid &= (aLength < mMaxLength);
  
  return valid;
}

void PatternOut::createTangents()
{
  for ( int i = 0; i < mRoot->mDesc.size(); ++i )
    createTangentsRec( mRoot->mDesc[i], mRoot->mTan, mRoot->mNor );
}

void PatternOut::createTangentsRec( PatternOutNode* aNode, 
                                    const D3DXVECTOR3& aParentTan,
                                    const D3DXVECTOR3& aParentNor )
{
  if (aNode->mDesc.size() == 0 )
    return;

  aNode->mAccumulatedParentsTangent += aParentTan;
  aNode->mAccumulatedParentsNormal  += aParentNor;
  aNode->mNumParents++;

  for ( int i = 0; i < aNode->mDesc.size(); ++i )
    createTangentsRec( aNode->mDesc[i], aNode->tangent(), aNode->normal() );
}

D3DXVECTOR3 PatternOutNode::tangent() const
{
  if( mTangentAndNormal )
    return mTan;
  else
    return (1.0f/float(mNumParents))*mAccumulatedParentsTangent;
}

D3DXVECTOR3 PatternOutNode::normal() const
{
  if( mTangentAndNormal )
    return mNor;
  else
    return (1.0f/float(mNumParents))*mAccumulatedParentsNormal;
}

void PatternOut::createBranches()
{
  mEdges.push_back( &mVertexAEdge );
  mEdges.push_back( &mVertexBEdge );

  createEdgeArraysRec( mRoot );
  initPatternOutEdges( mRoot );

  std::vector<int> branch;
  createBranchesRec( mRoot, branch, 0 );

  selectEdgesToUpdate();
}

void PatternOut::selectEdgesToUpdate()
{
  for ( int i = 0; i < mEdges.size() ; ++i )
  {
    bool foundInAllBranches = true;
    for ( int j = 0; foundInAllBranches && j < mBranches.size() ; ++j )
      foundInAllBranches &= StlHelpers::isInVector( mBranches[j].mEdgeUsed, i );

    if ( !foundInAllBranches )
      for ( int j = 0; j < mBranches.size() ; ++j )
        if ( StlHelpers::isInVector( mBranches[j].mEdgeUsed, i ) )
          mBranches[j].mEdgesToUpdate.push_back( i );

  }
  
}


void PatternOut::createEdgeArraysRec( PatternOutNode* aVertex )
{
  aVertex->mEdges.resize( aVertex->mDesc.size() );
  for ( int i = 0; i < aVertex->mDesc.size() ; ++i )
  {
    
    createEdgeArraysRec( aVertex->mDesc[i] );
  }

}


int PatternOut::createBranchesRec( PatternOutNode* aVertex, std::vector<int>& aBranchEdges, int aBranchIndex )
{  
  //  Base case

  if( aVertex->mDesc.size() == 0 )
  {
    mBranches.resize( aBranchIndex + 1 );
    mBranches[aBranchIndex].mEdgeUsed.push_back( 0 );
    mBranches[aBranchIndex].mEdgeUsed.push_back( 1 );
    for ( int i = 0; i < aBranchEdges.size() ; ++i )
      mBranches[aBranchIndex].mEdgeUsed.push_back( aBranchEdges[i] );
    return aBranchIndex;
  }
 
  int currentBranchIndex = aBranchIndex;

  if( aVertex->mDesc.size() == 1 )
  {
    int i = 0;
    int index;
    if ( ! aVertex->mEdgesCreated )
    {
      aVertex->mEdgesCreated = true;
      for ( int j = 0; j < aVertex->mDesc.size() ; ++j )
      {
        index = mEdges.size();        
        mEdges.push_back( &aVertex->mEdges[j] );            
        aVertex->mEdgesId.push_back( index );
      }
      index = aVertex->mEdgesId[0];
    }
    else
      index = aVertex->mEdgesId[i];

    aBranchEdges.push_back( index );

    if ( i == 0 )
    {      
      currentBranchIndex = createBranchesRec( aVertex->mDesc[0], aBranchEdges, currentBranchIndex );
    }
    return currentBranchIndex;
  }

  //  Make copies of current branch
  std::vector< std::vector<int> > branchEdges; 
  branchEdges.resize( aVertex->mDesc.size() );
  for ( int i = 0; i < aVertex->mDesc.size() ; ++i )  
    branchEdges[i] = aBranchEdges;
  
  for ( int i = 0; i < aVertex->mDesc.size() ; ++i )
  {
    int index;
    if ( ! aVertex->mEdgesCreated )
    {
      aVertex->mEdgesCreated = true;
      for ( int j = 0; j < aVertex->mDesc.size() ; ++j )
      {
        index = mEdges.size();
        
        mEdges.push_back( &aVertex->mEdges[j] );            
        aVertex->mEdgesId.push_back( index );
      }
      index = aVertex->mEdgesId[0];
    }
    else
      index = aVertex->mEdgesId[i];

    branchEdges[i].push_back( index );
    if ( i == 0)
      currentBranchIndex = createBranchesRec( aVertex->mDesc[0], branchEdges[i], currentBranchIndex );       
    else
      currentBranchIndex = createBranchesRec( aVertex->mDesc[i], branchEdges[i], currentBranchIndex + 1 );       
  }

  return currentBranchIndex;
}

};
