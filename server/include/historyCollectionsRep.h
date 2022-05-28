#ifndef YAFR_HISTORYCOLLECTIONSREP_H
#define YAFR_HISTORYCOLLECTIONSREP_H

#include <mutex>
#include "dbManager.h"


struct HistoryCollectionsRep {
private:
    DBManager &manager_;
    std::string tableName_ = "historyCollections";
    std::mutex *mutex_;  // Need to be locked before every work with DB
public:

    HistoryCollectionsRep(DBManager &m, std::mutex *mutex)
            : manager_(m), mutex_(mutex) {
        std::unique_lock l(*mutex_);
        std::unique_ptr<sql::Statement> stmt(
                manager_.getConnection().createStatement());
        stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName_ +
                      "("
                      "userId INT,"
                      "actionNum INT,"
                      "actionType TINYTEXT,"
                      "bookId INT"
                      ")");
    }

};

#endif //YAFR_HISTORYCOLLECTIONSREP_H
