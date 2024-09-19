#include "pixelpuppetmain.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QDockWidget>
#include <QWidget>
#include <QInputDialog>
#include <QMenuBar>
#include <QSizeGrip>
#include <QLabel>    // Added to display width/height
#include <QSlider>   // Added for resizing sliders
#include <QDebug>
#include <QIcon>
#include <cstdlib>   // For rand() in spray brush

PixelPuppetMain::PixelPuppetMain(QWidget *parent)
    : QMainWindow(parent), currentColor(Qt::black), drawing(false), brushSize(10), showGrid(false), currentBrushType(Normal), currentLayer(0) {

    qDebug() << "Starting PixelPuppet application...";

    // Set up the canvas
    canvas = QImage(800, 800, QImage::Format_RGB32);
    canvas.fill(Qt::white);

    // Add the initial layer
    layers.append(canvas);  // Start with one layer
    layerVisibility.append(true);  // The initial layer is visible by default

    // Set window properties
    setWindowTitle("PixelPuppet - Pixel Art Tool");

    // Show the window maximized
    resize(1200, 800);  // Open in a larger window

    // Initialize the dock widget with all the tools
    setupDockWidget();

    // Set up the file menu
    setupMenu();

    // Set up the resize handle in the bottom right corner
    setupResizeHandle();

    // Initialize the layer list with the first layer
    updateLayerList();
}

PixelPuppetMain::~PixelPuppetMain() {}

void PixelPuppetMain::setupMenu() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *loadAction = new QAction(tr("&Open"), this);
    fileMenu->addAction(loadAction);
    connect(loadAction, &QAction::triggered, this, &PixelPuppetMain::loadImage);

    QAction *setCanvasSizeAction = new QAction(tr("&Set Canvas Size"), this);
    fileMenu->addAction(setCanvasSizeAction);
    connect(setCanvasSizeAction, &QAction::triggered, this, &PixelPuppetMain::setCanvasSize);

    QAction *saveAction = new QAction(tr("&Export"), this);
    fileMenu->addAction(saveAction);
    connect(saveAction, &QAction::triggered, this, &PixelPuppetMain::saveImage);

    QAction *bucketFillAction = new QAction(tr("&Bucket Fill"), this);
    fileMenu->addAction(bucketFillAction);
    connect(bucketFillAction, &QAction::triggered, this, &PixelPuppetMain::bucketFill);
}

void PixelPuppetMain::loadImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "PNG Files (*.png);;JPEG Files (*.jpg)");
    if (!fileName.isEmpty()) {
        canvas.load(fileName);
        update();
    }
}

void PixelPuppetMain::setCanvasSize() {
    bool ok;
    int width = QInputDialog::getInt(this, tr("Canvas Width"), tr("Enter width:"), canvas.width(), 1, 5000, 1, &ok);
    if (ok) {
        int height = QInputDialog::getInt(this, tr("Canvas Height"), tr("Enter height:"), canvas.height(), 1, 5000, 1, &ok);
        if (ok) {
            canvas = QImage(width, height, QImage::Format_RGB32);
            canvas.fill(Qt::white);
            update();
        }
    }
}

void PixelPuppetMain::removeLayer() {
    if (layers.size() > 1) {
        int row = layerListWidget->currentRow();
        layers.removeAt(row);
        layerVisibility.removeAt(row);
        currentLayer = qMax(0, currentLayer - 1);  // Ensure currentLayer is valid after removal
        updateCanvas();
        updateLayerList();
    } else {
        qDebug() << "Cannot remove the last remaining layer!";
    }
}

void PixelPuppetMain::toggleLayerVisibility() {
    if (layerVisibility.size() > currentLayer) {
        layerVisibility[currentLayer] = !layerVisibility[currentLayer];
        update();
    }
}

void PixelPuppetMain::bucketFill() {
    QPainter painter(&layers[currentLayer]);
    painter.fillRect(layers[currentLayer].rect(), QBrush(currentColor));
    update();
}

