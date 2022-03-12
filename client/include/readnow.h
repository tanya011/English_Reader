#ifndef READNOW_H
#define READNOW_H

#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDesktopWidget>
#include <QFile>
#include <QGraphicsScene>
#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QRect>
#include <QString>
#include <QTextEdit>
#include <QTextStream>
#include <QToolBar>
#include <QWidget>
#include <iostream>
#include <QMainWindow>

namespace translate {
std::string translate(const std::string &text);
}

class ReadNow : public QMainWindow {
public:
    explicit ReadNow(QMainWindow *parent = nullptr);
    void printBook(const QString &book = nullptr);

    ~ReadNow() override = default;


    void buttonsInLibraryConnectWithReader(){
        this->removeToolBar(this->fileToolBar);
        this->fileToolBar->clear();
        this->createActions();
        this->createToolBars();
    }

private slots:
    void translateText();

private:
    void printWindowWithTranslate();
    void buttonPhraseToDict();
    void createActions();
    void createToolBars();

    int screen_width;
    int screen_height;
    QTextEdit *textEdit{};    // виджет, отображающий текст
    QToolBar *fileToolBar{};  // панель управления
    QTextEdit *text{}; // окошко с переводом
    QAction *translateSelectedText{};
};

#endif  // READNOW_H