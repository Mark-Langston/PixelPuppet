#include "PaintTogetherDialog.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>
#include <QRandomGenerator>

PaintTogetherDialog::PaintTogetherDialog(QWidget *parent, NetworkManager *networkManager)
    : QDialog(parent), networkManager(networkManager) {
    // Set up the dialog layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *infoLabel = new QLabel("Choose to Host or Join a session.", this);
    layout->addWidget(infoLabel);

    hostButton = new QPushButton("Host", this);
    joinButton = new QPushButton("Join", this);

    layout->addWidget(hostButton);
    layout->addWidget(joinButton);

    // Add a label for the session code
    QLabel *sessionCodeLabel = new QLabel("Session Code:", this);
    layout->addWidget(sessionCodeLabel);

    // Add the non-editable text field to display the session code
    sessionCodeField = new QLineEdit(this);
    sessionCodeField->setReadOnly(true);  // Make it non-editable
    layout->addWidget(sessionCodeField);

    connect(hostButton, &QPushButton::clicked, this, &PaintTogetherDialog::onHostClicked);
    connect(joinButton, &QPushButton::clicked, this, &PaintTogetherDialog::onJoinClicked);

    setLayout(layout);
    setWindowTitle("Paint Together");
}

PaintTogetherDialog::~PaintTogetherDialog() {}

void PaintTogetherDialog::setSessionCode(const QString &sessionCode) {
    // Set the session code in the text field
    sessionCodeField->setText(sessionCode);
}

void PaintTogetherDialog::onHostClicked() {
    // Here, we don't generate the session code in this dialog. The main window is responsible for that.
    // Emit the signal for session creation with the session code passed from the main window
    emit sessionCreated(sessionCodeField->text());  // Signal the main window with the session code
    accept();  // Close the dialog
}

void PaintTogetherDialog::onJoinClicked() {
    bool ok;
    QString sessionCode = QInputDialog::getText(this, tr("Join Session"),
                                                tr("Enter session code:"),
                                                QLineEdit::Normal, "", &ok);

    if (ok && !sessionCode.isEmpty()) {
        emit sessionJoined(sessionCode);  // Signal the main window with the session code
        accept();  // Close the dialog
    }
}
