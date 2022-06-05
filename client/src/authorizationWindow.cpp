#include "include/authorizationWindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QLabel>
#include "include/serverProblemsException.h"

AuthorizationWindow::AuthorizationWindow(ConnectingWindow *parent)
    : QWidget(parent),
      parent_(parent),
      box_(new QWidget),
      layout_(new QVBoxLayout) {
    box_->setLayout(layout_);
    box_->setParent(this);
    box_->setGeometry(50, 20, 500, 300);
    updateWindow();
}
void AuthorizationWindow::updateWindow() {
    if (!parent_->user->isAuthorized()) {
        if (exitBtn_ != nullptr) {
            layout_->removeWidget(exitBtn_);
            delete exitBtn_;
        }
        authBtn_ = new QPushButton(tr("Вход"));
        nameField_ = new QLineEdit;
        passwordField_ = new QLineEdit;
        login = new QLabel;
        password = new QLabel;
        login->setText("Login:");
        password->setText("Password:");
        layout_->insertWidget(0, login);
        layout_->insertWidget(1, nameField_);
        layout_->insertWidget(2, password);
        layout_->insertWidget(3, passwordField_);
        layout_->insertWidget(4, authBtn_);

        QObject::connect(authBtn_, &QPushButton::clicked, this, [=]() {
            auto username = nameField_->text().toStdString();
            auto password = passwordField_->text().toStdString();
            while (true) {
                try {
                    parent_->user->init(username, password);
                    break;
                } catch (ServerProblemsExceptionAbort &e) {
                    exit(0);
                } catch (ServerProblemsExceptionReconnect &) {
                    continue;
                } catch (std::exception &e) {
                    std::cout << e.what() << std::endl;
                    break;
                }
            }
            parent_->updateCollection();
            this->updateWindow();
        });
    } else {
        if (nameField_ != nullptr) {
            layout_->removeWidget(login);
            layout_->removeWidget(nameField_);
            layout_->removeWidget(password);
            layout_->removeWidget(passwordField_);
            layout_->removeWidget(authBtn_);
            delete login;
            delete authBtn_;
            delete password;
            delete nameField_;
            delete passwordField_;
        }
        exitBtn_ = new QPushButton(tr("Выход"));
        layout_->addWidget(exitBtn_);
        QObject::connect(exitBtn_, &QPushButton::clicked, this, [=]() {
            parent_->user->exit();
            this->updateWindow();
            parent_->updateReadNow("", "", "Select text");
        });
    }
}
