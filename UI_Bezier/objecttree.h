#ifndef OBJECTTREE_H
#define OBJECTTREE_H

#include <QTreeWidget>
#include <QStringList>
#include <string>
#include <QObject>
#include <QtGui>

#include "Editor_Bezier/editorbezier.h"

class ObjectTree : public QTreeWidget
{
    Q_OBJECT

public:
    ObjectTree(  EditorBezier* aEditor );

    void  addObjectEditor ( const std::string& aName );
    void  initObjectEditor( const std::vector<std::string>& aName );

private:
    QTreeWidgetItem *mItems;
    EditorBezier* mEditor;

public slots:
    void objectEditorSelected (QTreeWidgetItem * item, int column );

};

#endif // OBJECTTREE_H
