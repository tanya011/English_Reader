#include <include/collectionsHistoryRep.h>

CollectionsHistoryRep::CollectionsHistoryRep(DBManager &m, std::mutex *mutex)
: manager_(m), mutex_(mutex) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::Statement> stmt(
            manager_.getConnection().createStatement());
    stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName_ +
                  "("
                  "userId INT,"
                  "ActionNum INT,"
                  "actionType TINYTEXT,"
                  "bookId INT"
                  ")");
}

std::vector<ActCollectionsHistory> CollectionsHistoryRep::giveHistoryById(int userId, int startAt) {
    std::unique_lock l(*mutex_);

    std::vector<ActCollectionsHistory> actions;

    std::unique_ptr<sql::Statement> stmt(
            manager_.getConnection().createStatement());

    std::unique_ptr<sql::ResultSet> lastActionN = std::unique_ptr<sql::ResultSet>(
            stmt->executeQuery("SELECT * FROM " + tableName_ +
                               " WHERE userId="+std::to_string(userId) + " AND ActionNum>=" + std::to_string(startAt)));

    while (lastActionN->next()){
        actions.push_back({lastActionN->getString("actionType"), lastActionN->getInt("bookId")});
    }

    return actions;
}

int CollectionsHistoryRep::lastAction(int userId){
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::Statement> stmt(
            manager_.getConnection().createStatement());

    std::unique_ptr<sql::ResultSet> lastActionN = std::unique_ptr<sql::ResultSet>(
            stmt->executeQuery(R"(SELECT MAX(ActionNum) AS "ActionNum" FROM )" + tableName_ +
                               " WHERE userId="+std::to_string(userId)));

    int action = 0;
    while (lastActionN->next()){
        action = lastActionN->getInt("ActionNum");
    }
    return action;
}

void CollectionsHistoryRep::addInHistory(int userId, ActCollectionsHistory action) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::Statement> stmt(
            manager_.getConnection().createStatement());

    std::unique_ptr<sql::ResultSet> lastActionN = std::unique_ptr<sql::ResultSet>(
            stmt->executeQuery(R"(SELECT MAX(ActionNum) AS "ActionNum" FROM )" + tableName_ +
                               " WHERE userId="+std::to_string(userId)));

    int newAction = 1;
    while (lastActionN->next()){
        newAction = lastActionN->getInt("ActionNum") + 1;
    }

    std::unique_ptr<sql::PreparedStatement> prst(
            manager_.getConnection().prepareStatement(
                    "INSERT INTO " + tableName_ +
                    " (userId, ActionNum, actionType, bookId) VALUES "
                    "(?,?,?,?)"));

    prst->setInt(1, userId);
    prst->setInt(2, newAction);
    prst->setString(3, action.type);
    prst->setInt(4, action.bookId);
    prst->execute();
}