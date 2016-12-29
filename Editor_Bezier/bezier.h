#ifndef BEZIER_H
#define BEZIER_H

#include "Editor_Bezier/editorobject.h"
#include "App/dx_misc.hpp"
#include <vector>
#include <QDataStream>
#include <QTextStream>
#include <qstring.h>

class Bezier : public EditorObject
{
public:
    Bezier();
    Bezier( int aUniqueId, std::string& aName );
    Bezier( int aUniqueId, std::string& aName, const Bezier& b );
    ~Bezier(){};


    void appendPoint( const D3DXVECTOR3& aP, const D3DXVECTOR3& aUnit );
    void deletePoint( const int aIndex );

    std::vector<D3DXVECTOR3> getPoints();
    std::vector<D3DXVECTOR3> getCurve();


    std::vector<D3DXVECTOR3> getTangents( int aIndex );
    std::vector<D3DXVECTOR3> getTangentPoints( int aIndex );

    int           getNumPoints();
    D3DXVECTOR3   getPoint( int aIndex );

    void          translate ( const D3DXVECTOR3& aVector );

    void          setPoint( int aIndex, const D3DXVECTOR3& p );
    void          setPointForceTangentContinuity( int aIndex, const D3DXVECTOR3& p );
    void          setTangent(int aIndex, bool aPreviousTangent, const D3DXVECTOR3& p);
    void          setTangentCorrectOpposite(int aIndex, bool aPreviousTangent, const D3DXVECTOR3& p);
    void          close();
    int           getNumStoredElements();

private:

    int     mNumSegments;
    int     mNumPoints;
    std::vector<D3DXVECTOR3>  mPoints;

    int     mTesellationSegment;

private:
    int indexOfPoint( int aIndex );

public:
    void exportToC( QTextStream& aStream );
    void exportToCLast( QTextStream& aStream );

    friend QDataStream& operator<< (QDataStream &stream, const Bezier& ob) {
        QString name = QString::fromStdString(ob.mName);
        stream << name;
        stream << ob.mNumSegments;
        stream << ob.mNumPoints;
        stream << ob.mUniqueId;
        stream << ob.mTesellationSegment;
        stream << ob.mPoints.size();

        for (unsigned int i = 0; i < ob.mPoints.size(); ++i )
        {
          stream << ob.mPoints[i].x;
          stream << ob.mPoints[i].y;
          stream << ob.mPoints[i].z;
        }

        return stream;
    }
    friend QDataStream& operator>> (QDataStream &stream, Bezier& ob) {
        QString name;
        stream >> name;
        ob.mName = name.toStdString();
        ob.mHighLighted = false;
        stream >> ob.mNumSegments;
        stream >> ob.mNumPoints;
        stream >> ob.mUniqueId;
        stream >> ob.mTesellationSegment;
        unsigned int size;
        stream >> size;
        ob.mPoints.resize(size);

        for (int i = 0; i < size; ++i )
        {
          stream >> ob.mPoints[i].x;
          stream >> ob.mPoints[i].y;
          stream >> ob.mPoints[i].z;
        }

        return stream;
    }
};


#endif // BEZIER_H
