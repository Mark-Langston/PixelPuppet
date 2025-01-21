#ifndef PAINTTOGETHERDIALOG_H
#define PAINTTOGETHERDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include "NetworkManager.h"

class PaintTogetherDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PaintTogetherDialog(QWidget *parent = nullptr, NetworkManager *networkManager = nullptr);
    ~PaintTogetherDialog();

    // Method to set the session code
    void setSessionCode(const QString &sessionCode);

private:
    QLineEdit *sessionCodeField;  // Non-editable text field for session code
    QPushButton *hostButton;
    QPushButton *joinButton;
    NetworkManager *networkManager;

private slots:
    void onHostClicked();
    void onJoinClicked();

signals:
    // Signals to notify the main window of session creation or joining
    void sessionCreated(const QString &sessionCode);
    void sessionJoined(const QString &sessionCode);
};

#endif // PAINTTOGETHERDIALOG_H
