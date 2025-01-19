#ifndef BRUSH_H
#define BRUSH_H

#include <QPainter>
#include <QPoint>
#include <QColor>

class Brush {
public:
    enum Type { Normal, Spray, Square };

    static void draw(QPainter &painter, const QPoint &pos, int size, const QColor &color, Type type);

private:
    static void drawNormal(QPainter &painter, const QPoint &pos, int size, const QColor &color);
    static void drawSpray(QPainter &painter, const QPoint &pos, int size, const QColor &color);
    static void drawSquare(QPainter &painter, const QPoint &pos, int size, const QColor &color);
};

#endif // BRUSH_H
