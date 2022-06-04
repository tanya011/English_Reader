#include "include/serverProblemsWindow.h"
#include "../include/serverProblemsException.h"
#include <QMessageBox>

ServerProblemsWindow::ServerProblemsWindow() {
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

void ServerProblemsWindow::show() {
    messageBox_.show();
}

ServerProblemsWindowUpdateLibrary::ServerProblemsWindowUpdateLibrary() {
    messageBox_.setText("There was a problem connecting to the server!");
    messageBox_.setDetailedText("Из-за проблем с подключением к серверу невозможно подгрузить книги из библиотеки."
                                "Вы можете переподключиться или не подгружать книги. ");
    messageBox_.setWindowTitle("Failed");
    messageBox_.setFixedSize(1200, 400);
    messageBox_.setIcon(QMessageBox::Warning);
    //messageBox_.addButton(tr("Show details"), QMessageBox::detailedText);
    messageBox_.addButton(QMessageBox::Abort);
    messageBox_.addButton(tr("Reconnect"), QMessageBox::NoRole);
    res = messageBox_.exec();
    if (res == QMessageBox::Abort) {
        throw ServerProblemsExceptionNotUploadLibrary();
    } else {
        throw ServerProblemsExceptionReconnect();
    }
}

void ServerProblemsWindowUpdateLibrary::show() {
    messageBox_.show();
}

ServerProblemsWindowAddInCollection::ServerProblemsWindowAddInCollection() {
    messageBox_.setText("There was a problem connecting to the server!");
    messageBox_.setDetailedText("Из-за проблем с подключением к серверу невозможно добавить книгу в коллекцию."
                                "Вы можете переподключиться или не добавлять книгу. ");
    messageBox_.setWindowTitle("Failed");
    messageBox_.setFixedSize(1200,400);
    messageBox_.setIcon(QMessageBox::Warning);
    messageBox_.addButton(QMessageBox::Abort);
    messageBox_.addButton(tr("Reconnect"), QMessageBox::NoRole);
    res = messageBox_.exec();
    if (res == QMessageBox::Abort) {
        throw ServerProblemsExceptionNotAddInCollection();
    }
    else {
        throw ServerProblemsExceptionReconnect();
    }
}

void ServerProblemsWindowAddInCollection::show() {
    messageBox_.show();
}

ServerProblemsWindowDeleteInCollection::ServerProblemsWindowDeleteInCollection() {
    messageBox_.setText("There was a problem connecting to the server!");
    messageBox_.setDetailedText("Из-за проблем с подключением к серверу невозможно удалить книгу в коллекции."
                                "Вы можете переподключиться или не удалять книгу. ");
    messageBox_.setWindowTitle("Failed");
    messageBox_.setFixedSize(1200,400);
    messageBox_.setIcon(QMessageBox::Warning);
    messageBox_.addButton(QMessageBox::Abort);
    messageBox_.addButton(tr("Reconnect"), QMessageBox::NoRole);
    res = messageBox_.exec();
    if (res == QMessageBox::Abort) {
        throw ServerProblemsExceptionNotDeleteInCollection();
    }
    else {
        throw ServerProblemsExceptionReconnect();
    }
}

void ServerProblemsWindowDeleteInCollection::show() {
    messageBox_.show();
}

ServerProblemsWindowSaveDict::ServerProblemsWindowSaveDict() {
    messageBox_.setText("There was a problem connecting to the server!");
    messageBox_.setDetailedText("Из-за проблем с подключением к серверу невозможно сохранить слова на сервер."
                                "Вы можете переподключиться или сделать это позже. ");
    messageBox_.setWindowTitle("Failed");
    messageBox_.setFixedSize(1200,400);
    messageBox_.setIcon(QMessageBox::Warning);
    messageBox_.addButton(QMessageBox::Abort);
    messageBox_.addButton(tr("Reconnect"), QMessageBox::NoRole);
    res = messageBox_.exec();
    if (res == QMessageBox::Abort) {
        throw ServerProblemsExceptionNotSaveDict();
    }
    else {
        throw ServerProblemsExceptionReconnect();
    }
}

void ServerProblemsWindowSaveDict::show() {
    messageBox_.show();
}