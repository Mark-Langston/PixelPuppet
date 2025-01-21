#include "pixelpuppetmain.h"
#include "canvas.h"
#include "PaintTogetherDialog.h"
#include "NetworkManager.h"
#include "sessioncreateddialog.h"  // Include the new session created dialog

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
#include <QColorDialog>
#include <QStatusBar>
#include <QSpacerItem>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QByteArray>

PixelPuppetMain::PixelPuppetMain(QWidget *parent)
    : QMainWindow(parent), networkManager(new NetworkManager(this)) {
    // Use the system's default theme (light or dark)
    QApplication::setPalette(QApplication::style()->standardPalette());

    // Ensure Canvas is correctly created and used
    canvas = new Canvas(this);
    setCentralWidget(canvas);

    // Updated to load icons from the root directory
    setWindowIcon(QIcon("your_icon.svg"));

    setupMenu();
    setupToolbar();

    setWindowTitle("PixelPuppet");

    setMinimumSize(1200, 800);
    resize(1200, 800);

    // Connect to network manager signal for updating user count
    connect(networkManager, &NetworkManager::guestCountUpdated, this, &PixelPuppetMain::updateUserCount);
}

PixelPuppetMain::~PixelPuppetMain() {
    delete networkManager;  // Cleanup network manager
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

    // Options Menu
    QMenu *optionsMenu = menuBar()->addMenu(tr("&Options"));
    QAction *resizeAction = new QAction(tr("Resize Canvas"), this);
    connect(resizeAction, &QAction::triggered, this, [=]() {
        bool ok;
        int width = QInputDialog::getInt(this, tr("Set Canvas Width"), tr("Width:"), canvas->width(), 100, 5000, 1, &ok);
        if (!ok) return;

        int height = QInputDialog::getInt(this, tr("Set Canvas Height"), tr("Height:"), canvas->height(), 100, 5000, 1, &ok);
        if (ok) {
            resizeCanvas(width, height);
        }
    });
    optionsMenu->addAction(resizeAction);  // Add resize option to options menu

    // Paint Together Menu
    QAction *paintTogetherAction = new QAction(tr("Paint Together"), this);
    connect(paintTogetherAction, &QAction::triggered, this, &PixelPuppetMain::onPaintTogetherClicked);
    optionsMenu->addAction(paintTogetherAction);
}

void PixelPuppetMain::setupToolbar() {
    QToolBar *toolbar = addToolBar(tr("Tools"));

    // Updated to load icons from the root directory
    QAction *brushAction = new QAction(QIcon("brush.svg"), tr("Brush"), this);
    brushAction->setToolTip(tr("Use the brush tool to paint."));
    toolbar->addAction(brushAction);
    connect(brushAction, &QAction::triggered, canvas, &Canvas::setBrushTool);

    // Brush size slider
    QLabel *brushSizeLabel = new QLabel("Brush Size:", this);
    toolbar->addWidget(brushSizeLabel);

    QSlider *brushSizeSlider = new QSlider(Qt::Horizontal, this);
    brushSizeSlider->setRange(1, 100);  // Brush size range
    brushSizeSlider->setValue(canvas->getBrushSize());  // Set initial value
    brushSizeSlider->setFixedWidth(120);
    toolbar->addWidget(brushSizeSlider);
    connect(brushSizeSlider, &QSlider::valueChanged, canvas, &Canvas::setBrushSize);

    QAction *eraserAction = new QAction(QIcon("eraser.svg"), tr("Eraser"), this);
    eraserAction->setToolTip(tr("Use the eraser tool to erase parts of your drawing."));
    toolbar->addAction(eraserAction);
    connect(eraserAction, &QAction::triggered, canvas, &Canvas::setEraserTool);

    QAction *fillAction = new QAction(QIcon("fill.svg"), tr("Fill"), this);
    fillAction->setToolTip(tr("Fill an area with color."));
    toolbar->addAction(fillAction);
    connect(fillAction, &QAction::triggered, canvas, &Canvas::setFillTool);

    // Color picker action
    QAction *colorPickerAction = new QAction(QIcon("color-picker.svg"), tr("Color Picker"), this);
    colorPickerAction->setToolTip(tr("Pick a color from the canvas."));
    toolbar->addAction(colorPickerAction);

    connect(colorPickerAction, &QAction::triggered, this, [=]() {
        QColor newColor = QColorDialog::getColor(canvas->getBrushColor(), this, tr("Pick Color"));
        if (newColor.isValid()) {
            canvas->setBrushColor(newColor);  // Update brush color with the selected color
        }
    });

    // Add a spacer to push the user count label to the right
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    toolbar->addWidget(spacer);

    // Add user count label to the toolbar (Changed from "Guests" to "Users")
    userCountLabel = new QLabel("Users: 1", this);  // Initialize with the host as 1 user
    toolbar->addWidget(userCountLabel);
}