void PixelPuppetMain::setupDockWidget() {
    toolDock = new QDockWidget("Tools", this);
    toolDock->setAllowedAreas(Qt::RightDockWidgetArea);
    toolDock->setFixedWidth(300);

    QWidget *toolWidget = new QWidget;
    QVBoxLayout *toolLayout = new QVBoxLayout;

    setupPalette(toolLayout);
    setupBrushSizes(toolLayout);
    setupBrushTypes(toolLayout);
    setupGridToggle(toolLayout);
    setupLayerToolbar(toolLayout);

    // Add Width and Height Display Labels
    widthLabel = new QLabel(QString("Width: %1 px").arg(canvas.width()));
    heightLabel = new QLabel(QString("Height: %1 px").arg(canvas.height()));
    toolLayout->addWidget(widthLabel);
    toolLayout->addWidget(heightLabel);

    // Add sliders for resizing the canvas
    widthSlider = new QSlider(Qt::Horizontal);
    widthSlider->setRange(100, 5000);
    widthSlider->setValue(canvas.width());
    connect(widthSlider, &QSlider::valueChanged, this, &PixelPuppetMain::updateCanvasSize);
    toolLayout->addWidget(widthSlider);

    heightSlider = new QSlider(Qt::Horizontal);
    heightSlider->setRange(100, 5000);
    heightSlider->setValue(canvas.height());
    connect(heightSlider, &QSlider::valueChanged, this, &PixelPuppetMain::updateCanvasSize);
    toolLayout->addWidget(heightSlider);

    // Add the Import Image button
    QPushButton *importButton = new QPushButton("Import Image");
    connect(importButton, &QPushButton::clicked, this, &PixelPuppetMain::importImage);
    toolLayout->addWidget(importButton);

    // Add the Export Image button
    QPushButton *exportButton = new QPushButton("Export Image");
    connect(exportButton, &QPushButton::clicked, this, &PixelPuppetMain::saveImage);
    toolLayout->addWidget(exportButton);

    toolWidget->setLayout(toolLayout);
    toolDock->setWidget(toolWidget);
    addDockWidget(Qt::RightDockWidgetArea, toolDock);
}

void PixelPuppetMain::setupLayerToolbar(QVBoxLayout *toolLayout) {
    layerListWidget = new QListWidget();
    connect(layerListWidget, &QListWidget::currentRowChanged, this, &PixelPuppetMain::layerSelectionChanged);
    toolLayout->addWidget(layerListWidget);

    QPushButton *addLayerButton = new QPushButton("Add Layer");
    connect(addLayerButton, &QPushButton::clicked, this, &PixelPuppetMain::addLayer);
    toolLayout->addWidget(addLayerButton);

    QPushButton *removeLayerButton = new QPushButton();
    removeLayerButton->setIcon(QIcon::fromTheme("edit-delete", QIcon(":/icons/trash.png")));
    removeLayerButton->setToolTip("Delete Selected Layer");
    connect(removeLayerButton, &QPushButton::clicked, this, &PixelPuppetMain::deleteSelectedLayer);
    toolLayout->addWidget(removeLayerButton);
}

void PixelPuppetMain::updateLayerList() {
    layerListWidget->clear();
    for (int i = 0; i < layers.size(); ++i) {
        layerListWidget->addItem(QString("Layer %1").arg(i + 1));
    }
    layerListWidget->setCurrentRow(currentLayer);  // Ensure current layer is selected
}

void PixelPuppetMain::deleteSelectedLayer() {
    int row = layerListWidget->currentRow();

    if (layers.size() > 1 && row >= 0 && row < layers.size()) {  // Added check to prevent crashing
        layers.removeAt(row);
        layerVisibility.removeAt(row);
        currentLayer = qMax(0, currentLayer - 1);  // Ensure currentLayer is valid after deletion
        updateCanvas();
        updateLayerList();
    } else {
        qDebug() << "Cannot remove this layer or invalid layer selected!";
    }
}

void PixelPuppetMain::layerSelectionChanged() {
    int selectedRow = layerListWidget->currentRow();
    if (selectedRow >= 0 && selectedRow < layers.size()) {
        currentLayer = selectedRow;
    } else {
        qDebug() << "Invalid layer selection!";
    }
}

