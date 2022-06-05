#include "include/wordSetRep.h"

WordSetRepServ::WordSetRepServ(DBManager &m, std::mutex *mutex)
    : manager_(m),
      mutex_(mutex),
      stmt_(manager_.getConnection().createStatement()) {
    std::unique_lock l(*mutex_);
    stmt_->execute("CREATE TABLE IF NOT EXISTS " + tableName_ +
                   "("
                   "userid INT,"
                   "id INT NOT NULL,"
                   "name TINYTEXT"
                   ")");
}

void WordSetRepServ::addWordSet(int userId,
                                int id,
                                const std::string &wordSetName) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::PreparedStatement> prst1(
        manager_.getConnection().prepareStatement(
            "SELECT * FROM " + tableName_ +
            " WHERE userid=? AND id=? AND name=?"));
    prst1->setInt(1, userId);
    prst1->setInt(2, id);
    prst1->setString(3, wordSetName);
    std::unique_ptr<sql::ResultSet> reqRes(prst1->executeQuery());
    if (reqRes->next()) {
        return;
    }
    std::unique_ptr<sql::PreparedStatement> prst2(
        manager_.getConnection().prepareStatement(
            "INSERT INTO " + tableName_ +
            " (userid, id, name) VALUES (?,?,?)"));
    prst2->setInt(1, userId);
    prst2->setInt(2, id);
    prst2->setString(3, wordSetName);

    prst2->execute();
}

std::vector<WordSet> WordSetRepServ::getUserWordSets(int userId) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::ResultSet> reqRes(
        stmt_->executeQuery("SELECT * FROM " + tableName_ +
                            " WHERE userId=" + std::to_string(userId)));
    std::vector<WordSet> wordSets;
    while (reqRes->next()) {
        wordSets.emplace_back(
            static_cast<int>(reqRes->getInt("id")),
            static_cast<std::string>(reqRes->getString("name")));
    }
    return wordSets;
}