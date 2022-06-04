#ifndef READNOW_H
#define READNOW_H

#include <QToolBar>
#include "connectingWindow.h"
#include "dictionaryWindow.h"

// Функция перевода
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

    QString getAuthorTitle();

    bool emptyTranslatedTextDisplay();

    void cleanTranslatedTextDisplay();

    void updateToolBar();

private slots:
    void translateText();

private:
    ConnectingWindow* parent_;

    void createWindowWithTranslate(); // создаем окошко с переводом
    void createButtonAddPhraseToDict();  // создание кнопки "Добавить в словарь"
    QTextEdit *bookTextDisplay_{};  // виджет, отображающий текст
    QPushButton *button_; // Кнопка "Добавить в словарь"

    int screenWidth_;
    int screenHeight_;

    QAction *translateSelectedTextAction_;

    QString selectedText_;
    QString translatedText_;
    QString authorName_ = "";
    QString title_ = "";

    QTextEdit *translatedTextDisplay_{};  // окошко с переводом
    QToolBar *toolBar_{};
    void createActions();
    void createToolBars();

    void makeConnectWithDict();
};

#endif  // READNOW_H
