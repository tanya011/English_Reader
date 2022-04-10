#ifndef READNOW_H
#define READNOW_H

#include <QToolBar>
#include <QMainWindow>
#include "dictionary.h"
#include "dictionaryWindow.h"

namespace translate {
    std::string translate(const std::string &text);
}

class ReadNowWindow : public QMainWindow {
public:
    explicit ReadNowWindow(QMainWindow *parent = nullptr);
    void printBook(const QString &book = nullptr, const QString &author = nullptr, const QString &title = nullptr);
    ~ReadNowWindow() override = default;


    // TODO toolbar создается для всех книг один ...
    // будет переделано
    void buttonsInLibraryConnectWithReader() { // эта функция вызывается в libraryWindow
        this->removeToolBar(this->toolBar_);
        this->toolBar_->clear();
        this->createActions();
        this->createToolBars();
    }

    void makeConnectWithDict(QMainWindow &parent,
                             DictionaryWindow &dictionary) {
        connect(button_, &QPushButton::clicked, &dictionary, [&]() {
            if (translatedTextDisplay_ != nullptr) {
                int wordId = dictionary.mLogic_.addWord(
                    selectedText_.toStdString(), translatedText_.toStdString());
                int setId = dictionary.mLogic_.createWordSet(
                    authorName_.toStdString() + " " + title_.toStdString());
                //dictionary.add_group_to_menu(setId, authorName.toStdString() + title.toStdString());
                dictionary.mLogic_.addWordToWordSet(wordId, setId);
            }
        });
    }

    QString getAuthorTitle() {
        return authorName_ + " " + title_;
    }

// TODO эта штука используется в library
    QTextEdit *translatedTextDisplay_{}; // окошко с переводом

private slots:
    void translateText();

private:
    [[nodiscard]] QSize sizeHint() const override;

    void createWindowWithTranslate();

    void createButtonAddPhraseToDict(); // создание кнопки "Добавить в словарь"
    void createActions();

    void createToolBars(); // панель управления

    int screenWidth_;
    int screenHeight_;

    QTextEdit *bookTextDisplay_{};    // виджет, отображающий текст

    QToolBar *toolBar_{};  // панель управления

    QAction *translateSelectedTextAction_;

    QString selectedText_;
    QString translatedText_;
    QString authorName_ = "";
    QString title_ = "";

    QPushButton *button_; // в будущем уберется

public:

};

#endif  // READNOW_H
