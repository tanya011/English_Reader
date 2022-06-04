#ifndef YAFR_COLLECTIONSHISTORYREP_H
#define YAFR_COLLECTIONSHISTORYREP_H

#include "dbManager.h"
#include "actCollectionsHistory.h"
#include <mutex>

struct CollectionsHistoryRep {
private:
    DBManager &manager_;
    std::string tableName_ = "collectionsHistory";
    std::mutex *mutex_;  // Need to be locked before every work with DB
public:
    CollectionsHistoryRep(DBManager &m, std::mutex *mutex);
    std::vector<ActCollectionsHistory> giveHistoryById(int userId, int startAt);
    int lastAction(int userId);
    void addInHistory(int userId, ActCollectionsHistory action);
};

#endif //YAFR_COLLECTIONSHISTORYREP_H