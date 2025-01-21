#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QNetworkInterface>

class NetworkManager : public QObject {
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    // Start server to host the session
    void startServer();

    // Connect to a server using a decoded session code
    void connectToServer(const QString &ipv6Address, int port);

    // Method to encode the IPv6 address and port to a session code
    QString encodeSessionCode(const QString &ipv6Address, int port);

    // Method to decode the session code back to IPv6 and port
    bool decodeSessionCode(const QString &sessionCode, QString &ipv6Address, int &port);

    // Method to get the local IPv6 address of the machine
    QString getLocalIPv6Address();

signals:
    // Signal for sending received data
    void receivedData(const QByteArray &data);

    // Signal for updating the guest count
    void guestCountUpdated(int guestCount);

private:
    QTcpServer *tcpServer;
    QList<QTcpSocket*> clients;
};

#endif // NETWORKMANAGER_H
