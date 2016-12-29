#include "ObjEditor/ObjectEditor.h"
#include "UI/MainWindow.hpp"

///////////////////////////////////////////////////////////////////////////
///// GizmoT

void ObjectEditor::startOperationGizmoT()
{

}


void ObjectEditor::performOperationGizmoT( D3DXVECTOR3 aRayB,
                                           D3DXVECTOR3 aRayE,
                                           D3DXVECTOR3 aRayB2,
                                           D3DXVECTOR3 aRayE2,
                                           D3DXVECTOR3 aCamU,
                                           D3DXVECTOR3 aCamV)
{
  if ( !mOperationUnfinished )
  {
    mOperationUnfinished = true;
    saveEditorState();
  }

  D3DXVECTOR3 posE       = mGizmoT.newPosition( aRayB,
                                               aRayE,
                                               aCamU,
                                               aCamV );

  D3DXVECTOR3 posB       = mGizmoT.newPosition( aRayB2,
                                               aRayE2,
                                               aCamU,
                                               aCamV );

  mObject->opTranslate ( posE-posB, mObject->getCurrentSelection() );
}

void ObjectEditor::endOperationGizmoT()
{
  mObject->endOpTranslate();
  endOpGeneric();
}


///////////////////////////////////////////////////////////////////////////
///// GizmoR


void ObjectEditor::startOperationGizmoR()
{

}


void ObjectEditor::performOperationGizmoR( D3DXVECTOR3 aRayB,
                                           D3DXVECTOR3 aRayE,
                                           D3DXVECTOR3 aRayB2,
                                           D3DXVECTOR3 aRayE2,
                                           D3DXVECTOR3 aCamU,
                                           D3DXVECTOR3 aCamV)
{
  if ( !mOperationUnfinished )
  {
    mOperationUnfinished = true;
    saveEditorState();
  }

  D3DXVECTOR3 posE       = mGizmoT.newPosition( aRayB,
                                               aRayE,
                                               aCamU,
                                               aCamV );

  D3DXVECTOR3 posB       = mGizmoT.newPosition( aRayB2,
                                               aRayE2,
                                               aCamU,
                                               aCamV );

  mObject->opTranslate ( posE-posB, mObject->getCurrentSelection() );
}

void ObjectEditor::endOperationGizmoR()
{
  endOpGeneric();
}

///////////////////////////////////////////////////////////////////////////
///// GizmoS


void ObjectEditor::startOperationGizmoS()
{
  mOperationUnfinished = true;
  saveEditorState();
  mObject->opStartScale();
}


void ObjectEditor::performOperationGizmoS( D3DXVECTOR3 aRayB,
                                           D3DXVECTOR3 aRayE,
                                           D3DXVECTOR3 aRayB2,
                                           D3DXVECTOR3 aRayE2,
                                           D3DXVECTOR3 aCamU,
                                           D3DXVECTOR3 aCamV)
{

  D3DXVECTOR3 posE       = mGizmoS.newPosition( aRayB,
                                               aRayE,
                                               aCamU,
                                               aCamV );

  D3DXVECTOR3 posB       = mGizmoS.newPosition( aRayB2,
                                               aRayE2,
                                               aCamU,
                                               aCamV );

  mObject->opScale ( (posE-posB), mObject->getCurrentSelection() );
}

void ObjectEditor::endOperationGizmoS()
{
  mObject->endOpScale();
  endOpGeneric();
}

///////////////////////////////////////////////////////////////////////////
///// Extrude

void  ObjectEditor::startOpExtrude()
{
  switch (mOptionExtrudeMode)
  {
  case eOpExtModeOffset:
    mExtrudeValue = 0.0f;
    mObject->opStartExtrude( mObject->getCurrentSelection(),
                             mOptionsExtrude);
    break;
  case eOpExtModeInner:
    mExtrudeValue = 0.0f;
    mObject->opStartExtrudeInner( mObject->getCurrentSelection(),
                                  mOptionsExtrudeInner);
    break;
  case eOpExtModeMatrix:
    mExtrudeValue = 0.0f;
    mObject->opStartExtrudeMatrix( mObject->getCurrentSelection(),
                                   mOptionsExtrudeMatrix);
    break;
  }
}

void  ObjectEditor::performOpExtrude ( float aX, float aY )
{
  switch (mOptionExtrudeMode)
  {
  case eOpExtModeOffset:
    mExtrudeValue += aX*0.01f;
    mObject->opExtrude( mExtrudeValue );
    break;
  case eOpExtModeInner:
    mExtrudeValue += aX*0.01f;
    mObject->opExtrudeInner( mExtrudeValue );
    break;
  case eOpExtModeMatrix:
    mExtrudeValue += aX*0.01f;
    mObject->opExtrudeMatrix( mExtrudeValue );
    break;
  }
}


