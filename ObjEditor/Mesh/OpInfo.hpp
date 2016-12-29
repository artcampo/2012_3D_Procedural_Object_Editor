#ifndef _OP_INFO_HPP
#define _OP_INFO_HPP

#include    "App/dx_misc.hpp"
#include    "ObjEditor/ObjEditorSharedData.hpp"
#include    "ObjEditor/Mesh/OperationsOptions.hpp"
#include    "ObjEditor/Mesh/OperationsInfo.hpp"

#include <vector>

#define _MESH_QT_IO
#define _MESH_CPP_IO

#ifdef _MESH_QT_IO
  #include <QDataStream>
#endif

#ifdef _MESH_CPP_IO
  #include <iostream>
  #include <fstream>
  using namespace std;
#endif

namespace OpInfo
{
  enum eStoreIdOption{
    eIdEntityNotStored,
    eIdEntityStored
  };

  enum eStoreComponentOption{
    eIdComponentNotStored,
    eIdComponentStored
  };



  ///////////////////////////////////////////////////////////////
  // QT IO
  ///////////////////////////////////////////////////////////////

#ifdef _MESH_QT_IO
  void loadSelectionSingleComponent( QDataStream &stream,
                                     std::vector<ObjEd::Selection>& s,
                                     bool aStoreIdEntity,
                                     bool aStoreIdComponent,
                                     int aDefaultTypeEntity );

  void saveSelectionSingleComponent( QDataStream &stream,
                                     const std::vector<ObjEd::Selection>& s,
                                     bool aStoreIdEntity,
                                     bool aStoreIdComponent  );

  void loadEdgeIntersection( QDataStream &stream,
                                     std::vector<ObjEd::EdgeIntersection>& s);
  void saveEdgeIntersection( QDataStream &stream,
                        const std::vector<ObjEd::EdgeIntersection>& s );
#endif

  ///////////////////////////////////////////////////////////////
  // CPP IO
  ///////////////////////////////////////////////////////////////
#ifdef _MESH_CPP_IO
  void loadSelectionSingleComponent( std::fstream &stream,
                                     std::vector<ObjEd::Selection>& s,
                                     bool aStoreIdEntity,
                                     bool aStoreIdComponent,
                                     int aDefaultTypeEntity );

  void saveSelectionSingleComponent( std::ofstream &stream,
                                     const std::vector<ObjEd::Selection>& s,
                                     bool aStoreIdEntity,
                                     bool aStoreIdComponent  );

  void loadEdgeIntersection( std::fstream &stream,
                                     std::vector<ObjEd::EdgeIntersection>& s);
  void saveEdgeIntersection( std::ofstream &stream,
                        const std::vector<ObjEd::EdgeIntersection>& s );

  static int gStoreComponent;
#endif
  ////////////////////////////////////////////////////////////////////////
  //  Extrude Family
  ////////////////////////////////////////////////////////////////////////

class OpExtrude{

public:
  OpExtrude()
  {

  };
  OpExtrude( const OpExtrude& rhs )
  {
    sel = std::vector<ObjEd::Selection>( rhs.sel.begin(), rhs.sel.end() );
    value = rhs.value;
    options = rhs.options;    
    mStoreIdComponent = rhs.mStoreIdComponent;
  };

  OpExtrude( float v,
             const std::vector<ObjEd::Selection>& s,
             Options::ExtrudeOptions& aOptions,
             int aStoreIdComponent)
  {
    value = v;
    sel = std::vector<ObjEd::Selection>( s.begin(), s.end() );
    options = aOptions;
    mStoreIdComponent = aStoreIdComponent;
  }

public:
  std::vector<ObjEd::Selection> sel;
  float value;
  Options::ExtrudeOptions options;
  int mStoreIdComponent;

#ifdef _MESH_QT_IO
  friend QDataStream& operator<< (QDataStream &stream, const OpExtrude& ob) {
    stream << ob.value;
    stream << ob.options.numParts;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent );
    return stream;
  };
  friend QDataStream& operator>> (QDataStream &stream, OpExtrude& ob){
    stream >> ob.value;
    stream >> ob.options.numParts;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent, ObjEd::eModeFace );
    return stream;
  };

