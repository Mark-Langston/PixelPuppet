#ifndef PIXELPUPPETMAIN_H
#define PIXELPUPPETMAIN_H

#include <QMainWindow>
#include <QIcon>
#include <QSlider>  // Include the slider header
#include <QLabel>  // Include label for the slider

class Canvas;

class PixelPuppetMain : public QMainWindow {
    Q_OBJECT

public:
    explicit PixelPuppetMain(QWidget *parent = nullptr);

private:
    void setupMenu();
    void setupToolbar();
    QIcon loadIcon(const QString &iconName);  // Declare the loadIcon function
    void showAboutDialog();  // Declare the showAboutDialog function
    void resizeCanvas(int width, int height);  // Declare the resizeCanvas slot

    Canvas *canvas; // Central drawing canvas

    // New members to manage the brush size slider
    QSlider *brushSizeSlider;  // Slider to adjust brush size
    QLabel *brushSizeLabel;    // Label to display brush size text next to the slider
};

#endif // PIXELPUPPETMAIN_H
