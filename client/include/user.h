#ifndef YAFR_USER_H
#define YAFR_USER_H

#include <queue>
#include "../include/actCollectionsHistory.h"
#include "../include/book.h"
#include "../include/httplib.h"
#include "../include/word.h"
#include "../include/wordset.h"
#include "bookRep.h"
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

// Here http used, so we can't send password to server safely
struct User {
private:
    BookRep *bookRep_;
    WordRep *wordRep_;
    WordSetRep *wordSetRep_;
    WordSetContentRep *wordSetContentRep_;

    httplib::Client client_{"localhost:8080"};
    bool isAuthorized_ = false;
    std::string token_;
    std::vector<action> actionsToDBCollections;

public:
    std::queue<ActCollectionsHistory> Queue;

    User(WordRep *wordRep,
         WordSetRep *wordSetRep,
         WordSetContentRep *wordSetContentRep,
         BookRep *bookRep);

    void init(const std::string &username, const std::string &password);

    std::vector<Book> getCollectionBooks();

    std::vector<Book> getLibraryBooks();

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

    int addBookToLocalCollection(int bookId);

    int getLastCollectionAction();

    int addBookToCollection(int bookId);

    std::vector<Book> updateLibrary();

    void deleteCollectionBook(int bookId);

    bool isAuthorized() const;

    void exit();

    void newActionInCollection(std::string action, int bookId);

    std::vector<ActCollectionsHistory> getNewActions(int startAt);

    // TODO: sync_dict();

    void syncCollection();

    // TODO: sync_lib();
};

#endif  // YAFR_USER_H