#endif
#ifdef _MESH_CPP_IO

  friend std::ofstream& operator<< (std::ofstream &stream, const OpExtrude& ob) {
    stream << ob.value << endl;
    stream << ob.options.numParts << endl;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent );
    return stream;
  };
  friend std::fstream& operator>> (std::fstream &stream, OpExtrude& ob){
    stream >> ob.value;
    stream >> ob.options.numParts;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent, ObjEd::eModeFace );
    return stream;
  };

#endif

};

class OpExtrudeInner{

public:
  OpExtrudeInner()
  {

  };
  OpExtrudeInner( const OpExtrudeInner& rhs )
  {
    sel = std::vector<ObjEd::Selection>( rhs.sel.begin(), rhs.sel.end() );
    value = rhs.value;
    options = rhs.options;
    mStoreIdComponent = rhs.mStoreIdComponent;
  };

  OpExtrudeInner( float v,
                  const std::vector<ObjEd::Selection>& s,
                  Options::ExtInnerOptions& aOptions,
                  int aStoreIdComponent)
  {
    value = v;
    sel = std::vector<ObjEd::Selection>( s.begin(), s.end() );
    options = aOptions;
    mStoreIdComponent = aStoreIdComponent;
  }

public:
  std::vector<ObjEd::Selection> sel;
  float value;
  Options::ExtInnerOptions options;
  int mStoreIdComponent;

#ifdef _MESH_QT_IO
  friend QDataStream& operator<< (QDataStream &stream, const OpExtrudeInner& ob) {
    stream << ob.value;
    stream << ob.options.numParts;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent );
    return stream;
  };
  friend QDataStream& operator>> (QDataStream &stream, OpExtrudeInner& ob){
    stream >> ob.value;
    stream >> ob.options.numParts;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent, ObjEd::eModeFace );
    return stream;
  };

#endif
#ifdef _MESH_CPP_IO

  friend std::ofstream& operator<< (std::ofstream &stream, const OpExtrudeInner& ob) {
    stream << ob.value << endl;
    stream << ob.options.numParts << endl;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent );
    return stream;
  };
  friend std::fstream& operator>> (std::fstream &stream, OpExtrudeInner& ob){
    stream >> ob.value;
    stream >> ob.options.numParts;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent, ObjEd::eModeFace );
    return stream;
  };
#endif

};


class OpExtrudeMatrix{

public:
  OpExtrudeMatrix()
  {

  };
  OpExtrudeMatrix( const OpExtrudeMatrix& rhs )
  {
    sel = std::vector<ObjEd::Selection>( rhs.sel.begin(), rhs.sel.end() );
    value = rhs.value;
    options = rhs.options;
    mStoreIdComponent = rhs.mStoreIdComponent;
  };

  OpExtrudeMatrix( float v,
                   const std::vector<ObjEd::Selection>& s,
                   Options::ExtMatrixOptions& aOptions,
                   int aStoreIdComponent )
  {
    value = v;
    sel = std::vector<ObjEd::Selection>( s.begin(), s.end() );
    options = aOptions;
    mStoreIdComponent = aStoreIdComponent;
  }

public:
  std::vector<ObjEd::Selection> sel;
  float value;
  Options::ExtMatrixOptions options;
  int mStoreIdComponent;

#ifdef _MESH_QT_IO
  friend QDataStream& operator<< (QDataStream &stream, const OpExtrudeMatrix& ob) {
    stream << ob.value;
    stream << ob.options.numParts;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent );
    return stream;
  };
  friend QDataStream& operator>> (QDataStream &stream, OpExtrudeMatrix& ob){
    stream >> ob.value;
    stream >> ob.options.numParts;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent, ObjEd::eModeFace );
    return stream;
  };

#endif
#ifdef _MESH_CPP_IO

  friend std::ofstream& operator<< (std::ofstream &stream, const OpExtrudeMatrix& ob) {
    stream << ob.value << endl;
    stream << ob.options.numParts << endl;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent );
    return stream;
  };
  friend std::fstream& operator>> (std::fstream &stream, OpExtrudeMatrix& ob){
    stream >> ob.value;
    stream >> ob.options.numParts;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent, ObjEd::eModeFace );
    return stream;
  };
