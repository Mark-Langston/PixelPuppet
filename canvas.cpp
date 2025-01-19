#include "canvas.h"
#include <QPainter>
#include <QMouseEvent>
#include <QFile>
#include <QImageReader>
#include <QImageWriter>
#include <QQueue>  // Include QQueue for flood fill algorithm
#include <QColorDialog>  // Include QColorDialog for color picking

Canvas::Canvas(QWidget *parent)
    : QWidget(parent),
    currentLayer(0),
    brushSize(10),
    brushColor(Qt::black),
    drawing(false),
    currentTool(Brush) {
    setAttribute(Qt::WA_StaticContents);
    setMinimumSize(1200, 800);  // Ensure the canvas has a minimum size
    initializeCanvas();
    setFixedSize(1200, 800);  // Fix the size of the canvas to match the window size
}

void Canvas::initializeCanvas() {
    QImage firstLayer(size(), QImage::Format_ARGB32_Premultiplied);
    firstLayer.fill(Qt::white);
    layers.append(firstLayer);
    layerVisibility.append(true);
    update();
}

// Brush settings
void Canvas::setBrushTool() { currentTool = Brush; }
void Canvas::setEraserTool() { currentTool = Eraser; }
void Canvas::setFillTool() { currentTool = Fill; }
void Canvas::setColorPickerTool() { currentTool = ColorPicker; }

void Canvas::setBrushSize(int size) { brushSize = size; }
int Canvas::getBrushSize() const { return brushSize; }  // Getter for brush size

void Canvas::setBrushColor(const QColor &color) { brushColor = color; }
QColor Canvas::getBrushColor() const { return brushColor; }

void Canvas::resizeCanvas(int width, int height) {
    setFixedSize(width, height);  // Adjust the canvas size without stretching
    QImage resizedLayer(width, height, QImage::Format_ARGB32_Premultiplied);
    resizedLayer.fill(Qt::transparent);
    layers[0] = resizedLayer; // Resize the first layer
    update();
}

// Layer management
void Canvas::addLayer() {
    QImage newLayer(size(), QImage::Format_ARGB32_Premultiplied);
    newLayer.fill(Qt::transparent);
    layers.append(newLayer);
    layerVisibility.append(true);
    currentLayer = layers.size() - 1;
    update();
}

void Canvas::removeLayer(int index) {
    if (index >= 0 && index < layers.size()) {
        layers.removeAt(index);
        layerVisibility.removeAt(index);
        if (currentLayer >= layers.size()) {
            currentLayer = layers.size() - 1;
        }
        update();
    }
}

void Canvas::setCurrentLayer(int index) {
    if (index >= 0 && index < layers.size()) {
        currentLayer = index;
    }
}

void Canvas::setLayerVisibility(int index, bool visible) {
    if (index >= 0 && index < layerVisibility.size()) {
        layerVisibility[index] = visible;
        update();
    }
}

int Canvas::getLayerCount() const { return layers.size(); }
int Canvas::getCurrentLayer() const { return currentLayer; }

void Canvas::importImage(const QString &filePath) {
    QImageReader reader(filePath);
    if (!reader.canRead()) {
        return;
    }

    QImage image = reader.read();
    if (!image.isNull()) {
        layers.clear();
        layerVisibility.clear();

        QImage resizedImage(size(), QImage::Format_ARGB32_Premultiplied);
        resizedImage.fill(Qt::transparent);
        QPainter painter(&resizedImage);
        painter.drawImage(0, 0, image);
        layers.append(resizedImage);
        layerVisibility.append(true);

        currentLayer = 0;
        update();
    }
}

void Canvas::exportImage(const QString &filePath) {
    QImage finalImage(size(), QImage::Format_ARGB32_Premultiplied);
    finalImage.fill(Qt::transparent);
    QPainter painter(&finalImage);

    for (int i = 0; i < layers.size(); ++i) {
        if (layerVisibility[i]) {
            painter.drawImage(0, 0, layers[i]);
        }
    }

    QImageWriter writer(filePath);
    writer.write(finalImage);
}

