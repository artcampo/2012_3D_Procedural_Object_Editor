#ifndef HE_FACE_HPP
#define HE_FACE_HPP


#include <vector>

class HE_edge;
class HE_vert;

class HE_face{
public:
  HE_face(){};  //  TODO: only for resizing of vectors!
  HE_face( int aId ){ mId = aId; };
  HE_face( const HE_face& rhs );
  void setEdge( const HE_edge* e );

  std::vector<HE_edge*>  edges () const;
  std::vector<HE_vert*>  vertices() const;

  std::vector<HE_edge*> edgeChain( const HE_vert* aV1, const HE_vert* aV2 ) const;

  HE_edge*  edgeIncidentToVertex( HE_vert* aVertex ) const;
public:
  HE_edge*    mEdge;
  int         mId;
private:
  friend class HE_mesh; //for serialization
  //HE_face& operator=( const HE_face& rhs );
};

#endif // HE_FACE_HPP
