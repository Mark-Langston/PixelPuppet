#include <QApplication>
#include <QIcon>
#include "pixelpuppetmain.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set application icon from file path
    app.setWindowIcon(QIcon("C:/Users/mark_/OneDrive/Desktop/PixelPuppet/PixelPuppet/build/Desktop_Qt_6_7_2_MinGW_64_bit-Debug/icon.png"));

    PixelPuppetMain window;
    window.show();

    return app.exec();
}