void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    drawCheckeredBackground(painter);

    for (int i = 0; i < layers.size(); ++i) {
        if (layerVisibility[i]) {
            painter.drawImage(0, 0, layers[i]);
        }
    }
}

void Canvas::resizeEvent(QResizeEvent *event) {
    for (int i = 0; i < layers.size(); ++i) {
        QImage resizedLayer(event->size(), QImage::Format_ARGB32_Premultiplied);
        resizedLayer.fill(Qt::transparent);
        QPainter painter(&resizedLayer);
        painter.drawImage(0, 0, layers[i]);
        layers[i] = resizedLayer;
    }
    update();
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    if (currentTool == Brush && event->button() == Qt::LeftButton) {
        if (rect().contains(event->pos())) {
            drawing = true;
            lastPoint = event->pos();
        }
    }
    if (currentTool == Eraser && event->button() == Qt::LeftButton) {
        if (rect().contains(event->pos())) {
            drawing = true;
            lastPoint = event->pos();
        }
    }

    if (currentTool == Fill && event->button() == Qt::LeftButton) {
        if (rect().contains(event->pos())) {
            fillArea(event->pos());
        }
    }

    if (currentTool == ColorPicker && event->button() == Qt::LeftButton) {
        if (rect().contains(event->pos())) {
            // Open the color dialog to pick a new color
            QColor newColor = QColorDialog::getColor(brushColor, this, "Pick Color");
            if (newColor.isValid()) {
                brushColor = newColor;
            }
        }
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    if (drawing && currentTool == Brush && event->buttons() & Qt::LeftButton) {
        if (rect().contains(event->pos())) {
            QPainter painter(&layers[currentLayer]);
            painter.setPen(QPen(brushColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(lastPoint, event->pos());
            lastPoint = event->pos();
            update();
        }
    }

    if (drawing && currentTool == Eraser && event->buttons() & Qt::LeftButton) {
        if (rect().contains(event->pos())) {
            QPainter painter(&layers[currentLayer]);
            painter.setCompositionMode(QPainter::CompositionMode_Clear);  // Set the erase mode
            painter.setPen(QPen(Qt::transparent, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));  // Make the eraser transparent
            painter.drawLine(lastPoint, event->pos());
            lastPoint = event->pos();
            update();
        }
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *event) {
    if (drawing && event->button() == Qt::LeftButton) {
        drawing = false;
    }
}

void Canvas::drawCheckeredBackground(QPainter &painter) {
    const int checkerSize = 10;
    QColor lightGray(200, 200, 200);
    QColor darkGray(150, 150, 150);

    for (int y = 0; y < height(); y += checkerSize) {
        for (int x = 0; x < width(); x += checkerSize) {
            painter.fillRect(x, y, checkerSize, checkerSize, (x / checkerSize + y / checkerSize) % 2 == 0 ? lightGray : darkGray);
        }
    }
}

void Canvas::fillArea(const QPoint &clickPoint) {
    QColor targetColor = layers[currentLayer].pixelColor(clickPoint);  // Get the clicked color
    if (targetColor == brushColor) return;  // No need to fill if the color is the same as the selected color

    // Use flood fill algorithm to fill the area with the selected color
    QImage &layer = layers[currentLayer];
    QQueue<QPoint> queue;
    queue.enqueue(clickPoint);

    while (!queue.isEmpty()) {
        QPoint point = queue.dequeue();
        if (point.x() < 0 || point.x() >= layer.width() || point.y() < 0 || point.y() >= layer.height()) {
            continue;  // Skip points outside the canvas
        }

        if (layer.pixelColor(point) == targetColor) {
            layer.setPixelColor(point, brushColor);

            // Enqueue the neighboring points
            queue.enqueue(QPoint(point.x() + 1, point.y()));
            queue.enqueue(QPoint(point.x() - 1, point.y()));
            queue.enqueue(QPoint(point.x(), point.y() + 1));
            queue.enqueue(QPoint(point.x(), point.y() - 1));
        }
    }

    update();
}
