#include "brush.h"
#include <cstdlib> // For random number generation in spray brush

void Brush::draw(QPainter &painter, const QPoint &pos, int size, const QColor &color, Type type) {
    switch (type) {
    case Normal:
        drawNormal(painter, pos, size, color);
        break;
    case Spray:
        drawSpray(painter, pos, size, color);
        break;
    case Square:
        drawSquare(painter, pos, size, color);
        break;
    }
}

void Brush::drawNormal(QPainter &painter, const QPoint &pos, int size, const QColor &color) {
    painter.setPen(QPen(color, size, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPoint(pos);
}

void Brush::drawSpray(QPainter &painter, const QPoint &pos, int size, const QColor &color) {
    painter.setPen(QPen(color, 1));
    int radius = size / 2;
    for (int i = 0; i < 100; ++i) {
        int dx = (rand() % (2 * radius)) - radius;
        int dy = (rand() % (2 * radius)) - radius;
        if (dx * dx + dy * dy <= radius * radius) {
            painter.drawPoint(pos + QPoint(dx, dy));
        }
    }
}

void Brush::drawSquare(QPainter &painter, const QPoint &pos, int size, const QColor &color) {
    painter.setBrush(QBrush(color));
    painter.drawRect(pos.x() - size / 2, pos.y() - size / 2, size, size);
}