#endif

};


////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
class OpBevelV{

public:
  OpBevelV()
  {

  };
  OpBevelV( const OpBevelV& rhs )
  {
    sel = std::vector<ObjEd::Selection>( rhs.sel.begin(), rhs.sel.end() );
    value = rhs.value;
    mStoreIdComponent = rhs.mStoreIdComponent;
  };

  OpBevelV( float v,
            const std::vector<ObjEd::Selection>& s,
            int aStoreIdComponent )
  {
    value = v;
    sel = std::vector<ObjEd::Selection>( s.begin(), s.end() );
    mStoreIdComponent = aStoreIdComponent;
  }

public:
  std::vector<ObjEd::Selection> sel;
  float value;
  int mStoreIdComponent;

#ifdef _MESH_QT_IO
  friend QDataStream& operator<< (QDataStream &stream, const OpBevelV& ob) {
    stream << ob.value;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent );
    return stream;
  };
  friend QDataStream& operator>> (QDataStream &stream, OpBevelV& ob){
    stream >> ob.value;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel,
                                          eIdEntityNotStored, ob.mStoreIdComponent, ObjEd::eModeVertex );
    return stream;
  };

#endif
#ifdef _MESH_CPP_IO

  friend std::ofstream& operator<< (std::ofstream &stream, const OpBevelV& ob) {
    stream << ob.value << endl;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent );
    return stream;
  };
  friend std::fstream& operator>> (std::fstream &stream, OpBevelV& ob){
    stream >> ob.value;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel,
                                          eIdEntityNotStored, ob.mStoreIdComponent, ObjEd::eModeVertex );
    return stream;
  };
#endif
};

////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
class OpBevelE{

public:
  OpBevelE()
  {

  };
  OpBevelE( const OpBevelV& rhs )
  {
    sel = std::vector<ObjEd::Selection>( rhs.sel.begin(), rhs.sel.end() );
    value = rhs.value;
    mStoreIdComponent = rhs.mStoreIdComponent;
  };

  OpBevelE( float v,
            const std::vector<ObjEd::Selection>& s,
            int aStoreIdComponent )
  {
    value = v;
    sel = std::vector<ObjEd::Selection>( s.begin(), s.end() );
    mStoreIdComponent = aStoreIdComponent;
  }

public:
  std::vector<ObjEd::Selection> sel;
  float value;
  int mStoreIdComponent;

#ifdef _MESH_QT_IO
  friend QDataStream& operator<< (QDataStream &stream, const OpBevelE& ob) {
    stream << ob.value;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent );
    return stream;
  };
  friend QDataStream& operator>> (QDataStream &stream, OpBevelE& ob){
    stream >> ob.value;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel,
                                          eIdEntityNotStored, ob.mStoreIdComponent, ObjEd::eModeEdge );
    return stream;
  };

#endif
#ifdef _MESH_CPP_IO

  friend std::ofstream& operator<< (std::ofstream &stream, const OpBevelE& ob) {
    stream << ob.value << endl;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent );
    return stream;
  };
  friend std::fstream& operator>> (std::fstream &stream, OpBevelE& ob){
    stream >> ob.value;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel,
                                          eIdEntityNotStored, ob.mStoreIdComponent, ObjEd::eModeEdge );
    return stream;
  };
#endif
};

////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
class OpKnifeLine{

public:
  OpKnifeLine()
  {
  };

  OpKnifeLine( const OpKnifeLine& rhs )
  {
    mKnifeInfo            = rhs.mKnifeInfo;
    mCuttedEdges          = rhs.mCuttedEdges;
    mKnifeLineComponentId = rhs.mKnifeLineComponentId;
  };

  OpKnifeLine(  const ObjEd::KniveLineInfo& aKnifeInfo,
                const std::vector<ObjEd::EdgeIntersection>& aCuttedEdges,
                const int aKnifeLineComponentId )
  {
    mKnifeInfo            = aKnifeInfo;
    mCuttedEdges          = aCuttedEdges;
    mKnifeLineComponentId = aKnifeLineComponentId;
  }

