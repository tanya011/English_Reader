#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include "connectingWindow.h"

class AuthorizationWindow : public QWidget {
private:
    ConnectingWindow *parent_;
    QLineEdit *nameField_= nullptr;
    QLineEdit *passwordField_ = nullptr;
    QWidget *box_;
    QVBoxLayout *layout_;
    QPushButton *authBtn_= nullptr;
    QPushButton* exitBtn_= nullptr;

public:
    explicit AuthorizationWindow(ConnectingWindow *parent = nullptr);
    void updateWindow();
};

#endif  // AUTHORIZATION_H
