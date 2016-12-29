#include "objecttree.h"

ObjectTree::ObjectTree( EditorBezier* aEditor )
{
  mEditor = aEditor;

  //this->set
  setColumnCount(1);
  QStringList q;
  q.append( QString("Bezier Splines"));

  setHeaderLabels(q);

  connect(this, SIGNAL(itemPressed ( QTreeWidgetItem*, int )),
          this, SLOT(objectEditorSelected ( QTreeWidgetItem*, int )));

}



void ObjectTree::addObjectEditor(const std::string &aName)
{
  mItems = new QTreeWidgetItem(this);
  QString text = QString::fromStdString(aName);
  mItems->setText( 0, text  );
  insertTopLevelItem ( 0, mItems );
}

void ObjectTree::objectEditorSelected(QTreeWidgetItem *item, int column)
{
  if (item != 0)
    mEditor->selectBezierCurve( item->text(column).toStdString() );
}

void  ObjectTree::initObjectEditor( const std::vector<std::string>& aName )
{
  clear();
  for ( unsigned int i = 0; i < aName.size() ; ++i)
    addObjectEditor( aName[i] );
}
