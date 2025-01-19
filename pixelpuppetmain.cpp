#include "pixelpuppetmain.h"
#include "canvas.h"

#include <QAction>
#include <QFileDialog>
#include <QMenuBar>
#include <QToolBar>
#include <QStyle>
#include <QMessageBox>
#include <QApplication>
#include <QIcon>
#include <QInputDialog>
#include <QLineEdit>
#include <QPalette>
#include <QSlider>
#include <QLabel>
#include <QColorDialog>  // Ensure QColorDialog is included

PixelPuppetMain::PixelPuppetMain(QWidget *parent) : QMainWindow(parent) {
    // Use the system's default theme (light or dark)
    QApplication::setPalette(QApplication::style()->standardPalette());

    canvas = new Canvas(this);
    setCentralWidget(canvas);

    setWindowIcon(QIcon(":/assets/icons/your_icon.svg"));

    setupMenu();
    setupToolbar();

    setWindowTitle("PixelPuppet");

    setMinimumSize(1200, 800);
    resize(1200, 800);
}

void PixelPuppetMain::resizeCanvas(int width, int height) {
    // Crop the canvas to the new size
    canvas->resize(width, height);  // Resize the canvas widget
    canvas->resizeCanvas(width, height);  // Adjust internal canvas size
}

void PixelPuppetMain::setupMenu() {
    // File Menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *loadAction = new QAction(style()->standardIcon(QStyle::SP_DialogOpenButton), tr("&Open"), this);
    loadAction->setToolTip(tr("Open an image file."));
    fileMenu->addAction(loadAction);
    connect(loadAction, &QAction::triggered, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "Images (*.png *.jpg *.bmp)");
        if (!fileName.isEmpty()) {
            canvas->importImage(fileName);
        }
    });

    QAction *saveAction = new QAction(style()->standardIcon(QStyle::SP_DialogSaveButton), tr("&Save"), this);
    saveAction->setToolTip(tr("Save the current image."));
    fileMenu->addAction(saveAction);
    connect(saveAction, &QAction::triggered, this, [=]() {
        QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "Images (*.png *.jpg *.bmp)");
        if (!fileName.isEmpty()) {
            canvas->exportImage(fileName);
        }
    });

    // Options Menu (where Resize Canvas will go)
    QMenu *optionsMenu = menuBar()->addMenu(tr("&Options"));
    QAction *resizeAction = new QAction(tr("Resize Canvas"), this);
    connect(resizeAction, &QAction::triggered, this, [=]() {
        // Prompt for new canvas dimensions
        bool ok;
        int width = QInputDialog::getInt(this, tr("Set Canvas Width"), tr("Width:"), canvas->width(), 100, 5000, 1, &ok);
        if (!ok) return;

        int height = QInputDialog::getInt(this, tr("Set Canvas Height"), tr("Height:"), canvas->height(), 100, 5000, 1, &ok);
        if (ok) {
            resizeCanvas(width, height);
        }
    });
    optionsMenu->addAction(resizeAction); // Add resize option to the options menu

    // Other Menu
    QMenu *otherMenu = menuBar()->addMenu(tr("&Other"));
    QAction *aboutAction = new QAction(tr("&About"), this);
    otherMenu->addAction(aboutAction);
    connect(aboutAction, &QAction::triggered, this, &PixelPuppetMain::showAboutDialog);
}

void PixelPuppetMain::setupToolbar() {
    QToolBar *toolbar = addToolBar(tr("Tools"));

    QAction *brushAction = new QAction(QIcon(":/assets/icons/brush.svg"), tr("Brush"), this);
    brushAction->setToolTip(tr("Use the brush tool to paint."));
    toolbar->addAction(brushAction);
    connect(brushAction, &QAction::triggered, canvas, &Canvas::setBrushTool);

    // Brush size slider
    QLabel *brushSizeLabel = new QLabel("Brush Size:", this);
    toolbar->addWidget(brushSizeLabel);

    QSlider *brushSizeSlider = new QSlider(Qt::Horizontal, this);
    brushSizeSlider->setRange(1, 100); // Brush size range
    brushSizeSlider->setValue(canvas->getBrushSize()); // Set initial value
    brushSizeSlider->setFixedWidth(120);  // Set the width of the slider to be smaller
    toolbar->addWidget(brushSizeSlider);
    connect(brushSizeSlider, &QSlider::valueChanged, canvas, &Canvas::setBrushSize);

    QAction *eraserAction = new QAction(QIcon(":/assets/icons/eraser.svg"), tr("Eraser"), this);
    eraserAction->setToolTip(tr("Use the eraser tool to erase parts of your drawing."));
    toolbar->addAction(eraserAction);
    connect(eraserAction, &QAction::triggered, canvas, &Canvas::setEraserTool);

    QAction *fillAction = new QAction(QIcon(":/assets/icons/fill.svg"), tr("Fill"), this);
    fillAction->setToolTip(tr("Fill an area with color."));
    toolbar->addAction(fillAction);
    connect(fillAction, &QAction::triggered, canvas, &Canvas::setFillTool);

    QAction *colorPickerAction = new QAction(QIcon(":/assets/icons/color-picker.svg"), tr("Color Picker"), this);
    colorPickerAction->setToolTip(tr("Pick a color from the canvas."));
    toolbar->addAction(colorPickerAction);
    connect(colorPickerAction, &QAction::triggered, canvas, &Canvas::setColorPickerTool);
}

// About Dialog - Function to Show Attribution Information
void PixelPuppetMain::showAboutDialog() {
    QMessageBox::information(this, tr("About PixelPuppet"),
                             tr("Material Icons are made available under the Apache License 2.0.\n"
                                "See: https://www.apache.org/licenses/LICENSE-2.0\n\n"
                                "Icons from Google Material Icons, licensed under Apache 2.0.\n"
                                "All other icons in the app are used from the open-source community.\n\n"
                                "PixelPuppet - A Pixel Art Painting Application"));
}
