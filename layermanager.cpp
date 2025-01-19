#include "layermanager.h"
#include "canvas.h"

#include <QCheckBox>
#include <QHBoxLayout>
#include <QListWidgetItem>

LayerManager::LayerManager(Canvas *canvas, QWidget *parent)
    : QWidget(parent), canvas(canvas) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    layerList = new QListWidget(this);
    layout->addWidget(layerList);
    connect(layerList, &QListWidget::itemChanged, this, [=](QListWidgetItem *item) {
        toggleLayerVisibility(item);
    });

    QHBoxLayout *buttonLayout = new QHBoxLayout();

    addLayerButton = new QPushButton("Add Layer", this);
    buttonLayout->addWidget(addLayerButton);
    connect(addLayerButton, &QPushButton::clicked, this, &LayerManager::addLayer);

    removeLayerButton = new QPushButton("Remove Layer", this);
    buttonLayout->addWidget(removeLayerButton);
    connect(removeLayerButton, &QPushButton::clicked, this, &LayerManager::removeLayer);

    layout->addLayout(buttonLayout);
    setLayout(layout);

    updateLayerList();
}

void LayerManager::addLayer() {
    canvas->addLayer();
    updateLayerList();
}

void LayerManager::removeLayer() {
    int currentIndex = layerList->currentRow();
    if (currentIndex >= 0) {
        canvas->removeLayer(currentIndex);
        updateLayerList();
    }
}

void LayerManager::switchLayer() {
    int currentIndex = layerList->currentRow();
    if (currentIndex >= 0) {
        canvas->setCurrentLayer(currentIndex);
    }
}

void LayerManager::toggleLayerVisibility(QListWidgetItem *item) {
    int index = layerList->row(item);
    bool isVisible = item->checkState() == Qt::Checked;
    canvas->setLayerVisibility(index, isVisible);
}

void LayerManager::updateLayerList() {
    layerList->clear();
    int layerCount = canvas->getLayerCount();
    for (int i = 0; i < layerCount; ++i) {
        QListWidgetItem *item = new QListWidgetItem(QString("Layer %1").arg(i + 1), layerList);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked); // Default to visible
        layerList->addItem(item);
    }
    layerList->setCurrentRow(canvas->getCurrentLayer());
}
