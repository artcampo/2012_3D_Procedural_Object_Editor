#ifndef _DIRGRAPH_HPP
#define _DIRGRAPH_HPP

//#include <algorithm>
#include <vector>

#include "Geometry/DirGraph/DirGraphDataTypes.hpp"
#include "Misc/StlHelpers.hpp"

// qt IO
#include <QFile>
#include <QString>
#include <QDataStream>
#include <QTextStream>

namespace DirectedGraph {

class DirGraph{
public:
  static DirectedGraph::DirGraph* load( QDataStream &ins, QString& aDirGraphName );
  void    save       (QDataStream &outs, QString& aDirGraphName );
  void    exportC    ( QTextStream &outs, QString& aExportName );
public:
  DirGraph(){ mRoot = 0;};

  void  setRoot( const int aIdv );
  int   newNode( const TrackBp::Vertex& aData );  
  int   newEdge( const int aIdv1, const int aIdv2, const TrackBp::EdgeData& aEdge );  
  void  setEdge( const int aIdv1, const int aIdv2, const TrackBp::EdgeData& aData );

  void  setVertexData ( const int aIdv, const D3DXVECTOR3& aPos );
  /*
  void  setDescendants  ( const int aIdv, std::vector<int>& aDesc );
  void  setParents      ( const int aIdv, std::vector<int>& aPar );
*/

  void  deleteVertex      ( const int aIdv );
  void  deleteEdge        ( const int aIdv1, const int aIdv2 );

  std::vector<Edge> edges() const;

  int               root() const;
  int               numVertices() const;
  int               numOutEdges( const int aIdv ) const;
  
  TrackBp::Vertex   vertex  ( const int aIdv ) const;
  TrackBp::EdgeData edge    ( const int aIdv1, const int aIdv2 ) const;
  int               numDesc ( const int aIdv ) const;
  int               numPar  ( const int aIdv ) const;

  std::vector<int>  descendants ( const int aIdv ) const;
  std::vector<int>  parents     ( const int aIdv ) const;

private:  
  int                           mRoot;
  std::vector<Vertex>           mVertices;
  std::vector<TrackBp::Vertex>  mVerticesData;

public:
  void  deleteDescendants ( const int aIdv );
  void  deleteParents     ( const int aIdv );
  void  deleteDescendant  ( const int aIdv1, const int aIdv2 );
  void  deleteParent      ( const int aIdv1, const int aIdv2 );
private:
  void edgesRec( const int aIdv, std::vector<Edge>& aEdges ) const;

  friend class DirGraphEditor;

};

};



#endif // _DIRGRAPH_HPP
