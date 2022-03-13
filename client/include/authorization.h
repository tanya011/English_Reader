#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

class Auth : public QWidget {
private:

public:
    explicit Auth(QWidget *parent = nullptr)
    : QWidget(parent) {
        auto box = new QWidget;
        auto layout = new QGridLayout;
        auto line1 = new QLineEdit;
        auto line2 = new QLineEdit;
        layout->addWidget(line1);
        layout->addWidget(line2);

        auto button_auth = new QPushButton;
        button_auth->setText("Войти");
        layout->addWidget(button_auth);

        box->setLayout(layout);
        box->setParent(this);
    }
};

#endif  // AUTHORIZATION_H

