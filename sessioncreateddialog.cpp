#include "sessioncreateddialog.h"

SessionCreatedDialog::SessionCreatedDialog(QWidget *parent, const QString &sessionCode)
    : QDialog(parent)
{
    // Set up the dialog layout
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *infoLabel = new QLabel("Session Created! Copy the code below:", this);
    layout->addWidget(infoLabel);

    sessionCodeBox = new QLineEdit(sessionCode, this);
    sessionCodeBox->setReadOnly(true);  // Make it non-editable
    layout->addWidget(sessionCodeBox);

    closeButton = new QPushButton("Close", this);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &SessionCreatedDialog::onCloseButtonClicked);

    setLayout(layout);
    setWindowTitle("Session Created");
}

void SessionCreatedDialog::onCloseButtonClicked() {
    accept();  // Close the dialog
}
