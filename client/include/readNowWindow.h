#ifndef READNOW_H
#define READNOW_H

#include <QMainWindow>
#include <QToolBar>
#include "connectingWindow.h"
#include "dictionaryWindow.h"

namespace translate {
std::string translate(const std::string &text);
}

class ReadNowWindow : public QMainWindow {
public:
    explicit ReadNowWindow(ConnectingWindow *parent);
    void printBook(const QString &book = nullptr,
                   const QString &author = nullptr,
                   const QString &title = nullptr);
    ~ReadNowWindow() override = default;

    // TODO toolbar создается для всех книг один ... Что это вообще такое?
    // будет переделано
    void buttonsInLibraryConnectWithReader() {  // эта функция вызывается в
                                                // libraryWindow
        this->removeToolBar(this->toolBar_);
        this->toolBar_->clear();
        this->createActions();
        this->createToolBars();
    }

    void makeConnectWithDict();

    QString getAuthorTitle() {
        return authorName_ + " " + title_;
    }

    // TODO эта штука используется в library
    QTextEdit *translatedTextDisplay_{};  // окошко с переводом

    QToolBar *toolBar_{};

    void createActions();

    void createToolBars();

private slots:
    void translateText();

private:
    ConnectingWindow* parent;

    [[nodiscard]] QSize sizeHint() const override;

    void createWindowWithTranslate();

    void createButtonAddPhraseToDict();  // создание кнопки "Добавить в словарь"

    // панель управления

    int screenWidth_;
    int screenHeight_;

    QTextEdit *bookTextDisplay_{};  // виджет, отображающий текст

    // панель управления

    QAction *translateSelectedTextAction_;

    QString selectedText_;
    QString translatedText_;
    QString authorName_ = "";
    QString title_ = "";

    QPushButton *button_;

public:
};

#endif  // READNOW_H
