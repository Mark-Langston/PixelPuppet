#include <QApplication>
#include <QIcon>
#include "pixelpuppetmain.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set application icon using a relative path
    app.setWindowIcon(QIcon("resources/icon.png"));

    PixelPuppetMain window;
    window.show();

    return app.exec();
}
