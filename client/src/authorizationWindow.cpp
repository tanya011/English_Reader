#include "include/authorizationWindow.h"

AuthorizationWindow::AuthorizationWindow(ConnectingWindow *parent)
    : QWidget(parent), parent_(parent) {
    updateWindow();
}
void AuthorizationWindow::updateWindow() {
    auto box = new QWidget;
    auto layout = new QGridLayout;
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
            qDeleteAll(
                box->findChildren<QWidget *>("", Qt::FindDirectChildrenOnly));
            updateWindow();
        });
    } else {
        auto exitBtn = new QPushButton(tr("Выход"));
        layout->addWidget(exitBtn);  // TODO: Nothing happends
        QObject::connect(exitBtn, &QPushButton::clicked, this, [=]() {
            parent_->user->exit();
            qDeleteAll(
                box->findChildren<QWidget *>("", Qt::FindDirectChildrenOnly));
            updateWindow();
        });
    }
    box->setLayout(layout);
    box->setParent(this);
}