  OpKnifeLine(  const ObjEd::KniveLineInfo& aKnifeInfo,
                const int aKnifeLineComponentId )
  {
    mKnifeInfo            = aKnifeInfo;
    mKnifeLineComponentId = aKnifeLineComponentId;    
  }

public:
  ObjEd::KniveLineInfo                  mKnifeInfo;
  std::vector<ObjEd::EdgeIntersection>  mCuttedEdges;
  int                                   mKnifeLineComponentId;

#ifdef _MESH_QT_IO
  friend QDataStream& operator<< (QDataStream &stream, const OpKnifeLine& ob) {
    stream << ob.mKnifeLineComponentId;
    stream << ob.mKnifeInfo.entityB.idEntity;
    stream << ob.mKnifeInfo.entityB.typeEntity;
    stream << ob.mKnifeInfo.entityE.idEntity;
    stream << ob.mKnifeInfo.entityE.typeEntity;
    stream << ob.mKnifeInfo.segmentPointB.x;
    stream << ob.mKnifeInfo.segmentPointB.y;
    stream << ob.mKnifeInfo.segmentPointB.z;
    stream << ob.mKnifeInfo.segmentPointE.x;
    stream << ob.mKnifeInfo.segmentPointE.y;
    stream << ob.mKnifeInfo.segmentPointE.z;
    OpInfo::saveEdgeIntersection( stream, ob.mCuttedEdges );
    return stream;
  };
  friend QDataStream& operator>> (QDataStream &stream, OpKnifeLine& ob){
    stream >> ob.mKnifeLineComponentId;
    stream >> ob.mKnifeInfo.entityB.idEntity;
    stream >> ob.mKnifeInfo.entityB.typeEntity;
    stream >> ob.mKnifeInfo.entityE.idEntity;
    stream >> ob.mKnifeInfo.entityE.typeEntity;
    stream >> ob.mKnifeInfo.segmentPointB.x;
    stream >> ob.mKnifeInfo.segmentPointB.y;
    stream >> ob.mKnifeInfo.segmentPointB.z;
    stream >> ob.mKnifeInfo.segmentPointE.x;
    stream >> ob.mKnifeInfo.segmentPointE.y;
    stream >> ob.mKnifeInfo.segmentPointE.z;
    OpInfo::loadEdgeIntersection( stream, ob.mCuttedEdges );
    return stream;
  };

#endif
#ifdef _MESH_CPP_IO

  friend std::ofstream& operator<< (std::ofstream &stream, const OpKnifeLine& ob) {
    stream << ob.mKnifeLineComponentId << endl;
    stream << ob.mKnifeInfo.entityB.idEntity << endl;
    stream << ob.mKnifeInfo.entityB.typeEntity << endl;
    stream << ob.mKnifeInfo.entityE.idEntity << endl;
    stream << ob.mKnifeInfo.entityE.typeEntity << endl;
    stream << ob.mKnifeInfo.segmentPointB.x << endl;
    stream << ob.mKnifeInfo.segmentPointB.y << endl;
    stream << ob.mKnifeInfo.segmentPointB.z << endl;
    stream << ob.mKnifeInfo.segmentPointE.x << endl;
    stream << ob.mKnifeInfo.segmentPointE.y << endl;
    stream << ob.mKnifeInfo.segmentPointE.z << endl;
    OpInfo::saveEdgeIntersection( stream, ob.mCuttedEdges );
    return stream;
  };
  friend std::fstream& operator>> (std::fstream &stream, OpKnifeLine& ob){
    stream >> ob.mKnifeLineComponentId;
    stream >> ob.mKnifeInfo.entityB.idEntity;
    stream >> ob.mKnifeInfo.entityB.typeEntity;
    stream >> ob.mKnifeInfo.entityE.idEntity;
    stream >> ob.mKnifeInfo.entityE.typeEntity;
    stream >> ob.mKnifeInfo.segmentPointB.x;
    stream >> ob.mKnifeInfo.segmentPointB.y;
    stream >> ob.mKnifeInfo.segmentPointB.z;
    stream >> ob.mKnifeInfo.segmentPointE.x;
    stream >> ob.mKnifeInfo.segmentPointE.y;
    stream >> ob.mKnifeInfo.segmentPointE.z;
    OpInfo::loadEdgeIntersection( stream, ob.mCuttedEdges );
    return stream;
  };
#endif
};


