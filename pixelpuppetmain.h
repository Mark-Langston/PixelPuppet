#ifndef PIXELPUPPETMAIN_H
#define PIXELPUPPETMAIN_H

#include <QMainWindow>
#include <QColor>
#include <QImage>
#include <QPainter>
#include <QMouseEvent>
#include <QDockWidget>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QListWidget>
#include <QSizeGrip>
#include <QLabel>       // Added for width/height display
#include <QSlider>      // Added for resizing sliders

class PixelPuppetMain : public QMainWindow {
    Q_OBJECT

public:
    explicit PixelPuppetMain(QWidget *parent = nullptr);
    ~PixelPuppetMain();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void chooseColor();
    void saveImage();
    void loadImage();
    void setCanvasSize();
    void addLayer();
    void removeLayer();
    void toggleLayerVisibility();
    void setBrushType(int index);
    void bucketFill();
    void importImage();
    void deleteSelectedLayer();
    void layerSelectionChanged();
    void updateCanvasSize();  // New slot for updating canvas size when sliders change

private:
    enum BrushType { Normal, Spray, Square };
    void setupPalette(QVBoxLayout *toolLayout);
    void setupBrushSizes(QVBoxLayout *toolLayout);
    void setupBrushTypes(QVBoxLayout *toolLayout);
    void setupGridToggle(QVBoxLayout *toolLayout);
    void setupLayerToolbar(QVBoxLayout *toolLayout);
    void setupDockWidget();
    void setupMenu();
    void setupResizeHandle();
    void updateCanvas();
    void updateLayerList();
    void drawNormalBrush(QPainter &painter, const QPoint &pos);
    void drawSprayBrush(QPainter &painter, const QPoint &pos);
    void drawSquareBrush(QPainter &painter, const QPoint &pos);

    QColor currentColor;
    QImage canvas;
    QPoint lastPoint;
    int brushSize;
    bool drawing;
    bool showGrid;
    BrushType currentBrushType;
    QDockWidget *toolDock;
    QSizeGrip *sizeGrip;

    QList<QImage> layers;
    QList<bool> layerVisibility;
    int currentLayer;

    QListWidget *layerListWidget;

    QLabel *widthLabel;  // Added to display canvas width
    QLabel *heightLabel; // Added to display canvas height
    QSlider *widthSlider;  // Slider for width
    QSlider *heightSlider; // Slider for height
};

#endif // PIXELPUPPETMAIN_H
