#ifndef QD3DWIDGET_TEXTURE_HPP
#define QD3DWIDGET_TEXTURE_HPP

#include <QWidget>
#include <QKeyEvent>

#include "App/EditorTextureApp.hpp"

class QD3DWidgetTexture : public QWidget
{
    Q_OBJECT
public:
  explicit
  QD3DWidgetTexture(QWidget *parent = 0, UINT width = 800, UINT height = 600,
             bool showFPS = true);
  ~QD3DWidgetTexture();
  QPaintEngine *paintEngine() const { return NULL; }
  void paintEvent(QPaintEvent* pEvent);
  bool changeShaderPath(LPCWSTR aPath);
  bool init();
  bool reset();
  void setPreviewSignalName ( std::string& aChannelName, std::string& aSignalName );
  void setCamera( D3DXVECTOR3 pos, D3DXVECTOR3 at );

private:
  void keyPressEvent( QKeyEvent *event );
  void keyReleaseEvent( QKeyEvent *event );
private:
  EditorTextureApp* app;
  UINT mWidth;
  UINT mHeight;
  bool mShowFPS;

  std::string mPreviewChannelName;
  std::string mPreviewSignalName;  
  bool        mHasPreviewName;  

};

#endif // QD3DWidgetTexture_HPP
