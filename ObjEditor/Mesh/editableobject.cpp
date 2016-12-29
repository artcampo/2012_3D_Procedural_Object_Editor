#include "editableobject.h"

editableobject::editableobject(){};

editableobject::editableobject( int aUniqueId, std::string& aName )
{
  mName         = aName;
  mUniqueId     = aUniqueId;
  mHighLighted  = false;
}

std::string editableobject::name()
{
  return mName;
}

void editableobject::setName(const std::string &aName)
{
  mName = aName;
}

void editableobject::setHighlighted(const bool &aValue)
{
  mHighLighted = aValue;
}

bool editableobject::isHighlighted()
{
  return mHighLighted;
}

int editableobject::getId()
{
  return mUniqueId;
}
