#ifndef _Pattern_HPP
#define _Pattern_HPP

//#include <algorithm>
#include <vector>

#include "GameBall/TrackBp.hpp"
#include "Geometry/DirGraph/DirGraphDataTypes.hpp"
#include "Misc/StlHelpers.hpp"
#include "Geometry/DxMathHelpers.hpp"

namespace DirectedGraph {  

  class PatternInNode;
  class GeomNode;
  class Branch;
  class PatternOutNode;
  class PatternOut;
  class PatternIn;

  ///////////////////////////////////////////////////////////////
  //PatternInNode
  ///////////////////////////////////////////////////////////////
  class PatternInNode{

  public:
    int   degIn;
    int   degOut;
    std::vector<PatternInNode> mDesc;

  public:
    PatternInNode (){};
    PatternInNode ( const int aDegIn, const int aDegOut )
    {
      degIn   = aDegIn;
      degOut  = aDegOut;
    }
    PatternInNode ( const int aDegIn, const int aDegOut, 
                  const std::vector<PatternInNode>& aDesc )
    {
      degIn   = aDegIn;
      degOut  = aDegOut;
      mDesc   = aDesc;
    }
    PatternInNode ( const int aDegIn, const int aDegOut, 
                  const PatternInNode& aDesc )
    {
      degIn   = aDegIn;
      degOut  = aDegOut;
      mDesc.clear();
      mDesc.push_back( aDesc );
    }
  };

  ///////////////////////////////////////////////////////////////
  //GeomNode
  ///////////////////////////////////////////////////////////////
  class GeomNode{
  public:
    GeomNode (){};
    GeomNode( const float aTanComponent, 
              const float aBinComponent,
              const float aNorComponent)
    {
      mTanComponent  = aTanComponent;
      mBinComponent  = aBinComponent;
      mNorComponent  = aNorComponent;
    }
  public:
    float mTanComponent;
    float mBinComponent;
    float mNorComponent;
  };

  ///////////////////////////////////////////////////////////////
  //Branch
  ///////////////////////////////////////////////////////////////
  class Branch{
  public:

  public:    
    std::vector<int>  mEdgeUsed;      // ids of edges used by this branch 
    std::vector<int>  mEdgesToUpdate; // ids of edges that have to be updated    
  };

  ///////////////////////////////////////////////////////////////
  //PatternOutNode
  ///////////////////////////////////////////////////////////////
  class PatternOutNode{
  public:
    //  Static data, at load time
    GeomNode  mGeomRules;
    bool        mTangentAndNormal;
    D3DXVECTOR3 mTan;
    D3DXVECTOR3 mNor;
    std::vector<PatternOutNode*>    mDesc;

    //  Dynamic data, for a current substitution
    bool      mVertexInGraphCreated;
    int       mVertexInGraphId;
    bool      mEdgesCreated;    
    std::vector<DirectedGraph::Edge>  mEdges;     // Edge, vertices corresponding to graph
                                                  // (not to this substitution!)    
    std::vector<int>                  mEdgesId;   // Edges id on branch
    //  temporal init data
  public:
    D3DXVECTOR3   mAccumulatedParentsTangent;
    D3DXVECTOR3   mAccumulatedParentsNormal;
    int           mNumParents;
  public:
    D3DXVECTOR3 tangent() const;
    D3DXVECTOR3 normal() const;
    PatternOutNode (){};
    PatternOutNode (  const GeomNode& aGeomRules )
    {
      mGeomRules        = aGeomRules;   
      mTangentAndNormal = false;
      mAccumulatedParentsTangent  = D3DXVECTOR3(0,0,0);
      mAccumulatedParentsNormal   = D3DXVECTOR3(0,0,0);
      mNumParents                 = 0;
    }
    PatternOutNode (  const GeomNode& aGeomRules, 
                      const D3DXVECTOR3& aTan,
                      const D3DXVECTOR3& aNor)
    {
      mGeomRules        = aGeomRules;   
      mTangentAndNormal = true;
      mTan              = aTan;
      mNor              = aNor;
    }

