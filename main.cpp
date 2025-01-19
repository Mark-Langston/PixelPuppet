#include "pixelpuppetmain.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    PixelPuppetMain mainWindow;
    mainWindow.show();

    return app.exec();
}
