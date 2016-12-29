#include "Geometry/DirGraph/DirGraph.hpp"

namespace DirectedGraph{

////////////////////////////////////////////////////////
//  modifying functions
///////////////////////////////////////////////////////

void  DirGraph::setRoot( const int aIdv )
{
  mRoot = aIdv;
}

int   DirGraph::newEdge( const int aIdv1, const int aIdv2, const TrackBp::EdgeData& aEdge )
{
  bool found = false;
  for( int i = 0; i < mVertices[aIdv1].desc.size() ; ++i )
      if ( mVertices[aIdv1].desc[i] == aIdv2 ) found = true;

  if ( !found )
  {
    
    mVertices[aIdv1].desc.push_back ( aIdv2 );
    mVertices[aIdv1].edgeData.push_back ( aEdge );
    mVertices[aIdv2].par.push_back  ( aIdv1 );  
  }

  return 0;
}

int   DirGraph::newNode( const TrackBp::Vertex& aData )
{
  int index = mVerticesData.size();
  mVerticesData.push_back( aData );
  mVertices.resize( mVerticesData.size() );
  return index;
}
/*
void  DirGraph::setRoot( const int aIdv1 )
{
}

void  DirGraph::setDescendants  ( const int aIdv, std::vector<int>& aDesc )
{
  mVertices[aIdv].desc = aDesc;
}

void  DirGraph::setParents      ( const int aIdv, std::vector<int>& aPar )
{
  mVertices[aIdv].par = aPar;
}
*/

void  DirGraph::deleteDescendants ( const int aIdv )
{
  for ( int i = 0; i < mVertices[aIdv].desc.size() ; ++i )
    deleteParent( mVertices[aIdv].desc[i], aIdv );
  mVertices[aIdv].desc.clear();
  mVertices[aIdv].edgeData.clear();
}

void  DirGraph::deleteParents     ( const int aIdv )
{
  for ( int i = 0; i < mVertices[aIdv].par.size() ; ++i )
    deleteDescendant( mVertices[aIdv].par[i], aIdv );
  mVertices[aIdv].par.clear();
}

void  DirGraph::deleteDescendant ( const int aIdv1, const int aIdv2 )
{
  for ( int i = 0; i < mVertices[aIdv1].desc.size() ; ++i )
    if ( mVertices[aIdv1].desc[i] == aIdv2 )
    {
      mVertices[aIdv1].desc.erase( mVertices[aIdv1].desc.begin() + i);
      mVertices[aIdv1].edgeData.erase( mVertices[aIdv1].edgeData.begin() + i);
      return;
    }    
}

void  DirGraph::deleteParent ( const int aIdv1, const int aIdv2 )
{
  for ( int i = 0; i < mVertices[aIdv1].par.size() ; ++i )
    if ( mVertices[aIdv1].par[i] == aIdv2 )
    {
      mVertices[aIdv1].par.erase( mVertices[aIdv1].par.begin() + i);
      return;
    }    
}

////////////////////////////////////////////////////////
//  consulting functions
///////////////////////////////////////////////////////

std::vector<Edge> DirGraph::edges() const
{
  std::vector<Edge> edges;
  edgesRec( 0, edges );
  return edges;
}

void DirGraph::edgesRec( const int aIdv, std::vector<Edge>& aEdges ) const
{
  for ( int i = 0; i < mVertices[aIdv].desc.size() ; ++i )
  {
    Edge e ( aIdv, mVertices[aIdv].desc[i], mVertices[aIdv].edgeData[i] );
    bool found = false;
    for( int i = 0; i < aEdges.size() ; ++i )
      if ( aEdges[i] == e ) found = true;
    if ( !found )
      aEdges.push_back( e );
  }

  for ( int i = 0; i < mVertices[aIdv].desc.size() ; ++i )
    edgesRec( mVertices[aIdv].desc[i], aEdges );
}

TrackBp::Vertex     DirGraph::vertex( const int aIdv ) const
{
  return mVerticesData[aIdv];
}



int               DirGraph::root() const
{
  return mRoot;
}

int               DirGraph::numVertices() const
{
  return mVertices.size();
}

int               DirGraph::numOutEdges( const int aIdv ) const
{
  return mVertices[aIdv].edgeData.size();
}

TrackBp::EdgeData          DirGraph::edge    ( const int aIdv1, const int aIdv2 ) const
{
  int index = 0;
  if ( !StlHelpers::findInVector( mVertices[aIdv1].desc, aIdv2, index ) )
    index = 0;  //TODO: say error
  
  return mVertices[aIdv1].edgeData[index];
}

void          DirGraph::setEdge    ( const int aIdv1, const int aIdv2, const TrackBp::EdgeData& aData  )
{
  int index = 0;
  if ( !StlHelpers::findInVector( mVertices[aIdv1].desc, aIdv2, index ) )
    return;  //TODO: say error
  
  mVertices[aIdv1].edgeData[index] = aData;
}

std::vector<int>  DirGraph::descendants ( const int aIdv ) const
{
  return mVertices[aIdv].desc;
}

std::vector<int>  DirGraph::parents     ( const int aIdv ) const
{
  return mVertices[aIdv].par;
}

int               DirGraph::numDesc ( const int aIdv ) const
{
  return mVertices[aIdv].desc.size();
}

int               DirGraph::numPar  ( const int aIdv ) const
{
  return mVertices[aIdv].par.size();
}

}

