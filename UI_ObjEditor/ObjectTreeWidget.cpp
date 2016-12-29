#include "ObjectTreeWidget.h"

ObjectTreeWidget::ObjectTreeWidget( QWidget *parent, ObjectEditor* aEditor )
  : QTreeWidget( parent )
{
  mEditor = aEditor;

  //this->set
  setColumnCount(1);
  QStringList q;
  q.append( QString("Objects"));

  setHeaderLabels(q);

  connect(this, SIGNAL(itemPressed ( QTreeWidgetItem*, int )),
          this, SLOT(objectEditorSelected ( QTreeWidgetItem*, int )));

}



void ObjectTreeWidget::addObject(const std::string &aName)
{
  mItems = new QTreeWidgetItem(this);
  QString text = QString::fromStdString(aName);
  mItems->setText( 0, text  );
  insertTopLevelItem ( 0, mItems );
}

void ObjectTreeWidget::objectEditorSelected(QTreeWidgetItem *item, int column)
{
  /*
  if (item != 0)
    mEditor->selectBezierCurve( item->text(column).toStdString() );
    */
}
