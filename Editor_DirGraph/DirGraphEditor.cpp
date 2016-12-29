#include "DirGraphEditor.hpp"
#include "UI/MainWindow.hpp"


DirGraphEditor::DirGraphEditor( )
{

  mDistanceThresholdSelectVertex = 0.1f;
  mDistanceThresholdSelectEdge    = 0.1f;
  mAdjustToGrid = true;
  mGridSize = 0.05f;
  cleanState();
}

DirGraphEditor::~DirGraphEditor()
{
  delete mDirGraph;
}



void DirGraphEditor::updateWorkView()
{
  emit updateViews();
}

void DirGraphEditor::adjustToGrid( D3DXVECTOR3& aP )
{
  if ( mAdjustToGrid )
  {
    aP.x = adjustCoordinate( aP.x, mGridSize );
    aP.y = adjustCoordinate( aP.y, mGridSize );
    aP.z = adjustCoordinate( aP.z, mGridSize );
  }
}

float DirGraphEditor::adjustCoordinate (const float aCoord, float aMod )
{
  float div = floor(aCoord/aMod) * aMod; ;
  float mod = aCoord-div;

  if (mod >= (0.5f*aMod))
      return (div + aMod);
  else if (mod <= (0.5f*aMod))
      return div;
  else
    return (div + aMod) ;

}


void DirGraphEditor::loadFromFile(QString &aFile)
{
  QFile file(aFile);

  if (!file.open(QIODevice::ReadOnly)) {
    QMessageBox::critical(this, tr("Error"),
        tr("Could not open file for load"));
    return;
  } else {

    delete mDirGraph;
    mDirGraph = NULL;

    QDataStream stream(&file);
    mDirGraph = DirectedGraph::DirGraph::load( stream, mDirGraphName );
    cleanState();

    if ( mDirGraph == NULL )
    {
      QMessageBox::critical(this, tr("Error"),
          tr("Corrupted file!"));
    }

    emit dirGraphNameChanged( mDirGraphName );
    updateViews();

  }
}

void DirGraphEditor::saveToFile(QString &aFile)
{
  QFile file(aFile);

  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::critical(this, tr("Error"),
        tr("Could not open file for save"));
    return;
  } else {

    QDataStream stream(&file);

    if ( stream.status() != QDataStream::Ok)
    {
      QMessageBox::critical(this, tr("Error"),
          tr("Error on stream creation."));
      return;
    }

    mDirGraph->save( stream, mDirGraphName );

    if ( stream.status() != QDataStream::Ok)
    {
      QMessageBox::critical(this, tr("Error"),
          tr("Error on stream write."));
      return;
    }

    file.flush();    
    file.close();

  }
}

void DirGraphEditor::exportToFile()
{
  QString fileName;
  fileName = mDirGraphName + QString(".txt");
  QFile file(fileName);

  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::critical(this, tr("Error"),
        tr("Could not open file for save"));
    return;
  } else {

    QTextStream stream(&file);

    if ( stream.status() != QTextStream::Ok)
    {
      QMessageBox::critical(this, tr("Error"),
          tr("Error on stream creation."));
      return;
    }

    mDirGraph->exportC( stream, mDirGraphName );


    if ( stream.status() != QDataStream::Ok)
    {
      QMessageBox::critical(this, tr("Error"),
          tr("Error on stream write."));
      return;
    }

    file.flush();
    file.close();

  }


}

bool  DirGraphEditor::checkVertex(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXMATRIX& aView, D3DXVECTOR3 aCamPos )
{

  DirGraphEd::Selection s = selectVertex( aRayB, aRayE, distanceVertexThreshold(), aCamPos );
  return (s.idEntity != -1);
}

int DirGraphEditor::getVertex   ( D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE, D3DXMATRIX& aView, D3DXVECTOR3 aCamPos )
{

  DirGraphEd::Selection s = selectVertex( aRayB, aRayE, distanceVertexThreshold(), aCamPos );
  return (s.idEntity);
}

void DirGraphEditor::highlightEntity(const D3DXVECTOR3 &aRayB, const D3DXVECTOR3 &aRayE, D3DXVECTOR3 aCamPos)
{
  mHighV = selectVertex ( aRayB, aRayE, distanceVertexThreshold(), aCamPos );
  mHighE = selectEdge   ( aRayB, aRayE, distanceEdgeThreshold(), aCamPos );

}

DirGraphEd::Selection DirGraphEditor::selectVertex(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE,
                                                   float aDistThreshold, D3DXVECTOR3 aCamPos )
{
  DirGraphEd::Selection s;
  s.typeEntity = DirGraphEd::eVertex;
  s.idEntity = -1;

  for ( unsigned int i = 0 ; i < mDirGraph->numVertices(); ++i )
  {
    bool  existsMin = false;
    float distanceMin;

    D3DXVECTOR3 pos = mDirGraph->vertex(i).pos;

    float distance = Intersections::distancePointToLine ( aRayB, aRayE, pos );
    bool update = false;
    if (!existsMin && distance < aDistThreshold ) update = true;
    if ( existsMin && distance < aDistThreshold && distance<distanceMin ) update = true;

    if (update)
    {
      distanceMin = distance;
      s.idEntity = i;
    }

  }

  return s;
}

