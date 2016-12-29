#ifndef EDGE_HPP
#define EDGE_HPP


class Edge
{
public:
    Edge(){};
    Edge( int abId, int aeId ) { bId = abId; eId = aeId; };
    int bId;
    int eId;
    Edge& operator=(const Edge& rhs)
    {
      bId = rhs.bId;
      eId = rhs.eId;
      return *this;
    }
};

#endif // EDGE_HPP
