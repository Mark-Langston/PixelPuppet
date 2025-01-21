#ifndef PIXELPUPPETMAIN_H
#define PIXELPUPPETMAIN_H

#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include "canvas.h"
#include "NetworkManager.h"
#include "PaintTogetherDialog.h"
#include "sessioncreateddialog.h"  // Include the new session created dialog
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QCryptographicHash>
#include <QByteArray>

class PixelPuppetMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit PixelPuppetMain(QWidget *parent = nullptr);
    ~PixelPuppetMain();

private:
    Canvas *canvas;
    NetworkManager *networkManager;
    QLabel *userCountLabel;  // QLabel for user count

    void setupMenu();
    void setupToolbar();
    void resizeCanvas(int width, int height);
    void showAboutDialog();
    void updateUserCount(int guestCount);  // Method to update the user count
    QString generateSessionCode();  // Method to generate the session code
    QString base64Encode(const QByteArray& data); // Base64 encoding function
    QByteArray base64Decode(const QString& encodedString); // Base64 decoding function
    QString getLocalIPv6Address(); // New method to get the local IPv6 address

private slots:
    void onPaintTogetherClicked();
    void onSessionCreated(QString sessionCode);
    void onSessionJoined(QString sessionCode);

signals:
    void userCountUpdated(int guestCount);  // Signal to update user count in main
};

#endif // PIXELPUPPETMAIN_H