////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
class OpNormScale{

public:
  OpNormScale()
  {

  };
  OpNormScale( const OpNormScale& rhs )
  {
    sel = std::vector<ObjEd::Selection>( rhs.sel.begin(), rhs.sel.end() );
    value = rhs.value;
    mStoreIdComponent = rhs.mStoreIdComponent;
  };

  OpNormScale( float v,
               const std::vector<ObjEd::Selection>& s,
               int aStoreIdComponent )
  {
    value = v;
    sel = std::vector<ObjEd::Selection>( s.begin(), s.end() );
    mStoreIdComponent = aStoreIdComponent;
  }

public:
  std::vector<ObjEd::Selection> sel;
  float value;
  int mStoreIdComponent;

#ifdef _MESH_QT_IO
  friend QDataStream& operator<< (QDataStream &stream, const OpNormScale& ob) {
    stream << ob.value;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent );
    return stream;
  };
  friend QDataStream& operator>> (QDataStream &stream, OpNormScale& ob){
    stream >> ob.value;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel,
                                          eIdEntityNotStored, ob.mStoreIdComponent, ObjEd::eModeFace );
    return stream;
  };

#endif
#ifdef _MESH_CPP_IO

  friend std::ofstream& operator<< (std::ofstream &stream, const OpNormScale& ob) {
    stream << ob.value << endl;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityNotStored, ob.mStoreIdComponent );
    return stream;
  };
  friend std::fstream& operator>> (std::fstream &stream, OpNormScale& ob){
    stream >> ob.value;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel,
                                          eIdEntityNotStored, ob.mStoreIdComponent, ObjEd::eModeFace );
    return stream;
  };
#endif
};

////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
class OpTranslate{

public:
  OpTranslate()
  {

  };
  OpTranslate( const OpTranslate& rhs )
  {
    sel = std::vector<ObjEd::Selection>( rhs.sel.begin(), rhs.sel.end() );
    value = rhs.value;
    mStoreIdComponent = rhs.mStoreIdComponent;
  };

  OpTranslate( D3DXVECTOR3 v,
               const std::vector<ObjEd::Selection>& s,
               int aStoreIdComponent )
  {
    value = v;
    sel = std::vector<ObjEd::Selection>( s.begin(), s.end() );
    mStoreIdComponent = aStoreIdComponent;
  }

public:
  std::vector<ObjEd::Selection> sel;
  D3DXVECTOR3 value;
  int mStoreIdComponent;

#ifdef _MESH_QT_IO
  friend QDataStream& operator<< (QDataStream &stream, const OpTranslate& ob) {
    stream << ob.value.x;
    stream << ob.value.y;
    stream << ob.value.z;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityStored, ob.mStoreIdComponent );
    return stream;
  };
  friend QDataStream& operator>> (QDataStream &stream, OpTranslate& ob){
    stream >> ob.value.x;
    stream >> ob.value.y;
    stream >> ob.value.z;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel, eIdEntityStored,
                                          ob.mStoreIdComponent, ObjEd::eModeNoEntity );
    return stream;
  };

#endif
#ifdef _MESH_CPP_IO

  friend std::ofstream& operator<< (std::ofstream &stream, const OpTranslate& ob) {
    stream << ob.value.x << endl;
    stream << ob.value.y << endl;
    stream << ob.value.z << endl;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityStored, ob.mStoreIdComponent );
    return stream;
  };
  friend std::fstream& operator>> (std::fstream &stream, OpTranslate& ob){
    stream >> ob.value.x;
    stream >> ob.value.y;
    stream >> ob.value.z;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel, eIdEntityStored,
                                          ob.mStoreIdComponent, ObjEd::eModeNoEntity );
    return stream;
  };
