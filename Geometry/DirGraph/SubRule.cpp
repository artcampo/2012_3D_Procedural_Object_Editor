#include "Geometry/DirGraph/SubRule.hpp"
#include "Noise/RandomNumberGenerator.hpp"

namespace DirectedGraph{
/*
  DirectedGraph::RuleMatch
          matchPatternRec(  const DirectedGraph::DirGraph& aGraph, 
                            const DirectedGraph::PatternIn& aPatternIn,
                            const int aStartingVertex )
  {
  }
*/
  float distance(  const DirectedGraph::DirGraph& aGraph,
                            const int aVid1, 
                            const int aVid2 )
  {

    TrackBp::Vertex v1 = aGraph.vertex( aVid1 );
    TrackBp::Vertex v2 = aGraph.vertex( aVid2 );

    D3DXVECTOR3 d = v1.pos - v2.pos;
    return D3DXVec3Length( &d );
  }

  DirectedGraph::RuleMatch
          matchPatternREC(  const DirectedGraph::DirGraph& aGraph,
                            DirectedGraph::PatternIn& aPatternIn,
                            DirectedGraph::PatternInNode const* aCurrentNode,
                            const int aStartingVertex,
                            const int aCurrentVertex,
                            DirectedGraph::PatternOutNode* aOutCurrentNode,
                            DirectedGraph::PatternOut* aOutcomingPattern )
  {

    DirectedGraph::RuleMatch m;
    m.matched = false;                

    if (  !(  aGraph.numPar ( aCurrentVertex ) == aCurrentNode->degIn &&
              aGraph.numDesc( aCurrentVertex ) == aCurrentNode->degOut) )
    {
      // no match for whole pattern      
    }    
    else
    {
      //local match
      if ( aCurrentNode->mDesc.size() == 0 )
      {
        //  last node reached      
        float distanceBegEnd = distance( aGraph, aStartingVertex, aCurrentVertex );
        TrackBp::Vertex vBeg = aGraph.vertex( aStartingVertex );
        TrackBp::Vertex vEnd = aGraph.vertex( aCurrentVertex );

        if ( distanceBegEnd > aPatternIn.mMinLength &&
             aOutcomingPattern->passesRequirements( distanceBegEnd, vBeg, vEnd ) )
        {
          m.matched           = true;                                
          m.lastVertex        = aCurrentVertex;
          
        }
        //  terminal node reached
        else
        {                
        }
      }
      else
      {
        //  test branches        
        bool allSubranchesMatched = true;
        DirectedGraph::RuleMatch matchBranch;                       
        std::vector<int> v = aGraph.descendants( aCurrentVertex );
        for ( int i = 0; allSubranchesMatched && i < aCurrentNode->mDesc.size() ; ++i )
        {
          
          matchBranch = matchPatternREC(  aGraph,
                                aPatternIn,
                                &aCurrentNode->mDesc[i],
                                aStartingVertex,
                                v[i],
                                aOutCurrentNode->mDesc[i],
                                aOutcomingPattern); 
          allSubranchesMatched = (matchBranch.matched == true);
        }
        if ( allSubranchesMatched )
        {
          m = matchBranch;
        }
        
      }
    }
  
    return m;
  }

  DirectedGraph::RuleMatch
          matchPattern( const DirectedGraph::DirGraph& aGraph, 
                        DirectedGraph::PatternIn& aPatternIn,
                        DirectedGraph::PatternOut* aOutPatternNode,
                        const int aStartingVertex )
  {    
    DirectedGraph::PatternInNode* currentNode = &aPatternIn.mMatch;
    int currentVertex                         = aStartingVertex;

    DirectedGraph::RuleMatch m;
    m  = matchPatternREC( aGraph, 
                            aPatternIn,
                            currentNode, 
                            aStartingVertex, 
                            currentVertex,
                            aOutPatternNode->mRoot,
                            aOutPatternNode);
    m.outcomingPattern = aOutPatternNode->mOutPatternId;
    return m;
  }

