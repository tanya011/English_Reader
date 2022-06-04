#include "include/authorizationWindow.h"
#include "include/serverProblemsException.h"

AuthorizationWindow::AuthorizationWindow(ConnectingWindow *parent)
    : QWidget(parent),
      parent_(parent),
      box_(new QWidget),
      layout_(new QVBoxLayout) {
    box_->setLayout(layout_);
    box_->setParent(this);
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
        layout_->insertWidget(0, nameField_);
        layout_->insertWidget(1, passwordField_);
        layout_->insertWidget(2, authBtn_);

        QObject::connect(authBtn_, &QPushButton::clicked, this, [=]() {
            auto username = nameField_->text().toStdString();
            auto password = passwordField_->text().toStdString();
            while (true) {
                try {
                    std::cout << "==========================HERE" << std::endl;
                    parent_->user->init(username, password);
                    std::cout << "===========================AND HERE" << std::endl;
                    break;
                } catch (ServerProblemsExceptionAbort &e) {
                    exit(0);
                } catch (ServerProblemsExceptionReconnect &){
                    continue;
                } catch (std::exception &e){
                    std::cout << e.what() << std::endl;
                    break;
                }
            }
            parent_->updateCollection();
            this->updateWindow();
        });
    } else {
        if (nameField_ != nullptr) {
            layout_->removeWidget(nameField_);
            layout_->removeWidget(passwordField_);
            layout_->removeWidget(authBtn_);
            delete authBtn_;
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
