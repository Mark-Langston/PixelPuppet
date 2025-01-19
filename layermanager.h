#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>

class Canvas;

class LayerManager : public QWidget {
    Q_OBJECT

public:
    explicit LayerManager(Canvas *canvas, QWidget *parent = nullptr);

private slots:
    void addLayer();
    void removeLayer();
    void switchLayer();
    void toggleLayerVisibility(QListWidgetItem *item);

private:
    Canvas *canvas;
    QListWidget *layerList;
    QPushButton *addLayerButton;
    QPushButton *removeLayerButton;
    void updateLayerList();
};

#endif // LAYERMANAGER_H
