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
#include "dictionary_logic.h"
#include "dictionary.h"

namespace translate {
std::string translate(const std::string &text);
}

class ReadNow : public QMainWindow {
public:
    explicit ReadNow(QMainWindow *parent = nullptr);
    void printBook(const QString &book = nullptr, const QString &author = nullptr, const QString &title = nullptr);

    ~ReadNow() override = default;


    void buttonsInLibraryConnectWithReader(){
        this->removeToolBar(this->fileToolBar);
        this->fileToolBar->clear();
        this->createActions();
        this->createToolBars();
    }

    void buttonConnectWithDict(QMainWindow &parent, Dictionary &dictionary) {
        connect(button, &QPushButton::clicked, &dictionary, [&](){
            if (translatedText != nullptr){
                int wordId = dictionary.m_logic.add_word(selectedText.toStdString(), translText.toStdString());
                int setId = dictionary.m_logic.create_wordSet(authorName.toStdString() + " " + title.toStdString());
                //dictionary.add_group_to_menu(setId, authorName.toStdString() + title.toStdString());
                dictionary.m_logic.add_word_to_group(wordId, setId);
            }
        } );
    }

    QString getAuthorTitle(){
        return authorName + " " + title;
    }

private slots:
    void translateText();

private:
    void printWindowWithTranslate();
    void buttonAddPhraseToDict();
    void createActions();
    void createToolBars();

    int screen_width;
    int screen_height;
    QTextEdit *bookText{};    // виджет, отображающий текст
    QToolBar *fileToolBar{};  // панель управления

    QAction *translateSelectedText{};
    QString selectedText;
    QString translText;
    QPushButton *button;
    QString authorName = "";
    QString title = "";
public:
    QTextEdit *translatedText{}; // окошко с переводом
};

#endif  // READNOW_H