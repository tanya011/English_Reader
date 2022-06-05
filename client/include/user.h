#ifndef YAFR_USER_H
#define YAFR_USER_H

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include <queue>
#include "../include/actCollectionsHistory.h"
#include "../include/book.h"
#include "../include/httplib.h"
#include "../include/word.h"
#include "../include/wordset.h"
#include "bookRep.h"
#include "include/periodic_function.h"
#include "wordRep.h"
#include "wordSetContentRep.h"
#include "wordSetRep.h"

namespace userRepLocal {
int getValue();
void newValue(int newValue);
}  // namespace userRepLocal

struct action {
    int numberOfAction;  // 1 = add, 0 = delete;
    int userId;
    int bookId;
};

struct User {
private:
    BookRep *bookRep_;
    WordRep *wordRep_;
    WordSetRep *wordSetRep_;
    WordSetContentRep *wordSetContentRep_;

    httplib::SSLClient client_;
    bool isAuthorized_ = false;
    std::string token_;

    std::vector<action> actionsToDBCollections;

    CallBackTimer timer_;

public:
    std::queue<ActCollectionsHistory> historyQueue_;

    User(WordRep *wordRep,
         WordSetRep *wordSetRep,
         WordSetContentRep *wordSetContentRep,
         BookRep *bookRep);

    ~User();

    void init(const std::string &username, const std::string &password);

    void startRequestThread();

    bool isAuthorized() const;

    void exit();

    std::vector<Book> getCollectionBooks();

    std::vector<Book> getLibraryBooks();

    int getLastCollectionAction();

    void addBookToCollection(int bookId);

    void deleteCollectionBook(int bookId);

    void newActionInCollection(const std::string &action, int bookId);

    std::vector<ActCollectionsHistory> getNewActions(int startAt);

    void syncCollection();

    std::vector<Word> getWords();

    std::vector<WordSet> getWordSets();

    std::vector<std::pair<int, int>> getSetContents();

    void downloadDictDataFromServer();

    void sendWordRepHistoryChange(HistoryChangeWordRep change);

    void sendWordSetRepHistoryChange(HistoryChangeWordSetRep change);

    void sendWordSetContentRepHistoryChange(
        HistoryChangeWordSetContentRep change);

    void updateDictionaryChanges();

    void clearTablesDict();
};

#endif  // YAFR_USER_H
