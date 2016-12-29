#ifndef GRADIENTRENDERER_H
#define GRADIENTRENDERER_H

//#include "arthurwidgets.h"
#include <QBrush>
 #include <QPen>
 #include <QPixmap>
 #include <QWidget>
#include <QPainter>


class GradientRenderer : public QWidget
    {
        Q_OBJECT

    public:
        enum Shape { Line, Points, Polyline, Polygon, Rect, RoundedRect, Ellipse, Arc,
                     Chord, Pie, Path, Text, Pixmap };

        GradientRenderer(QWidget *parent = 0, const int aLength = 100);

        QSize minimumSizeHint() const;
        QSize sizeHint() const;

        void setColors( const QColor& c1, const QColor& c2 );

    public slots:
        void setShape(Shape shape);
        void setPen(const QPen &pen);
        void setBrush(const QBrush &brush);
        void setAntialiased(bool antialiased);
        void setTransformed(bool transformed);

    protected:
        void paintEvent(QPaintEvent *event);


    private:
        QLinearGradient* linearGradient;
        QColor c1, c2;
        Shape shape;
        QPen pen;
        QBrush brush;
        bool antialiased;
        bool transformed;
        QPixmap pixmap;
        int mLength;
};
#endif // GRADIENTRENDERER_H