void PixelPuppetMain::updateCanvasSize() {
    int newWidth = widthSlider->value();
    int newHeight = heightSlider->value();

    // Update the canvas size for all layers
    for (int i = 0; i < layers.size(); ++i) {
        QImage resizedLayer = layers[i].scaled(newWidth, newHeight);
        layers[i] = resizedLayer;
    }

    // Update the labels
    widthLabel->setText(QString("Width: %1 px").arg(newWidth));
    heightLabel->setText(QString("Height: %1 px").arg(newHeight));

    update();
}

void PixelPuppetMain::setupResizeHandle() {
    sizeGrip = new QSizeGrip(this);
    sizeGrip->setVisible(true);
    sizeGrip->setGeometry(width() - 20, height() - 20, 20, 20);
}

void PixelPuppetMain::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    sizeGrip->move(width() - 20, height() - 20);
    update();
}

void PixelPuppetMain::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    updateCanvas();  // Ensure all layers are composited
    painter.drawImage(0, 0, canvas);

    if (showGrid) {
        QPen gridPen(Qt::lightGray, 1, Qt::DashLine);
        painter.setPen(gridPen);
        int gridSize = 10;

        for (int i = 0; i < canvas.width(); i += gridSize) {
            painter.drawLine(i, 0, i, canvas.height());
        }

        for (int i = 0; i < canvas.height(); i += gridSize) {
            painter.drawLine(0, i, canvas.width(), i);
        }
    }
}

void PixelPuppetMain::updateCanvas() {
    canvas.fill(Qt::white);  // Reset the composite canvas
    QPainter painter(&canvas);
    for (int i = 0; i < layers.size(); ++i) {
        if (layerVisibility[i]) {
            painter.drawImage(0, 0, layers[i]);
        }
    }
    update();
}

void PixelPuppetMain::addLayer() {
    QImage newLayer(canvas.size(), QImage::Format_ARGB32_Premultiplied);
    newLayer.fill(Qt::transparent);
    layers.append(newLayer);
    layerVisibility.append(true);  // Make the new layer visible by default
    currentLayer = layers.size() - 1;
    updateLayerList();  // Update the layer list after adding a new layer
    update();
}

void PixelPuppetMain::saveImage() {
    QString fileName = QFileDialog::getSaveFileName(this, "Export Image", "", "PNG Files (*.png);;JPEG Files (*.jpg)");
    if (!fileName.isEmpty()) {
        if (fileName.endsWith(".png")) {
            canvas.save(fileName, "PNG");
        } else if (fileName.endsWith(".jpg")) {
            canvas.save(fileName, "JPG");
        }
    }
}

void PixelPuppetMain::importImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Import Image", "", "PNG Files (*.png);;JPEG Files (*.jpg)");
    if (!fileName.isEmpty()) {
        QImage importedImage(fileName);
        if (!importedImage.isNull()) {
            addLayer();  // Add a new layer for the imported image
            layers[currentLayer] = importedImage.scaled(canvas.size());  // Scale the imported image to fit the canvas
            update();
        }
    }
}

void PixelPuppetMain::chooseColor() {
    QColor color = QColorDialog::getColor(currentColor, this, "Select Color");
    if (color.isValid()) {
        currentColor = color;
    }
}

void PixelPuppetMain::setBrushType(int index) {
    currentBrushType = static_cast<BrushType>(index);
}

void PixelPuppetMain::setupBrushSizes(QVBoxLayout *toolLayout) {
    QComboBox *brushSizeComboBox = new QComboBox;
    brushSizeComboBox->addItem("10px", 10);
    brushSizeComboBox->addItem("20px", 20);
    brushSizeComboBox->addItem("30px", 30);
    brushSizeComboBox->addItem("40px", 40);
    brushSizeComboBox->setCurrentIndex(0);

    connect(brushSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        brushSize = brushSizeComboBox->itemData(index).toInt();
    });

    toolLayout->addWidget(brushSizeComboBox);
}

