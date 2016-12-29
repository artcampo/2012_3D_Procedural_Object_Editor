#ifndef _editableobject_H
#define _editableobject_H

#include <string>

class editableobject
{
public:
    editableobject();
    editableobject( int aUniqueId, std::string& aName );
    virtual ~editableobject() = 0;

    void setName( const std::string& aName );
    std::string name();

    void setHighlighted ( const bool& aValue );
    bool isHighlighted();
    int   getId();
protected:
    bool          mHighLighted;
    std::string   mName;
    int           mUniqueId;
};

#endif // editableobject