DirGraphEd::Selection DirGraphEditor::selectEdge(D3DXVECTOR3 aRayB, D3DXVECTOR3 aRayE,
                                 float aDistThreshold, D3DXVECTOR3 aCamPos )
{
  DirGraphEd::Selection s;
  s.typeEntity = DirGraphEd::eEdge;
  s.idEntity = -1;

  std::vector<DirectedGraph::Edge> edges = mDirGraph->edges();
  bool  existsMin = false;
  float distanceMin;

  for ( unsigned int i = 0 ; i < edges.size() ; ++i )
  {
    D3DXVECTOR3 v1, v2;
    v1 = mDirGraph->vertex( edges[i].vb ).pos;
    v2 = mDirGraph->vertex( edges[i].ve ).pos;

    D3DXVECTOR3 pointInSegment;
    float distance = Intersections::distanceSegmentToLine(  aRayB, aRayE, v1, v2, pointInSegment );
    bool update = false;
    if (!existsMin && distance < aDistThreshold ) update = true;
    if ( existsMin && distance < aDistThreshold && distance<distanceMin ) update = true;

    if (update)
    {
      distanceMin = distance;
      s.idEntity = i;
    }

  }

  return s;
}


float DirGraphEditor::distanceVertexThreshold()
{
  return mDistanceThresholdSelectVertex;
}

float DirGraphEditor::distanceEdgeThreshold()
{
  return mDistanceThresholdSelectEdge;
}

void DirGraphEditor::newFile()
{
  mDirGraph = new DirectedGraph::DirGraph();
  mDirGraph->newNode( TrackBp::Vertex( D3DXVECTOR3(0,0,0), D3DXVECTOR3(1,0,0), D3DXVECTOR3(0,1,0) ));

  cleanState();
}


void DirGraphEditor::cleanState()
{
  mSelectedVertex = 0;
  mHighE.idEntity = -1;
  mHighV.idEntity = -1;
}


void DirGraphEditor::changeSelectedVertex(const int aVid)
{
  mSelectedVertex = aVid;
  emit updateViews();
}

int DirGraphEditor::createVertex(const D3DXVECTOR3 &aPos)
{
  D3DXVECTOR3 pos = aPos;
  adjustToGrid( pos );
  return mDirGraph->newNode( TrackBp::Vertex( pos ) );
}


void DirGraphEditor::selectWorkView( QD3DWidgetDirG* aView )
{
  if ( mWorkingView != aView )
  {
    mWorkingView = aView;
    updateWorkView();
  }
}

EditableDirGraph * DirGraphEditor::convertModel()
{
  return EditableDirGraph::convert( *mDirGraph, mSelectedVertex, mHighV.idEntity, mHighE.idEntity,
                                     mWorkingView->app->getD3DXdevice() );
}

int   DirGraphEditor::selectedVertex() const
{
  return mSelectedVertex;
}

void DirGraphEditor::createEdge(const int aVid1, const int aVid2)
{
  mDirGraph->newEdge( aVid1, aVid2, TrackBp::EdgeData() );
  updateWorkView();
}


D3DXVECTOR3 DirGraphEditor::vertexPos ( const int aVid ) const
{
  return mDirGraph->vertex(aVid).pos;
}

void DirGraphEditor::modifyVertex(const int aVid, const D3DXVECTOR3 &aPos)
{  
  D3DXVECTOR3 pos = aPos;
  adjustToGrid( pos );
  mDirGraph->setVertexData( aVid, pos );

  std::ostringstream st;
  st<< "Pos : (" <<pos.x <<", " <<pos.y<<", " <<pos.z << ")";
  QString vertexPosition ( st.str().c_str() );
  updateStatusBarByTab( vertexPosition );
}


void DirGraphEditor::deleteHighlighted()
{

  if (mHighV.idEntity != -1 && mHighV.idEntity != 0)
  {
    mDirGraph->deleteVertex( mHighV.idEntity );
    mHighV.idEntity = -1;
    mSelectedVertex = 0;
    updateWorkView();
  }
  else
  {
    if (mHighE.idEntity != -1 )
    {
      std::vector<DirectedGraph::Edge> edges = mDirGraph->edges();
      int v1, v2;
      v1 = edges[ mHighE.idEntity].vb;
      v2 = edges[ mHighE.idEntity].ve;
      mDirGraph->deleteEdge( v1, v2 );
      mHighE.idEntity = -1;
      updateWorkView();
    }
  }
}

void DirGraphEditor::updateStatusBarByTab( QString aName )
{
  GlobalEditor::gMainWindow->updateStatusBarByTab(aName);
}

void DirGraphEditor::setGraphExportName(QString &aName)
{
  mDirGraphName = aName;
}

void DirGraphEditor::toggleAdjustGrid(bool v)
{
  mAdjustToGrid = v;
}

void DirGraphEditor::adjustYCoord()
{
  for ( unsigned int i = 0 ; i < mDirGraph->numVertices(); ++i )
  {
    D3DXVECTOR3 pos = mDirGraph->vertex(i).pos;
    pos.y = 0;
    mDirGraph->setVertexData( i, pos );
  }
}
void DirGraphEditor::adjustAllPointsToGrid()
{
  for ( unsigned int i = 0 ; i < mDirGraph->numVertices(); ++i )
  {
    D3DXVECTOR3 pos = mDirGraph->vertex(i).pos;
    adjustToGrid( pos ) ;
    mDirGraph->setVertexData( i, pos );
  }
}
