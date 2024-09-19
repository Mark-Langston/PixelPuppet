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
#include <QSizeGrip>
#include <QSlider>

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

private slots:
    void chooseColor();
    void saveImage();
    void loadImage();
    void setCanvasSize();

private:
    void setupPalette(QVBoxLayout *toolLayout);     // Function to set up color palette
    void setupBrushSizes(QVBoxLayout *toolLayout);  // Function to set up brush sizes
    void setupGridToggle(QVBoxLayout *toolLayout);  // Function to set up grid toggle
    void setupDockWidget();                         // Function to set up the dock widget
    void setupMenu();                               // Function to set up the file menu
    void setupResizeHandle();                       // Function to add resizable handle
    void setupCanvasScaler(QVBoxLayout *toolLayout); // Function to add canvas scaler
    void setupExportButton(QVBoxLayout *toolLayout); // Function to add export button

    QColor currentColor;       // Currently selected color
    QImage canvas;             // Canvas for drawing
    QPoint lastPoint;          // Last point drawn
    int brushSize;             // Current brush size
    bool drawing;              // Whether the user is currently drawing
    bool showGrid;             // Whether the grid is visible
    QDockWidget *toolDock;     // Dock widget for tools
    QSizeGrip *sizeGrip;       // Resizable handle at bottom right corner
};

#endif // PIXELPUPPETMAIN_H