  int    applyRuleCreateNodeRec(  DirectedGraph::DirGraph& aGraph,
                                  DirectedGraph::PatternOut* aOutPattern,
                                  DirectedGraph::PatternOutNode* aOutPatternNode,
                                  const TrackBp::Vertex& aVerBeg,
                                  const int aEndVertex,
                                  const D3DXVECTOR3& aTan,
                                  const D3DXVECTOR3& aBin,
                                  const float aLength )
  {    
    using namespace Rand;
    RandGen r(42);

    D3DXVECTOR3 nor;
    D3DXVec3Cross( &nor, &aBin, &aTan );

    float tanLength = aLength*aOutPatternNode->mGeomRules.mTanComponent* 0.2 * (r.randFloat_0_1() - 0.5f )
                      + aLength*aOutPatternNode->mGeomRules.mTanComponent;
    float binLength = aLength*aOutPatternNode->mGeomRules.mBinComponent* 0.2 * (r.randFloat_0_1() - 0.5f )
                      + aLength*aOutPatternNode->mGeomRules.mBinComponent;
    float norLength = aLength*aOutPatternNode->mGeomRules.mNorComponent* 0.2 * (r.randFloat_0_1() - 0.5f )
                      + aLength*aOutPatternNode->mGeomRules.mNorComponent;


    D3DXVECTOR3 pos = aVerBeg.pos
                      + tanLength*aTan
                      + binLength*aBin
                      - norLength*nor;    

    D3DXVECTOR3 nTan = DxMath::alignToTangents( aOutPattern->mRoot->tangent(), 
                                                aOutPatternNode->tangent(), 
                                                aVerBeg.tan );

    D3DXVECTOR3 nNor = DxMath::alignToTangents( aOutPattern->mRoot->tangent(), 
                                                aOutPatternNode->tangent(), 
                                                aVerBeg.nor );

    TrackBp::Vertex n ( pos, nTan, nNor );
    
    

    bool recurseChildren = false;
    int currentVertex;
    if ( aOutPatternNode->mVertexInGraphCreated )
      currentVertex = aOutPatternNode->mVertexInGraphId;
    else
    {
      currentVertex = aGraph.newNode( n );
      aOutPatternNode->mVertexInGraphCreated  = true;
      aOutPatternNode->mVertexInGraphId       = currentVertex;      
      recurseChildren = true;
    }

  
    if ( aOutPatternNode->mDesc.size() > 0  )
    {
      if ( recurseChildren )
      {
        //  Node has children, create them
        aOutPatternNode->mEdgesCreated = true;
        std::vector<int> descendants ( aOutPatternNode->mDesc.size() );
        for ( int i = 0; i < aOutPatternNode->mDesc.size() ; ++i )
        {      
          DirectedGraph::PatternOutNode* currentSubs = aOutPatternNode->mDesc[i];
          descendants[i]= applyRuleCreateNodeRec( aGraph,
                                aOutPattern,
                                currentSubs,
                                aVerBeg,
                                aEndVertex,
                                aTan,
                                aBin,
                                aLength);          
        }    

        for ( int i = 0; i < aOutPatternNode->mDesc.size() ; ++i )
        {
          aGraph.newEdge( currentVertex, descendants[i], 0 );
          aOutPatternNode->mEdges[i] = Edge( currentVertex, descendants[i], TrackBp::EdgeData( 0 ) );
        }
      }
    }
    else
    {
      //  Vertex is terminal, create edge to graph
      aGraph.newEdge( currentVertex, aEndVertex, 0 );
      aOutPattern->mVertexBEdge = Edge( currentVertex, aEndVertex, TrackBp::EdgeData( 0 ) );
    }

    return currentVertex;
  }

  void    applyRule(  DirectedGraph::DirGraph& aGraph,
                      DirectedGraph::PatternOut* aOutPattern,
                      const int aStartingVertex,
                      const int aEndVertex )
  {
    TrackBp::EdgeData e = aGraph.edge( aStartingVertex, aEndVertex );
    aOutPattern->mOriginalDesiredLength = e.mDesiredLength;
    aGraph.deleteDescendants( aStartingVertex );
    aGraph.deleteParents    ( aEndVertex );
    
    bool finished = false;

    TrackBp::Vertex vb = aGraph.vertex( aStartingVertex );
    TrackBp::Vertex ve = aGraph.vertex( aEndVertex );

    D3DXVECTOR3 tan = ve.pos - vb.pos;
    float l = D3DXVec3Length( &tan );
    D3DXVec3Normalize( &tan, &tan );
    D3DXVECTOR3 bin;
    D3DXVec3Cross( &bin, &tan, &vb.nor );
    D3DXVec3Normalize( &bin, &bin );

    DirectedGraph::PatternOutNode* currentSubs = aOutPattern->mRoot;

    int n0 = applyRuleCreateNodeRec( aGraph,
                            aOutPattern,
                            currentSubs,
                            vb,
                            aEndVertex,
                            tan,
                            bin,
                            l);

    aGraph.newEdge( aStartingVertex, n0, 0 );    
    aOutPattern->mVertexAEdge = Edge( aStartingVertex, n0, TrackBp::EdgeData( 0 ) );   
  }

  void    balanceBranchesLengths( DirectedGraph::DirGraph& aGraph,
                                  DirectedGraph::PatternOut* aOutPattern )
  {
  }

  /*
    Resets the created Ids in graph.
  */
  void  initPatternOut ( DirectedGraph::PatternOutNode* aPattern )
  {
    aPattern->mVertexInGraphCreated = false;

    for ( int i = 0; i < aPattern->mDesc.size() ; ++i )
      initPatternOut( aPattern->mDesc[i] );

  }


}
