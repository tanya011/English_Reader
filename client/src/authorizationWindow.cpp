#include "include/authorizationWindow.h"

AuthorizationWindow::AuthorizationWindow(ConnectingWindow *parent)
    : QWidget(parent) {
    auto box = new QWidget;
    auto layout = new QGridLayout;
    if (!parent->user->isAuthorized()) {
        auto nameField = new QLineEdit;
        auto passwordField = new QLineEdit;
        layout->addWidget(nameField);
        layout->addWidget(passwordField);

        auto authBtn = new QPushButton;
        authBtn->setText("Войти");
        layout->addWidget(authBtn);

        QObject::connect(authBtn, &QPushButton::clicked, this, [=]() {
            auto username = nameField->text().toStdString();
            auto password = passwordField->text().toStdString();
            parent->user->init(username, password);
        });
    } else {
        auto exitBtn = new QPushButton(tr("Выход"));
        layout->addWidget(exitBtn);  // TODO: Nothing happends
    }
    box->setLayout(layout);
    box->setParent(this);
}
