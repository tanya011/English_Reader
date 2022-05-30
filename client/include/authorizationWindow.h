#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H


#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include "connectingWindow.h"

class AuthorizationWindow : public QWidget {
private:
    ConnectingWindow* parent_;
    QWidget* box;
    QVBoxLayout* layout = new QVBoxLayout();
public:
    explicit AuthorizationWindow(ConnectingWindow *parent = nullptr);
    void updateWindow();

};

#endif  // AUTHORIZATION_H
