#include "include/authorizationWindow.h"

AuthorizationWindow::AuthorizationWindow(ConnectingWindow *parent)
    : QWidget(parent), parent_(parent) {
    updateWindow();
}
void AuthorizationWindow::updateWindow() {
    qDeleteAll(this->findChildren<QWidget *>("", Qt::FindDirectChildrenOnly));
    auto box = new QWidget;
    auto layout = new QVBoxLayout;
    box->setLayout(layout);
    box->setParent(this);
    if (!parent_->user->isAuthorized()) {
        auto nameField = new QLineEdit;
        auto passwordField = new QLineEdit;
        layout->addWidget(nameField);
        layout->addWidget(passwordField);

        auto authBtn = new QPushButton(tr("Войти"));
        layout->addWidget(authBtn);

        QObject::connect(authBtn, &QPushButton::clicked, this, [=]() {
            auto username = nameField->text().toStdString();
            auto password = passwordField->text().toStdString();
            parent_->user->init(username, password);
            this->updateWindow();
        });
    } else {
        auto exitBtn = new QPushButton(tr("Выход"));
        layout->addWidget(exitBtn);
        QObject::connect(exitBtn, &QPushButton::clicked, this, [=]() {
            parent_->user->exit();
            this->updateWindow();
        });
    }
}
