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
#include <QSlider>
#include <QLabel>
#include <QDebug>  // For debugging output

PixelPuppetMain::PixelPuppetMain(QWidget *parent)
    : QMainWindow(parent), currentColor(Qt::black), drawing(false), brushSize(10), showGrid(false) {

    // Debug output to verify that the application starts
    qDebug() << "Starting PixelPuppet application...";

    // Set up the canvas
    canvas = QImage(800, 800, QImage::Format_RGB32);
    canvas.fill(Qt::white);  // Ensure canvas has a white background

    // Set window properties and set an initial larger size
    setWindowTitle("PixelPuppet - Pixel Art Tool");
    resize(1200, 800);  // Open with a larger window size

    // Initialize the dock widget with all the tools
    setupDockWidget();

    // Set up the file menu (optional, we'll move the Export button to the dock)
    setupMenu();

    // Set up the resize handle in the bottom right corner
    setupResizeHandle();
}

PixelPuppetMain::~PixelPuppetMain() {}

void PixelPuppetMain::setupDockWidget() {
    // Create the dock widget for the tools
    toolDock = new QDockWidget("Tools", this);
    toolDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    toolDock->setFloating(false); 

    toolDock->setFixedWidth(200);  // Adjust the size to make it smaller

    // Create a container widget to hold all the tool elements
    QWidget *toolWidget = new QWidget;

    // Create a vertical layout for the tools inside the dock widget
    QVBoxLayout *toolLayout = new QVBoxLayout;

    // Set up the color palette, brush sizes, grid toggle, canvas scaler, and export button
    setupPalette(toolLayout);
    setupBrushSizes(toolLayout);
    setupGridToggle(toolLayout);
    setupCanvasScaler(toolLayout);
    setupExportButton(toolLayout);

    // Set the layout for the container widget
    toolWidget->setLayout(toolLayout);

    // Set the widget to the dock and dock it to the left
    toolDock->setWidget(toolWidget);
    addDockWidget(Qt::LeftDockWidgetArea, toolDock);
}

void PixelPuppetMain::setupResizeHandle() {
    // Create a resize handle in the bottom-right corner
    sizeGrip = new QSizeGrip(this);
    sizeGrip->setVisible(true);

    // Move the size grip to the bottom-right corner
    sizeGrip->setGeometry(width() - 20, height() - 20, 20, 20);
}

void PixelPuppetMain::setupPalette(QVBoxLayout *toolLayout) {
    QHBoxLayout *paletteLayout = new QHBoxLayout;

    // Comprehensive list of colors
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

    toolLayout->addLayout(paletteLayout);  // Add palette to the tool layout
}

void PixelPuppetMain::setupBrushSizes(QVBoxLayout *toolLayout) {
    QComboBox *brushSizeComboBox = new QComboBox;
    brushSizeComboBox->addItem("10px", 10);
    brushSizeComboBox->addItem("20px", 20);
    brushSizeComboBox->addItem("30px", 30);
    brushSizeComboBox->addItem("40px", 40);
    brushSizeComboBox->setCurrentIndex(0);  // Default brush size: 10px

    connect(brushSizeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        brushSize = brushSizeComboBox->itemData(index).toInt();
    });

    toolLayout->addWidget(brushSizeComboBox);  // Add brush size selector to tool layout
}

void PixelPuppetMain::setupGridToggle(QVBoxLayout *toolLayout) {
    QCheckBox *gridCheckBox = new QCheckBox("Show Grid");
    connect(gridCheckBox, &QCheckBox::stateChanged, this, [=](int state) {
        showGrid = (state == Qt::Checked);
        update();
    });

    toolLayout->addWidget(gridCheckBox);  // Add grid toggle to tool layout
}

void PixelPuppetMain::setupCanvasScaler(QVBoxLayout *toolLayout) {
    QLabel *canvasSizeLabel = new QLabel("Canvas Size:");
    toolLayout->addWidget(canvasSizeLabel);

    QSlider *widthSlider = new QSlider(Qt::Horizontal);
    widthSlider->setRange(100, 5000);
    widthSlider->setValue(canvas.width());
    toolLayout->addWidget(new QLabel("Width"));
    toolLayout->addWidget(widthSlider);

    QSlider *heightSlider = new QSlider(Qt::Horizontal);
    heightSlider->setRange(100, 5000);
    heightSlider->setValue(canvas.height());
    toolLayout->addWidget(new QLabel("Height"));
    toolLayout->addWidget(heightSlider);

    connect(widthSlider, &QSlider::valueChanged, this, [=](int value) {
        canvas = QImage(value, canvas.height(), QImage::Format_RGB32);
        canvas.fill(Qt::white);
        update();
    });

    connect(heightSlider, &QSlider::valueChanged, this, [=](int value) {
        canvas = QImage(canvas.width(), value, QImage::Format_RGB32);
        canvas.fill(Qt::white);
        update();
    });
}

void PixelPuppetMain::setupExportButton(QVBoxLayout *toolLayout) {
    QPushButton *exportButton = new QPushButton("Export");
    connect(exportButton, &QPushButton::clicked, this, &PixelPuppetMain::saveImage);
    toolLayout->addWidget(exportButton);
}

// Set up the file menu
void PixelPuppetMain::setupMenu() {
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *setCanvasSizeAction = new QAction(tr("&Set Canvas Size"), this);
    fileMenu->addAction(setCanvasSizeAction);
    connect(setCanvasSizeAction, &QAction::triggered, this, &PixelPuppetMain::setCanvasSize);

    QAction *saveAction = new QAction(tr("&Export"), this);
    fileMenu->addAction(saveAction);
    connect(saveAction, &QAction::triggered, this, &PixelPuppetMain::saveImage);
}

void PixelPuppetMain::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawing = true;
        lastPoint = event->pos();
        qDebug() << "Mouse pressed at:" << lastPoint;  // Debug output for mouse press event
        QPainter painter(&canvas);
        painter.setPen(QPen(currentColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawPoint(event->pos());
        update();
    }
}

void PixelPuppetMain::mouseMoveEvent(QMouseEvent *event) {
    if (drawing && (event->buttons() & Qt::LeftButton)) {
        qDebug() << "Mouse moved to:" << event->pos();  // Debug output for mouse move event
        QPainter painter(&canvas);
        painter.setPen(QPen(currentColor, brushSize, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(lastPoint, event->pos());
        lastPoint = event->pos();
        update();
    }
}

void PixelPuppetMain::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        drawing = false;
        qDebug() << "Mouse released";  // Debug output for mouse release event
    }
}

void PixelPuppetMain::paintEvent(QPaintEvent *event) {
    qDebug() << "paintEvent called";  // Debug print for paint event
    QPainter painter(this);

    // Ensure the canvas is valid
    if (!canvas.isNull()) {
        painter.drawImage(0, 0, canvas);
    } else {
        qDebug() << "Canvas is null!";
    }

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

    qDebug() << "Painting completed";  // Debug statement to confirm painting
}

// Allow the user to set the canvas size
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

// Implementation of chooseColor()
void PixelPuppetMain::chooseColor() {
    QColor color = QColorDialog::getColor(currentColor, this, "Select Color");
    if (color.isValid()) {
        currentColor = color;
    }
}

// Implementation of loadImage()
void PixelPuppetMain::loadImage() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "PNG Files (*.png);;JPEG Files (*.jpg)");
    if (!fileName.isEmpty()) {
        canvas.load(fileName);
        update();  // Redraw the canvas with the loaded image
    }
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
