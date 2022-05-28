#ifndef YAFR_USER_H
#define YAFR_USER_H

#include "../include/book.h"
#include "../include/httplib.h"
#include "../include/actCollectionsHistory.h"
#include "bookRep.h"
#include "userRepLocal.h"

struct action{
    int numberOfAction; //1 = add, 0 = delete;
    int userId;
    int bookId;
};

// Here http used, so we can't send password to server safely
struct User {
private:
    BookRep *bookRep_;
    UserRepLocal *userRepLocal_;
    httplib::Client client_{"localhost:8080"};
    bool isAuthorized_ = false;
    std::string token_;
    std::vector<action> actionsToDBCollections;

public:
    User(BookRep *bookRep, UserRepLocal *userRepLocal);
    void init(const std::string &username, const std::string &password);
    std::vector<Book> getLibraryBooks();
    int addBookToCollection(int bookId);
    std::vector<Book> updateLibrary();
    std::vector<Book> getCollectionBooks();
    void deleteCollectionBook(int bookId);

    bool isAuthorized();

    void exit();

    void newActionInCollection(std::string action, int bookId);

    std::vector<ActCollectionsHistory> getNewActions(int startAt);

    // TODO: sync_dict();
    // TODO: sync_collect();
    // TODO: sync_lib();
};

#endif  // YAFR_USER_H
