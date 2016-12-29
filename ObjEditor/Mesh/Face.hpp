#ifndef FACE_HPP
#define FACE_HPP

#include <vector>
#include "vertex.hpp"

class Face
{

public:
    struct  TriangleDecomposition{ int v1; int v2; int v3; };

    Face(){};
    Face( std::vector<int>& aVertices, std::vector<int>& aHalfEdges );

    void   convertHalfEdgesToFullEdges ( const std::vector<int>& aHalfEdgeToFullEdge );

    int    nVertices() const;
    int    vId(const int i) const;

    int    nEdges() const;
    int    eId(const int i) const;

    int   nDecompositionTriangles() const;
    TriangleDecomposition decompTriangle( const int i ) const;   

private:
    std::vector<int>  mVerticesId;
    std::vector<int>  mEdgesId;

};

#endif // FACE_HPP
