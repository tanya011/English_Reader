#include "include/wordSetRep.h"

WordSetRepServ::WordSetRepServ(DBManager &m, std::mutex *mutex)
        : manager(m), mutex_(mutex), stmt(manager.getConnection().createStatement()) {
    std::unique_lock l(*mutex_);
    stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName +
                  "("
                  "userid INT,"
                  "id INT NOT NULL,"
                  "name TINYTEXT"
                  ")");
}

void WordSetRepServ::addWordSet(int userId, int id, const std::string &wordSetName) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::ResultSet> reqRes(
            stmt->executeQuery("SELECT * FROM " + tableName + " WHERE userid=" + std::to_string(userId) + " AND id=" + std::to_string(id) +  " AND name='" +
                               wordSetName + "'"));
    if (reqRes->next()) {
        return;
    }
    std::unique_ptr<sql::PreparedStatement> prst(
            manager.getConnection().prepareStatement("INSERT INTO " +
                                                     tableName +
                                                     " (userid, id, name) VALUES "
                                                     "(?, ?,?)"));
    prst->setInt(1, userId);
    prst->setInt(2, id);
    prst->setString(3, wordSetName);

    prst->execute();
}

void WordSetRepServ::deleteWordSetById(int userId, int id) {
    std::unique_lock l(*mutex_);
    stmt->execute("DELETE FROM " + tableName +
                  " WHERE userid=" + std::to_string(userId) + " AND id=" + std::to_string(id));
}

std::vector<WordSet> WordSetRepServ::getUserWordSets(int userId) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
            "SELECT * FROM " + tableName + " WHERE userId=" + std::to_string(userId)));
    std::vector<WordSet> wordSets;
    while (reqRes->next()){
        wordSets.emplace_back(
                static_cast<int>(reqRes->getInt("id")),
                static_cast<std::string>(reqRes->getString("name")));
    }
    return wordSets;
}