void  ObjectEditor::endOpExtrude()
{
  switch (mOptionExtrudeMode)
  {
  case eOpExtModeOffset:
    mObject->endOpExtrude();
    break;
  case eOpExtModeInner:
    mObject->endOpExtrudeInner();
    break;
  case eOpExtModeMatrix:
    mObject->endOpExtrudeMatrix();
    break;
  }
}




///////////////////////////////////////////////////////////////////////////
///// BevelV

void  ObjectEditor::startOpBevelV()
{
  mBevelVValue = 0.2f;
  mObject->opStartBevelV( mObject->getCurrentSelection() );
}

void  ObjectEditor::performOpBevelV ( float aX, float aY )
{
  mBevelVValue += aX*0.01f;
  mObject->opBevelV( mBevelVValue );
}

void  ObjectEditor::endOpBevelV()
{
  mObject->endOpBevelV();
}

///////////////////////////////////////////////////////////////////////////
///// BevelV

void  ObjectEditor::startOpBevelE()
{
  mBevelEValue = 0.2f;
  mObject->opStartBevelE( mObject->getCurrentSelection() );
}

void  ObjectEditor::performOpBevelE ( float aX, float aY )
{
  mBevelEValue += aX*0.01f;
  mObject->opBevelE( mBevelEValue );
}

void  ObjectEditor::endOpBevelE()
{
  mObject->endOpBevelE();
}

///////////////////////////////////////////////////////////////////////////
///// Normal Scale

void  ObjectEditor::startOpNorScale()
{
  mNorScaleValue = 1.0f;
  mObject->opStartNorScale( mObject->getCurrentSelection() );
}

void  ObjectEditor::performOpNorScale ( float aX, float aY )
{
  mNorScaleValue += aX*0.01f;
  mObject->opNorScale( mNorScaleValue );
}

void  ObjectEditor::endOpNorScale()
{
  mObject->endOpNorScale();
}



///////////////////////////////////////////////////////////////////////////
///// Knive

void ObjectEditor::startOpKniveLine( const KniveLineInfo& aInfo )
{
  QFile file( QString ("Crash_Knife_Op.txt"));

  if (!file.open(QIODevice::WriteOnly)) {
    QMessageBox::critical(this, tr("Error"),
        tr("Could not open file for save (knife)"));
    return;
  } else {
    QTextStream stream(&file);

    if ( stream.status() != QTextStream::Ok)
    {
      QMessageBox::critical(this, tr("Error"),
          tr("Error on stream creation (knife)."));
      return;
    }

    stream << aInfo.entityB.idComponent;
    stream << ",";
    stream << aInfo.entityB.idEntity;
    stream << ",";
    stream << aInfo.entityB.typeEntity;
    stream << endl;

    stream << aInfo.entityE.idComponent;
    stream << ",";
    stream << aInfo.entityE.idEntity;
    stream << ",";
    stream << aInfo.entityE.typeEntity;
    stream << endl;

    stream << aInfo.segmentPointB.x;
    stream << ",";
    stream << aInfo.segmentPointB.y;
    stream << ",";
    stream << aInfo.segmentPointB.z;
    stream << endl;

    stream << aInfo.segmentPointE.x;
    stream << ",";
    stream << aInfo.segmentPointE.y;
    stream << ",";
    stream << aInfo.segmentPointE.z;


    if ( stream.status() != QTextStream::Ok)
    {
      QMessageBox::critical(this, tr("Error"),
          tr("Error on stream write (knife)."));
      return;
    }

    file.flush();
    file.close();
  }
}

void ObjectEditor::performOpKniveLine(ObjEd::KniveLineInfo &aInfo, D3DXMATRIX& aView, D3DXVECTOR3 aCamPos )
{
  mObject->opKniveLine( aInfo, aView, aCamPos );
}

void  ObjectEditor::endOpKniveLine()
{
  mObject->endOpKniveLine();
}


///////////////////////////////////////////////////////////////////////////
///// Texture

void ObjectEditor::startOpTexture()
{
}

void ObjectEditor::performOpTexture( const int aComId, const int aFaceId )
{
  if ( mObject->hasProcTex())
    mObject->opTexAddToSelection( aComId, aFaceId, mProcTexId );  
}

void ObjectEditor::endOpTexture()
{
  emit updateViews();
}


void ObjectEditor::initTexturing()
{
  mObject->initProcTex();
  //emit updateViews();
}
