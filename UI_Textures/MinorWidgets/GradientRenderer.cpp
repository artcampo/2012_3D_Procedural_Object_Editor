#include "UI_Textures\MinorWidgets\GradientRenderer.h"

GradientRenderer::GradientRenderer(QWidget *parent, const int aLength )
    : QWidget(parent),
      mLength(aLength)
{
    shape = Rect;
    antialiased = false;
    transformed = false;

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    linearGradient = new QLinearGradient(0,0,1,0);
    linearGradient->setColorAt(0, QColor( Qt::white) );
    linearGradient->setColorAt(1, QColor( Qt::black) );

    linearGradient->setSpread( QGradient::RepeatSpread );
    linearGradient->setCoordinateMode( QGradient::ObjectBoundingMode );
}

QSize GradientRenderer::minimumSizeHint() const
{
    //return QSize(100, 25);
  return QSize(mLength, 25);
}

QSize GradientRenderer::sizeHint() const
{
    //return QSize(100, 25);
  return QSize(mLength, 25);
}

void GradientRenderer::setShape(Shape shape)
{
    this->shape = shape;
    update();
}

void GradientRenderer::setPen(const QPen &pen)
{
    this->pen = pen;
    update();
}

void GradientRenderer::setBrush(const QBrush &brush)
{
    this->brush = brush;
    update();
}

void GradientRenderer::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}

void GradientRenderer::setTransformed(bool transformed)
{
    this->transformed = transformed;
    update();
}

void GradientRenderer::setColors( const QColor& c1, const QColor& c2 )
{
    this->c1 = c1;
    this->c2 = c2;
    this->transformed = transformed;
    linearGradient->setColorAt(0, c1 );
    linearGradient->setColorAt(1, c2 );
    update();
}

void GradientRenderer::paintEvent(QPaintEvent * /* event */)
{

    QPainter painter(this);

    brush = QBrush(*linearGradient);
/*
    Qt::PenStyle style = Qt::PenStyle(Qt::SolidLine);
    //renderArea->setPen(QPen(Qt::SolidLine));
    renderArea->setPen( QPen(*linearGradient, 25) );
*/
    painter.setPen(Qt::NoPen);
    painter.setBrush(brush);
    painter.fillRect(QRect(0, 0, width() - 1, height() - 1), brush);
    //painter.fillRect(QRect(0, 0, width() - 1, height() - 1),QColor(255, 0, 0, 127));


}
