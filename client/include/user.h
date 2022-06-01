#ifndef YAFR_USER_H
#define YAFR_USER_H

#include <queue>
#include "../include/book.h"
#include "../include/httplib.h"
#include "../include/actCollectionsHistory.h"
#include "bookRep.h"

namespace userRepLocal{
    int getValue();
    void newValue(int newValue);
}

struct action {
    int numberOfAction; //1 = add, 0 = delete;
    int userId;
    int bookId;
};

// Here http used, so we can't send password to server safely
struct User {
private:
    BookRep *bookRep_;
    httplib::Client client_{"localhost:8080"};
    bool isAuthorized_ = false;
    std::string token_;
    std::vector<action> actionsToDBCollections;


public:
    std::queue<ActCollectionsHistory> Queue;

    User(BookRep *bookRep);

    void init(const std::string &username, const std::string &password);

    std::vector<Book> getCollectionBooks();

    std::vector<Book> getLibraryBooks();

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
