#include "editorobject.h"

EditorObject::EditorObject(){};

EditorObject::EditorObject( int aUniqueId, std::string& aName )
{
  mName         = aName;
  mUniqueId     = aUniqueId;
  mHighLighted  = false;
}

std::string EditorObject::name()
{
  return mName;
}

void EditorObject::setName(const std::string &aName)
{
  mName = aName;
}

void EditorObject::setHighlighted(const bool &aValue)
{
  mHighLighted = aValue;
}

bool EditorObject::isHighlighted()
{
  return mHighLighted;
}

int EditorObject::getId()
{
  return mUniqueId;
}
