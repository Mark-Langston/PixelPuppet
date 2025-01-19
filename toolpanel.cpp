#include "toolpanel.h"
#include "canvas.h"
#include "layermanager.h"

#include <QColorDialog>
#include <QFileDialog>

ToolPanel::ToolPanel(Canvas *canvas, QWidget *parent)
    : QWidget(parent), canvas(canvas) {
    setupUI();
}

void ToolPanel::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Brush Size Selector
    brushSizeBox = new QComboBox(this);
    brushSizeBox->addItem("Small", 5);
    brushSizeBox->addItem("Medium", 10);
    brushSizeBox->addItem("Large", 20);
    layout->addWidget(brushSizeBox);
    connect(brushSizeBox, &QComboBox::currentIndexChanged, this, [=](int index) {
        canvas->setBrushSize(brushSizeBox->itemData(index).toInt());
    });

    // Brush Color Selector
    colorButton = new QPushButton("Select Color", this);
    layout->addWidget(colorButton);
    connect(colorButton, &QPushButton::clicked, this, [=]() {
        QColor color = QColorDialog::getColor(canvas->getBrushColor(), this, tr("Select Brush Color"));
        if (color.isValid()) {
            canvas->setBrushColor(color);
        }
    });

    // Import Button
    importButton = new QPushButton("Import Image", this);
    layout->addWidget(importButton);
    connect(importButton, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Import Image", "", "Images (*.png *.jpg *.bmp)");
        if (!fileName.isEmpty()) {
            canvas->importImage(fileName);
        }
    });

    // Export Button
    exportButton = new QPushButton("Export Image", this);
    layout->addWidget(exportButton);
    connect(exportButton, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Export Image", "", "Images (*.png *.jpg *.bmp)");
        if (!fileName.isEmpty()) {
            canvas->exportImage(fileName);
        }
    });

    // Add Layer Manager
    layerManager = new LayerManager(canvas, this);
    layout->addWidget(layerManager);

    setLayout(layout);
}