    PatternOutNode (  const GeomNode& aGeomRules, 
                      const std::vector<PatternOutNode*>& aDesc)
    {
      mGeomRules  = aGeomRules;      
      mDesc       = aDesc;     
      mTangentAndNormal = false;
      mAccumulatedParentsTangent  = D3DXVECTOR3(0,0,0);
      mAccumulatedParentsNormal   = D3DXVECTOR3(0,0,0);
      mNumParents                 = 0;
    }
    PatternOutNode (  const GeomNode& aGeomRules, 
                      const D3DXVECTOR3& aTan,
                      const D3DXVECTOR3& aNor,
                      const std::vector<PatternOutNode*>& aDesc)
    {
      mGeomRules  = aGeomRules;      
      mDesc       = aDesc;     
      mTangentAndNormal = true;
      mTan              = aTan;
      mNor              = aNor;
    }

    PatternOutNode (  const GeomNode& aGeomRules, 
                      PatternOutNode* aDesc )
    {
      mGeomRules  = aGeomRules;      
      mDesc.clear();
      mDesc.push_back( aDesc );
      mTangentAndNormal = false;
      mAccumulatedParentsTangent  = D3DXVECTOR3(0,0,0);
      mAccumulatedParentsNormal   = D3DXVECTOR3(0,0,0);
      mNumParents                 = 0;
    }
    PatternOutNode (  const GeomNode& aGeomRules, 
                      const D3DXVECTOR3& aTan,
                      const D3DXVECTOR3& aNor,
                      PatternOutNode* aDesc)
    {
      mGeomRules  = aGeomRules;      
      mDesc.clear();
      mDesc.push_back( aDesc );
      mTangentAndNormal = true;
      mTan              = aTan;
      mNor              = aNor;
    }
  };

  ///////////////////////////////////////////////////////////////
  //PatternOut
  ///////////////////////////////////////////////////////////////
  class PatternOut{
  public:
    PatternOut( PatternOutNode* aRoot, 
                const float aMinLength,
                const float aMaxLength,
                const int aOutPatternId )
    {
      mRoot         = aRoot;
      mOutPatternId = aOutPatternId;
      mMinLength    = aMinLength;
      mMaxLength    = aMaxLength;
      createBranches();
      createTangents();
    }
    bool passesRequirements(  float aLength, 
                              const TrackBp::Vertex& aVertBeg,
                              const TrackBp::Vertex& aVertEnd );

  public:
    //  Static data, at load time
    PatternOutNode*     mRoot;
    int                 mOutPatternId;    
    std::vector<Branch> mBranches;  
    float         mMinLength;
    float         mMaxLength;
  public:
    //  Data used while substituting
    std::vector<Edge*>  mEdges;     // pointers to this current subsitution's edges  
    float               mOriginalDesiredLength;
    DirectedGraph::Edge mVertexAEdge;   //  fake starting vertex
    DirectedGraph::Edge mVertexBEdge;   //  fake ending vertex
  private:
    //  compute branches of this pattern
    void createBranches();
    int  createBranchesRec  ( PatternOutNode* aVertex, std::vector<int>& aBranchEdges, int aBranchIndex );
    void createEdgeArraysRec( PatternOutNode* aVertex );
    void selectEdgesToUpdate();
    // compute tangents of internal vertices
    void createTangents();
    void createTangentsRec( PatternOutNode* aNode, 
                            const D3DXVECTOR3& aParentTan,
                            const D3DXVECTOR3& aParentNor );    
  };

  void  initPatternOutEdges ( PatternOutNode* aPattern );

  

  ///////////////////////////////////////////////////////////////
  //PatternIn
  ///////////////////////////////////////////////////////////////

  class PatternIn{
  public:
    PatternIn( const PatternInNode& aMatch,
              const float aMinLength,
              const float aMaxLength,
              const int aInPatternId )
    {
      mMatch        = aMatch;
      mMinLength    = aMinLength;
      mMaxLength    = aMaxLength;
      mInPatternId = aInPatternId;
    }
    bool passesRequirements(  float aLength, 
                              const TrackBp::Vertex& aVertBeg,
                              const TrackBp::Vertex& aVertEnd );
  public:
    PatternInNode mMatch;
    float         mMinLength;
    float         mMaxLength;
    int           mInPatternId;
  };

};  // end namespace DirectedGraph


#endif // _Pattern_HPP