void DirectedGraph::DirGraph::setVertexData(const int aIdv, const D3DXVECTOR3 &aPos)
{
  mVerticesData[aIdv].pos = aPos;
}

void DirectedGraph::DirGraph::deleteVertex(const int aIdv)
{
  deleteParents     ( aIdv );
  deleteDescendants ( aIdv );
  mVertices.erase     ( mVertices.begin() + aIdv );
  mVerticesData.erase ( mVerticesData.begin() + aIdv );
}

void DirectedGraph::DirGraph::deleteEdge(const int aIdv1, const int aIdv2)
{
  deleteDescendant  ( aIdv1, aIdv2 );
  deleteParent      ( aIdv2, aIdv1 );
}


DirectedGraph::DirGraph* DirectedGraph::DirGraph::load( QDataStream &ins, QString& aDirGraphName )
{
  int numBlocks;
  QString s,t;
  ins >> s;

  if (s != QString ("Not Gordian Editor v0.2"))
    return NULL;

  ins >> numBlocks;
  ins >> t;

  if (t != QString ("DirGraph Block"))
    return NULL;

  ins >> aDirGraphName;

  DirectedGraph::DirGraph* graph = new DirectedGraph::DirGraph();

  int   _i;
  float _f;
  ins >> _i; graph->mRoot = _i;


  int n;
  ins >> n;
  graph->mVertices.resize( n );
  graph->mVerticesData.resize( n );

  for ( int i = 0; i < graph->mVertices.size() ; ++i )
  {
    ins >> _f; (float) graph->mVerticesData[i].pos.x = _f;
    ins >> _f; (float) graph->mVerticesData[i].pos.y = _f;
    ins >> _f; (float) graph->mVerticesData[i].pos.z = _f;

    int k;
    ins >> k;

    for ( int j = 0; j < k ; ++j )
    {
      int descendant;
      ins >> descendant;
      graph->newEdge( i, descendant, TrackBp::EdgeData() );
    }

  }

  return graph;
}

void DirectedGraph::DirGraph::save       (QDataStream &outs, QString& aDirGraphName )
{
  QString title ("Not Gordian Editor v0.2");
  outs << title;
  outs << (int)1;

  QString meshBlock ("DirGraph Block");
  outs << meshBlock;

  outs << aDirGraphName;

  outs << (int)mRoot;
  outs << (int)mVertices.size();

  for ( int i = 0; i < mVertices.size() ; ++i )
  {
    outs << (float) mVerticesData[i].pos.x;
    outs << (float) mVerticesData[i].pos.y;
    outs << (float) mVerticesData[i].pos.z;

    outs << (int) mVertices[i].desc.size();
    for ( int j = 0; j < mVertices[i].desc.size() ; ++j )
    {
      outs << (int) mVertices[i].desc[j];
    }

  }

}

void DirectedGraph::DirGraph::exportC(QTextStream &outs , QString& aExportName)
{
  outs << "////////////////////////////////////////////////////////////"<< endl;
  outs << "//   " << aExportName << endl;
  outs << "////////////////////////////////////////////////////////////"<< endl << endl;

  outs << "namespace StoredDirGraph"<< aExportName << " {" << endl << endl;

  outs << "D3DXVECTOR3 StoredV[" << numVertices() << "] = {" << endl;

  int n = numVertices();
  for ( int i = 0 ; i < numVertices() ; ++i )
  {

    outs << "  D3DXVECTOR3("
           << vertex(i).pos.x << " , "
           << vertex(i).pos.y << " , "
           << vertex(i).pos.z << " ) ";
    if (i != (numVertices() - 1))
      outs << "," << endl;
    else
      outs << endl << "};"<< endl << endl;

  }

  //  num edges
  outs << "int StoredNumE[" << numVertices() << "] = {" << endl;
  int numPrinted = 0;
  for ( int i = 0 ; i < numVertices() ; ++i )
  {
    outs << mVertices[i].desc.size();
    if (numPrinted != ( numVertices() - 1))
    {
      outs << ",";
      if ((numPrinted % 3) == 2)
        outs << endl;
    }
    else
      outs << endl << "};"<< endl << endl;
    ++numPrinted;

  }

  //  Edges

  std::vector<Edge> e = edges();
  int numE = e.size();
  numPrinted = 0;
  outs << "int StoredE[" << e.size() << "] = {" << endl;
  for ( int i = 0 ; i < numVertices() ; ++i )
  {
    for ( int j = 0 ; j < e.size() ; ++j )
    {
      if ( e[j].vb == i )
      {
        outs << e[j].ve;
        if (numPrinted != ( e.size() - 1))
        {
          outs << ",";
          if ((numPrinted % 3) == 2)
            outs << endl;
        }
        else
          outs << endl << "};"<< endl;
        ++numPrinted;
      }
    }
  }

  outs << endl<< "}; // end of namespace StoredDirGraph"<< endl << endl ;

  //  Compounded structure
  outs << "DirectedGraph::StoredDirGraph " <<aExportName << " = { " << endl;
  outs << "  " <<n << ","<< endl;
  outs << "  " <<numE << ","<< endl;
  outs << "  &StoredDirGraph"<<aExportName<<"::StoredV[0]," << endl;
  outs << "  &StoredDirGraph"<<aExportName<<"::StoredNumE[0]," << endl;
  outs << "  &StoredDirGraph"<<aExportName<<"::StoredE[0] " << endl;

  outs << "};" << endl;

  outs.flush();
}