#endif
};

////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
class OpScale{

public:
  OpScale()
  {

  };
  OpScale( const OpScale& rhs )
  {
    sel = std::vector<ObjEd::Selection>( rhs.sel.begin(), rhs.sel.end() );
    center = rhs.center;
    value = rhs.value;
    mStoreIdComponent = rhs.mStoreIdComponent;
  };

  OpScale( D3DXVECTOR3 center, D3DXVECTOR3 value,
           const std::vector<ObjEd::Selection>& s,
           int aStoreIdComponent )
  {
    this->center = center;
    this->value = value;
    sel = std::vector<ObjEd::Selection>( s.begin(), s.end() );
    mStoreIdComponent = aStoreIdComponent;
  }

public:
  std::vector<ObjEd::Selection> sel;
  D3DXVECTOR3 center;
  D3DXVECTOR3 value;
  int mStoreIdComponent;

#ifdef _MESH_QT_IO
  friend QDataStream& operator<< (QDataStream &stream, const OpScale& ob) {
    stream << ob.center.x;
    stream << ob.center.y;
    stream << ob.center.z;
    stream << ob.value.x;
    stream << ob.value.y;
    stream << ob.value.z;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityStored, ob.mStoreIdComponent );
    return stream;
  };
  friend QDataStream& operator>> (QDataStream &stream, OpScale& ob){
    stream >> ob.center.x;
    stream >> ob.center.y;
    stream >> ob.center.z;
    stream >> ob.value.x;
    stream >> ob.value.y;
    stream >> ob.value.z;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel,
                                          eIdEntityStored, ob.mStoreIdComponent, ObjEd::eModeNoEntity );
    return stream;
  };

#endif
#ifdef _MESH_CPP_IO

  friend std::ofstream& operator<< (std::ofstream &stream, const OpScale& ob) {
    stream << ob.center.x << endl;
    stream << ob.center.y << endl;
    stream << ob.center.z << endl;
    stream << ob.value.x << endl;
    stream << ob.value.y << endl;
    stream << ob.value.z << endl;
    OpInfo::saveSelectionSingleComponent( stream, ob.sel, eIdEntityStored, ob.mStoreIdComponent );
    return stream;
  };
  friend std::fstream& operator>> (std::fstream &stream, OpScale& ob){
    stream >> ob.center.x;
    stream >> ob.center.y;
    stream >> ob.center.z;
    stream >> ob.value.x;
    stream >> ob.value.y;
    stream >> ob.value.z;
    OpInfo::loadSelectionSingleComponent( stream, ob.sel,
                                          eIdEntityStored, ob.mStoreIdComponent, ObjEd::eModeNoEntity );
    return stream;
  };
#endif
};

////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////
class OpPrimCube{

public:
  OpPrimCube(){};
  OpPrimCube( const OpPrimCube& rhs )
  {
    tesellation = rhs.tesellation;
    Meshcomponent = rhs.Meshcomponent;
  };

  OpPrimCube( int component, int tes )
  {
    tesellation = tes;
    Meshcomponent = component;
  }

public:
  int tesellation;
  int Meshcomponent;  

public:

#ifdef _MESH_QT_IO
  friend QDataStream& operator<< (QDataStream &stream, const OpPrimCube& ob) {
    stream << ob.tesellation;
    stream << ob.Meshcomponent;
    return stream;
  };
  friend QDataStream& operator>> (QDataStream &stream, OpPrimCube& ob){
    stream >> ob.tesellation;
    stream >> ob.Meshcomponent;
    return stream;
  };

#endif
#ifdef _MESH_CPP_IO

  friend std::ofstream& operator<< (std::ofstream &stream, const OpPrimCube& ob) {
    stream << ob.tesellation << endl;
    stream << ob.Meshcomponent << endl;
    return stream;
  };
  friend std::fstream& operator>> (std::fstream &stream, OpPrimCube& ob){
    stream >> ob.tesellation;
    stream >> ob.Meshcomponent;
    return stream;
  };

#endif

};

};

#endif // _OP_INFO_HPP
