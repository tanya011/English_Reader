#ifndef YAFR_SERVERPROBLEMSWINDOW_H
#define YAFR_SERVERPROBLEMSWINDOW_H

#include <QWidget>
#include <QMessageBox>

class ServerProblemsWindow : public QWidget {
private:
    QMessageBox messageBox_;
    int res;

public:
    ServerProblemsWindow() ;
    void show();
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
