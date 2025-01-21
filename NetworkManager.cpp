#include "NetworkManager.h"
#include <QHostAddress>
#include <QNetworkInterface>
#include <QByteArray>
#include <QDataStream>
#include <QDebug>

NetworkManager::NetworkManager(QObject *parent)
    : QObject(parent), tcpServer(nullptr) {}

NetworkManager::~NetworkManager() {
    delete tcpServer;
}

void NetworkManager::startServer() {
    tcpServer = new QTcpServer(this);
    if (tcpServer->listen(QHostAddress::AnyIPv6, 8080)) {
        qDebug() << "Server started!";
        connect(tcpServer, &QTcpServer::newConnection, this, [this]() {
            QTcpSocket *clientSocket = tcpServer->nextPendingConnection();
            clients.append(clientSocket);
            emit guestCountUpdated(clients.size());

            connect(clientSocket, &QTcpSocket::readyRead, this, [this, clientSocket]() {
                QByteArray data = clientSocket->readAll();
                emit receivedData(data);
            });

            connect(clientSocket, &QTcpSocket::disconnected, this, [this, clientSocket]() {
                clients.removeAll(clientSocket);
                clientSocket->deleteLater();
                emit guestCountUpdated(clients.size());
            });
        });
    } else {
        qDebug() << "Server failed to start!";
    }
}

void NetworkManager::connectToServer(const QString &ipv6Address, int port) {
    QTcpSocket *tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(QHostAddress(ipv6Address), port);
    connect(tcpSocket, &QTcpSocket::readyRead, this, [this, tcpSocket]() {
        QByteArray data = tcpSocket->readAll();
        emit receivedData(data);
    });
}

QString NetworkManager::encodeSessionCode(const QString &ipv6Address, int port) {
    QString addressPort = ipv6Address + ":" + QString::number(port);
    QByteArray encoded = addressPort.toUtf8().toBase64();  // Base64 encoding
    return QString(encoded);
}

bool NetworkManager::decodeSessionCode(const QString &sessionCode, QString &ipv6Address, int &port) {
    QByteArray decoded = QByteArray::fromBase64(sessionCode.toUtf8());
    QString decodedString(decoded);
    QStringList parts = decodedString.split(":");

    qDebug() << "Decoded session code:" << decodedString; // Add this line for debugging

    if (parts.size() == 2) {
        ipv6Address = parts[0];
        port = parts[1].toInt();

        qDebug() << "Extracted ipv6Address:" << ipv6Address << "Port:" << port; // Add this line for debugging

        return true;
    }

    return false;
}

QString NetworkManager::getLocalIPv6Address() {
    // Try fetching the first non-loopback IPv6 address on the system
    foreach (const QNetworkInterface &networkInterface, QNetworkInterface::allInterfaces()) {
        if (networkInterface.flags().testFlag(QNetworkInterface::IsUp)) {
            foreach (const QNetworkAddressEntry &entry, networkInterface.addressEntries()) {
                if (entry.ip().protocol() == QAbstractSocket::IPv6Protocol) {
                    return entry.ip().toString();
                }
            }
        }
    }
    return QString();  // Return an empty string if no IPv6 address is found
}
