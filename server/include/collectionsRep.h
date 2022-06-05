#ifndef YAFR_COLLECTIONSREP_H
#define YAFR_COLLECTIONSREP_H

#include <mutex>
#include "dbManager.h"

struct CollectionsRep {
private:
    DBManager &manager_;
    std::string tableName_ = "collections";
    std::mutex *mutex_;

public:
    CollectionsRep(DBManager &m, std::mutex *mutex);

    std::vector<int> getUserBookId(int userId);

    void addBookToUser(int userId, int bookId);

    void deleteBookFromUser(int userId, int bookId);
};

#endif  // YAFR_COLLECTIONSREP_H
