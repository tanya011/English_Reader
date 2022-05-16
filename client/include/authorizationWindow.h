#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H


#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include "connectingWindow.h"

class AuthorizationWindow : public QWidget {
private:
public:
    explicit AuthorizationWindow(ConnectingWindow *parent = nullptr);
};

#endif  // AUTHORIZATION_H
