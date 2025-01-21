#ifndef SESSIONCREATEDDIALOG_H
#define SESSIONCREATEDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class SessionCreatedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SessionCreatedDialog(QWidget *parent = nullptr, const QString &sessionCode = QString());

private slots:
    void onCloseButtonClicked();

private:
    QLineEdit *sessionCodeBox;
    QPushButton *closeButton;
};

#endif // SESSIONCREATEDDIALOG_H