void PixelPuppetMain::setupBrushTypes(QVBoxLayout *toolLayout) {
    QComboBox *brushTypeComboBox = new QComboBox;
    brushTypeComboBox->addItem("Normal Brush", Normal);
    brushTypeComboBox->addItem("Spray Paint", Spray);
    brushTypeComboBox->addItem("Square Brush", Square);
    brushTypeComboBox->setCurrentIndex(0);  // Default brush: Normal

    connect(brushTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PixelPuppetMain::setBrushType);

    toolLayout->addWidget(brushTypeComboBox);
}

void PixelPuppetMain::setupPalette(QVBoxLayout *toolLayout) {
    QHBoxLayout *paletteLayout = new QHBoxLayout;

    QColor colors[] = {
        Qt::black, Qt::white, Qt::red, Qt::green, Qt::blue, Qt::yellow,
        QColor(255, 165, 0), QColor(128, 0, 128), QColor(0, 255, 255), QColor(255, 20, 147),
        QColor(255, 105, 180), QColor(139, 69, 19), QColor(64, 224, 208), QColor(218, 165, 32)
    };

    for (const QColor &color : colors) {
        QPushButton *colorButton = new QPushButton;
        colorButton->setStyleSheet(QString("background-color: %1").arg(color.name()));
        colorButton->setFixedSize(30, 30);

        connect(colorButton, &QPushButton::clicked, this, [=]() {
            currentColor = color;
        });

        paletteLayout->addWidget(colorButton);
    }

    toolLayout->addLayout(paletteLayout);
}

void PixelPuppetMain::setupGridToggle(QVBoxLayout *toolLayout) {
    QCheckBox *gridCheckBox = new QCheckBox("Show Grid");
    connect(gridCheckBox, &QCheckBox::stateChanged, this, [=](int state) {
        showGrid = (state == Qt::Checked);
        update();
    });

    toolLayout->addWidget(gridCheckBox);
}

void PixelPuppetMain::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && layers.size() > currentLayer) {
        drawing = true;
        lastPoint = event->pos();
        QPainter painter(&layers[currentLayer]);

        switch (currentBrushType) {
        case Normal:
            drawNormalBrush(painter, event->pos());
            break;
        case Spray:
            drawSprayBrush(painter, event->pos());
            break;
        case Square:
            drawSquareBrush(painter, event->pos());
            break;
        }

        update();
    }
}

void PixelPuppetMain::mouseMoveEvent(QMouseEvent *event) {
    if (drawing && (event->buttons() & Qt::LeftButton) && layers.size() > currentLayer) {
        QPainter painter(&layers[currentLayer]);

        switch (currentBrushType) {
        case Normal:
            drawNormalBrush(painter, event->pos());
            break;
        case Spray:
            drawSprayBrush(painter, event->pos());
            break;
        case Square:
            drawSquareBrush(painter, event->pos());
            break;
        }

        lastPoint = event->pos();
        update();
    }
}

void PixelPuppetMain::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawing = false;
    }
}

void PixelPuppetMain::drawNormalBrush(QPainter &painter, const QPoint &pos) {
    painter.setPen(QPen(currentColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawPoint(pos);
}

void PixelPuppetMain::drawSprayBrush(QPainter &painter, const QPoint &pos) {
    painter.setPen(QPen(currentColor, 1));  // Use 1px pen for spray particles
    int radius = brushSize / 2;
    for (int i = 0; i < 100; ++i) {  // Spray particles count
        int dx = (rand() % (2 * radius)) - radius;
        int dy = (rand() % (2 * radius)) - radius;
        if (dx * dx + dy * dy <= radius * radius) {
            painter.drawPoint(pos + QPoint(dx, dy));
        }
    }
}

void PixelPuppetMain::drawSquareBrush(QPainter &painter, const QPoint &pos) {
    painter.setPen(QPen(currentColor, 1));  // Set to 1px pen for square filling
    painter.setBrush(QBrush(currentColor));
    painter.drawRect(pos.x() - brushSize / 2, pos.y() - brushSize / 2, brushSize, brushSize);
}
