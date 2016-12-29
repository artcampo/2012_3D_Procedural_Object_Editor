#ifndef _ObjectTreeWidget_H
#define _ObjectTreeWidget_H

#include <QTreeWidget>
#include <QStringList>
#include <string>
#include <QObject>
#include <QtGui>

#include "ObjEditor/ObjectEditor.h"

class ObjectTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    ObjectTreeWidget( QWidget *parent, ObjectEditor* aEditor );

    void  addObject ( const std::string& aName );

private:
    QTreeWidgetItem *mItems;
    ObjectEditor*   mEditor;


public slots:
    void objectEditorSelected (QTreeWidgetItem * item, int column );

};

#endif // ObjectTreeWidget_H
