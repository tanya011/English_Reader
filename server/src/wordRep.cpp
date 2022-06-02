#include "include/wordRep.h"

WordRepServ::WordRepServ(DBManager &m, std::mutex *mutex)
        : manager_(m), mutex_(mutex), stmt(manager_.getConnection().createStatement()) {
    std::unique_lock l(*mutex_);
    stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName +
                  "("
                  "userid INT,"
                  "id INT NOT NULL,"
                  "original TINYTEXT,"
                  "translation TINYTEXT,"
                  "context TINYTEXT"
                  ")");
}

void WordRepServ::addWord(int userId,
                          int id,
                          const std::string &original,
                          const std::string &translation,
                          const std::string &context) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
            "SELECT * FROM " + tableName + " WHERE userId=" + std::to_string(userId) + " AND id=" + std::to_string(id) + " AND original='" + original +
            "' AND translation='" + translation + "'"));
    if (reqRes->next()) {
        return;
    }
    std::unique_ptr<sql::PreparedStatement> prst(
            manager_.getConnection().prepareStatement(
                    "INSERT INTO " + tableName +
                    " (userid, id, original, translation, context) VALUES "
                    "(?,?,?,?,?)"));
    prst->setInt(1, userId);
    prst->setInt(2, id);
    prst->setString(3, original);
    prst->setString(4, translation);
    prst->setString(5, context);

    prst->execute();
}

void WordRepServ::deleteWordById(int userId, int id) {
    std::unique_lock l(*mutex_);
    stmt->execute("DELETE FROM " + tableName +
                  " WHERE userid=" + std::to_string(userId) + " AND id=" + std::to_string(id));
}

std::vector<Word> WordRepServ::getUserWords(int userId) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
            "SELECT * FROM " + tableName + " WHERE userId=" + std::to_string(userId)));
    std::vector<Word> words;
    while (reqRes->next()){
        words.emplace_back(
                static_cast<int>(reqRes->getInt("id")),
                static_cast<std::string>(reqRes->getString("original")),
                static_cast<std::string>(reqRes->getString("translation")),
                static_cast<std::string>(reqRes->getString("context")));
    }
    return words;
}