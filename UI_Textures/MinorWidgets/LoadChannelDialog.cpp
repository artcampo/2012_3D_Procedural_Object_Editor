#include "LoadChannelDialog.hpp"

LoadChannelDialog::LoadChannelDialog(QWidget *parent, std::vector<QString> aNames ) :
    QDialog(parent)
{


  mLayout = new QVBoxLayout(this);
  mLayout->setAlignment( Qt::AlignTop );
  setLayout(mLayout);

  mNestedLayout = new QHBoxLayout (this);

  mNamesList = new QListWidget(this);
  mNamesList->setSelectionMode ( QAbstractItemView::SingleSelection );

  for ( int i = 0; i < aNames.size(); ++i )
  {
    mNamesList->insertItem( 0, aNames[i]);
  }

  mNamesList->setCurrentItem( &QListWidgetItem( aNames[0] ) );

  mAcceptButton = new QPushButton(this);
  mAcceptButton->setText( QString ("Accept"));
  QObject::connect(mAcceptButton, SIGNAL(clicked()), this, SLOT(accept()));


  mCancelButton = new QPushButton(this);
  mCancelButton->setText( QString ("Cancel"));
  QObject::connect(mCancelButton, SIGNAL(clicked()), this, SLOT(reject()));




  mLayout->addWidget( mNamesList );
  mLayout->addLayout( mNestedLayout );
  mNestedLayout->addWidget( mAcceptButton );
  mNestedLayout->addWidget( mCancelButton );

}

QString LoadChannelDialog::selected()
{
  return mNamesList->currentItem()->text();
}
