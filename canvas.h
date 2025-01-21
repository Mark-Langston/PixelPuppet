#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QImage>
#include <QColor>
#include <QVector>
#include <QPoint>

class QPainter;
class QMouseEvent;
class QPaintEvent;
class QResizeEvent;
class QSlider;
class QColorDialog;

class Canvas : public QWidget {
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);

    void setBrushTool();
    void setEraserTool();
    void setFillTool();
    void setColorPickerTool();

    void setBrushSize(int size);
    void setBrushColor(const QColor &color);
    QColor getBrushColor() const;

    int getBrushSize() const;  // Declare the getter for brush size

    void addLayer();
    void removeLayer(int index);
    void setCurrentLayer(int index);
    void setLayerVisibility(int index, bool visible);
    int getLayerCount() const;
    int getCurrentLayer() const;

    void importImage(const QString &filePath);
    void exportImage(const QString &filePath);  // Remove const here
    void resizeCanvas(int width, int height);

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void initializeCanvas();
    void drawCheckeredBackground(QPainter &painter);

    void fillArea(const QPoint &clickPoint);  // Add the fillArea declaration

    QVector<QImage> layers;
    QVector<bool> layerVisibility;
    int currentLayer;
    int brushSize;
    QColor brushColor;
    bool drawing;
    QPoint lastPoint;
    enum Tool { Brush, Eraser, Fill, ColorPicker } currentTool;
};

#endif // CANVAS_H
