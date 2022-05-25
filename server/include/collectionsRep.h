#ifndef YAFR_COLLECTIONSREP_H
#define YAFR_COLLECTIONSREP_H

#include <mutex>
#include "dbManager.h"


struct CollectionsRep {
private:
    DBManager &manager_;
    std::string tableName_ = "collections";
    std::mutex *mutex_;  // Need to be locked before every work with DB
public:
    CollectionsRep(DBManager &m, std::mutex *mutex);

    std::vector<int> getUserBookId(
        int userId) ;  // TODO May be we should replace id with token

    void addBookToUser(int userId, int bookId);

    void deleteBookFromUser(int userId, int bookId);

};

#endif  // YAFR_COLLECTIONSREP_H
