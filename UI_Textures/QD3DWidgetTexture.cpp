#include "QD3DWidgetTexture.hpp"

#include <QMessageBox>
#include <QApplication>

QD3DWidgetTexture::QD3DWidgetTexture(QWidget *parent, UINT width, UINT height, bool showFPS):
    QWidget(parent)
{
  app       = new EditorTextureApp(winId(), width, height, showFPS);
  mWidth    = width;
  mHeight   = height;
  mShowFPS  = showFPS;
  mHasPreviewName = false;
  setAttribute( Qt::WA_OpaquePaintEvent, true);
  setAttribute( Qt::WA_PaintOnScreen, true);
}

QD3DWidgetTexture::~QD3DWidgetTexture()
{
  delete app;
}

bool QD3DWidgetTexture::init()
{
  return app->init ();
}

bool QD3DWidgetTexture::reset()
{
  delete app;
  app = new EditorTextureApp(winId(), mWidth, mHeight, mShowFPS);

  return app->init ();
}

void QD3DWidgetTexture::paintEvent( QPaintEvent* pEvent)
{
  app->drawScene( mPreviewChannelName, mPreviewSignalName, mHasPreviewName);
  update();
}

bool QD3DWidgetTexture::changeShaderPath(LPCWSTR aPath)
{
  return app->changeShaderPath (aPath);
}

void QD3DWidgetTexture::keyPressEvent( QKeyEvent *event )
{
    if ( event->isAutoRepeat() ) {
        event->ignore();
    }
    else {

      switch ( event->key() )
      {
        case Qt::Key_Up :
            app->keyPressed(VK_UP);
            break;

        case Qt::Key_Down :
            app->keyPressed(VK_DOWN);
            break;

        case Qt::Key_Right :
            app->keyPressed(VK_RIGHT);
            break;

        case Qt::Key_Left :
            app->keyPressed(VK_LEFT);
            break;

        case Qt::Key_W :
            app->keyPressed('W');
            break;

        case Qt::Key_S :
            app->keyPressed('S');
            break;

        default:
            event->ignore();
            break;
      }
    }
    event->accept();
}

void QD3DWidgetTexture::keyReleaseEvent( QKeyEvent *event )
{
    if ( event->isAutoRepeat() ){
        event->ignore();
    }
    else {

      switch ( event->key() )
      {
        case Qt::Key_Up :
            app->keyReleased (VK_UP);
            break;

        case Qt::Key_Down :
            app->keyReleased (VK_DOWN);
            break;

        case Qt::Key_Right :
            app->keyReleased (VK_RIGHT);
            break;

        case Qt::Key_Left :
            app->keyReleased (VK_LEFT);
            break;

        case Qt::Key_W :
            app->keyReleased ('W');
            break;

        case Qt::Key_S :
            app->keyReleased ('S');
            break;

        default:
            event->ignore();
            break;
      }
    }

    event->accept();
}

void QD3DWidgetTexture::setPreviewSignalName( std::string& aChannelName, std::string& aSignalName )
{
  mPreviewChannelName = aChannelName;
  mPreviewSignalName = aSignalName;
  mHasPreviewName = true;
}

void QD3DWidgetTexture::setCamera(D3DXVECTOR3 pos, D3DXVECTOR3 at)
{
  app->setCamera(pos,at);
}

