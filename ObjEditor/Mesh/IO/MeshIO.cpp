#include "ObjEditor/Mesh/Mesh.hpp"

///////////////////////////////////////////////////////////////
// QT IO
///////////////////////////////////////////////////////////////


void Mesh::save(QDataStream &outs)
{
  QString title ("Not Gordian Editor v0.2");
  outs << title;
  outs << (int)1;

  QString meshBlock ("Mesh Block");
  outs << meshBlock;

  mStack->save (outs);

  outs << (int) mTextures.size();

  for (int i = 0; i< mTextures.size(); ++i)
    mTextures[i].save(outs);

  for (int i = 0; i < nComponents(); ++i)
  {
    mMeshObject[i]->saveTexture( outs );
    mMeshObject[i]->saveSelection( outs );
  }

}

void Mesh::load(QDataStream &ins, QString& aPathApp )
{
  release();
  init();

  int numBlocks;
  QString s,t;
  ins >> s;

  if (s != QString ("Not Gordian Editor v0.2"))
    return;

  ins >> numBlocks;
  ins >> t;

  if (t != QString ("Mesh Block"))
    return;

  mStack->load (ins);
  reconstructFromStack();

  int size;
  ins >> size;
  mTextures.resize( size );

  for (int i = 0; i< mTextures.size(); ++i)
  {
    mTextures[i].load( ins, aPathApp );
  }

  for (int i = 0; i < nComponents(); ++i)
  {
    mMeshObject[i]->loadTexture( ins );
    mMeshObject[i]->loadSelection( ins );
  }

}

///////////////////////////////////////////////////////////////
// CPP IO
///////////////////////////////////////////////////////////////

void  Mesh::save( std::ofstream & outs )
{
  mStack->save(outs);

  for (int i = 0; i < nComponents(); ++i)
    mMeshObject[i]->saveTexture( outs );

}

void  Mesh::load( std::fstream & ins )
{
  release();
  init();

  mStack->load(ins);
  reconstructFromStack();

#ifdef _MESH_CPP_IO
  for (int i = 0; i < nComponents(); ++i)
    mMeshObject[i]->loadTexture( ins );
#endif


}