void PixelPuppetMain::onPaintTogetherClicked() {
    PaintTogetherDialog *dialog = new PaintTogetherDialog(this, networkManager);
    connect(dialog, &PaintTogetherDialog::sessionCreated, this, &PixelPuppetMain::onSessionCreated);
    connect(dialog, &PaintTogetherDialog::sessionJoined, this, &PixelPuppetMain::onSessionJoined);
    dialog->exec();
}

void PixelPuppetMain::onSessionCreated(QString sessionCode) {
    // Generate the session code here
    QString code = generateSessionCode();

    // Debugging output
    qDebug() << "Session Code Generated: " << code;

    // Show the session code popup only once and pass the code to the dialog
    SessionCreatedDialog *dialog = new SessionCreatedDialog(this, code);
    dialog->exec();

    // Alternatively, show it in the PaintTogetherDialog
    PaintTogetherDialog *paintDialog = new PaintTogetherDialog(this, networkManager);
    paintDialog->setSessionCode(code);  // Set session code into PaintTogetherDialog
    paintDialog->exec();
}

void PixelPuppetMain::onSessionJoined(QString sessionCode) {
    // Debugging output
    qDebug() << "Session Code Received: " << sessionCode;

    QString ipv6Address;
    int port;
    if (networkManager->decodeSessionCode(sessionCode, ipv6Address, port)) {
        qDebug() << "Connecting to " << ipv6Address << " on port " << port;
        networkManager->connectToServer(ipv6Address, port);  // Pass both IP and port to connectToServer
    } else {
        QMessageBox::warning(this, "Invalid Session Code", "The session code is invalid.");
    }
}

QString PixelPuppetMain::generateSessionCode() {
    QString ipv6Address = getLocalIPv6Address(); // Fetch the actual method to get the host's IPv6 address
    int port = 8080; // Replace with your chosen port
    QString sessionCode = ipv6Address + ":" + QString::number(port);

    return base64Encode(sessionCode.toUtf8());
}

// Add the getLocalIPv6Address method
QString PixelPuppetMain::getLocalIPv6Address() {
    foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces()) {
        // Only consider interfaces that are up and running
        if (interface.flags() & QNetworkInterface::IsUp) {
            foreach (const QNetworkAddressEntry &entry, interface.addressEntries()) {
                // Check for a global IPv6 address (not link-local)
                if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol &&
                    !entry.ip().toString().startsWith("fe80")) {  // Exclude link-local addresses
                    return entry.ip().toString();
                }
            }
        }
    }
    return QString();  // Return empty if no global IPv6 address is found
}

QString PixelPuppetMain::base64Encode(const QByteArray& data) {
    return QString::fromUtf8(data.toBase64());
}

QByteArray PixelPuppetMain::base64Decode(const QString& encodedString) {
    return QByteArray::fromBase64(encodedString.toUtf8());
}

// Update the user count (including host) displayed in the toolbar
void PixelPuppetMain::updateUserCount(int guestCount) {
    userCountLabel->setText("Users: " + QString::number(guestCount + 1));  // Add 1 to include the host
}

void PixelPuppetMain::showAboutDialog() {
    QMessageBox::information(this, tr("About PixelPuppet"),
                             tr("Material Icons are made available under the Apache License 2.0.\n"
                                "See: https://www.apache.org/licenses/LICENSE-2.0\n\n"
                                "Icons from Google Material Icons, licensed under Apache 2.0.\n"
                                "All other icons in the app are used from the open-source community.\n\n"
                                "PixelPuppet - A Pixel Art Painting Application"));
}

// Resizing the canvas widget
void PixelPuppetMain::resizeCanvas(int width, int height) {
    canvas->resize(width, height);  // Resize the canvas widget
    canvas->resizeCanvas(width, height);  // Adjust internal canvas size
}
