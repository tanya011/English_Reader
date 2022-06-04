#ifndef YAFR_SERVERPROBLEMSWINDOW_H
#define YAFR_SERVERPROBLEMSWINDOW_H

#include <QWidget>
#include <QMessageBox>
#include "include/serverProblemsException.h"


class ServerProblemsWindow : public QWidget {
private:
    QMessageBox messageBox_;
    int res;

public:
    ServerProblemsWindow() {
        messageBox_.setText("There was a problem connecting to the server!");
        messageBox_.setDetailedText("Из-за проблем с подключением к серверу невозможно войти в аккаунт."
                                    "Вы можете переподключиться или выйти из приложения. ");
        messageBox_.setWindowTitle("Failed");
        messageBox_.setFixedSize(1200,400);
        messageBox_.setIcon(QMessageBox::Warning);
        messageBox_.addButton(QMessageBox::Abort);
        messageBox_.addButton(tr("Reconnect"), QMessageBox::NoRole);
        res = messageBox_.exec();
        if (res == QMessageBox::Abort) {
            throw ServerProblemsExceptionAbort();
        }
        else {
            throw ServerProblemsExceptionReconnect();
        }
    }

    void show() {
        messageBox_.show();
    }
};

class ServerProblemsWindowUpdateLibrary : public QWidget {
private:
    QMessageBox messageBox_;
    int res;

public:
    ServerProblemsWindowUpdateLibrary();
    void show();
};

class ServerProblemsWindowAddInCollection : public QWidget {
private:
    QMessageBox messageBox_;
    int res;

public:
    ServerProblemsWindowAddInCollection();
    void show();
};

class ServerProblemsWindowDeleteInCollection : public QWidget {
private:
    QMessageBox messageBox_;
    int res;
public:
    ServerProblemsWindowDeleteInCollection();
    void show();
};

class ServerProblemsWindowSaveDict : public QWidget {
private:
    QMessageBox messageBox_;
    int res;
public:
    ServerProblemsWindowSaveDict();
    void show();
};

#endif //YAFR_SERVERPROBLEMSWINDOW_H
