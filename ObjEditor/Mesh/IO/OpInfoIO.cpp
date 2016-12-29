#include "ObjEditor/Mesh/OpInfo.hpp"

namespace OpInfo
{

  ///////////////////////////////////////////////////////////////
  // QT IO
  ///////////////////////////////////////////////////////////////

#ifdef _MESH_QT_IO
  void loadSelectionSingleComponent( QDataStream &stream,
                                     std::vector<ObjEd::Selection>& s,
                                     bool aStoreIdEntity,
                                     bool aStoreIdComponent,
                                     int aDefaultTypeEntity)
  {
    int length;
    stream >> length;
    int component, type;

    if ( aStoreIdComponent )
     stream >> component;
    else
      component = 0;

    if ( aStoreIdEntity )
     stream >> type;
    else
      type = aDefaultTypeEntity;

    s = std::vector<ObjEd::Selection> ( length );
    for ( unsigned int i = 0; i < s.size() ; ++i)
    {
      stream >> int ( s[i].idEntity );
      s[i].idComponent = component;
      s[i].typeEntity = type;
    }
  };

  void saveSelectionSingleComponent( QDataStream &stream,
                                     const std::vector<ObjEd::Selection>& s,
                                     bool aStoreIdEntity,
                                     bool aStoreIdComponent  )
  {
    stream << int(s.size());
    if (aStoreIdComponent)
     stream << int(s[0].idComponent);
    if (aStoreIdEntity)
     stream << int(s[0].typeEntity);
    for ( unsigned int i = 0; i < s.size() ; ++i)
      stream << int ( s[i].idEntity );

  };

  void loadEdgeIntersection( QDataStream &stream,
                                     std::vector<ObjEd::EdgeIntersection>& s)
  {
    int length;
    stream >> length;

    s = std::vector<ObjEd::EdgeIntersection> ( length );
    for ( unsigned int i = 0; i < s.size() ; ++i)
    {
      stream >> int    ( s[i].idEntity );
      stream >> float  ( s[i].p.x );
      stream >> float  ( s[i].p.y );
      stream >> float  ( s[i].p.z );
    }
  };

  void saveEdgeIntersection( QDataStream &stream,
                        const std::vector<ObjEd::EdgeIntersection>& s )
  {
    stream << int(s.size());
    for ( unsigned int i = 0; i < s.size() ; ++i)
    {
      stream << int    ( s[i].idEntity );
      stream << float  ( s[i].p.x );
      stream << float  ( s[i].p.y );
      stream << float  ( s[i].p.z );
    }

  };
#endif

  ///////////////////////////////////////////////////////////////
  // CPP IO
  ///////////////////////////////////////////////////////////////

#ifdef _MESH_CPP_IO
  void loadSelectionSingleComponent( std::fstream &stream,
                                     std::vector<ObjEd::Selection>& s,
                                     bool aStoreIdEntity,
                                     bool aStoreIdComponent,
                                     int aDefaultTypeEntity)
  {
    int length;
    stream >> length;
    int component, type;

    if ( aStoreIdComponent )
     stream >> component;
    else
      component = 0;

    if ( aStoreIdEntity )
     stream >> type;
    else
      type = aDefaultTypeEntity;

    s = std::vector<ObjEd::Selection> ( length );
    for ( unsigned int i = 0; i < s.size() ; ++i)
    {
      stream >> int ( s[i].idEntity );
      s[i].idComponent = component;
      s[i].typeEntity = type;
    }
  };

  void saveSelectionSingleComponent( std::ofstream &stream,
                                     const std::vector<ObjEd::Selection>& s,
                                     bool aStoreIdEntity,
                                     bool aStoreIdComponent  )
  {
    stream << int(s.size()) << endl;
    if (aStoreIdComponent)
     stream << int(s[0].idComponent) << endl;
    if (aStoreIdEntity)
     stream << int(s[0].typeEntity) << endl;
    for ( unsigned int i = 0; i < s.size() ; ++i)
      stream << int ( s[i].idEntity ) << endl;

  };

  void loadEdgeIntersection( std::fstream &stream,
                                     std::vector<ObjEd::EdgeIntersection>& s)
  {
    int length;
    stream >> length;

    s = std::vector<ObjEd::EdgeIntersection> ( length );
    for ( unsigned int i = 0; i < s.size() ; ++i)
    {
      stream >> int    ( s[i].idEntity );
      stream >> float  ( s[i].p.x );
      stream >> float  ( s[i].p.y );
      stream >> float  ( s[i].p.z );
    }
  };

  void saveEdgeIntersection( std::ofstream &stream,
                        const std::vector<ObjEd::EdgeIntersection>& s )
  {
    stream << int(s.size()) << endl;
    for ( unsigned int i = 0; i < s.size() ; ++i)
    {
      stream << int    ( s[i].idEntity ) << endl;
      stream << float  ( s[i].p.x ) << endl;
      stream << float  ( s[i].p.y ) << endl;
      stream << float  ( s[i].p.z ) << endl;
    }

  };
#endif

}
