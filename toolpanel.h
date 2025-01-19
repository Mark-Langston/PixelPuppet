#ifndef TOOLPANEL_H
#define TOOLPANEL_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>

class Canvas;
class LayerManager;

class ToolPanel : public QWidget {
    Q_OBJECT

public:
    explicit ToolPanel(Canvas *canvas, QWidget *parent = nullptr);

private:
    Canvas *canvas;
    LayerManager *layerManager;
    QComboBox *brushSizeBox;
    QPushButton *colorButton;
    QPushButton *importButton;
    QPushButton *exportButton;

    void setupUI();
};

#endif // TOOLPANEL_H
