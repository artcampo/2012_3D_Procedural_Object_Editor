#ifndef EDITOROBJECT_H
#define EDITOROBJECT_H

#include <string>

class EditorObject
{
public:
    EditorObject();
    EditorObject( int aUniqueId, std::string& aName );
    virtual ~EditorObject(){};

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

#endif // EDITOROBJECT_H
