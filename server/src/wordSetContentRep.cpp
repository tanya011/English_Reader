#include "include/wordSetContentRep.h"

WordSetContentRepServ::WordSetContentRepServ(DBManager &m, std::mutex *mutex)
        : manager(m), mutex_(mutex), stmt(manager.getConnection().createStatement()) {
    std::unique_lock l(*mutex_);
    stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName +
                  "("
                  "userid INT,"
                  "wordSetId INT NOT NULL,"
                  "wordId INT NOT NULL"
                  ")");
}

void WordSetContentRepServ::addWordToSetTable(int userId, int wordSetId,
                                              int wordId) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::PreparedStatement> prst(
            manager.getConnection().prepareStatement("INSERT INTO " + tableName +
                                                     " (userid, wordSetId, wordId) VALUES "
                                                     "(?,?,?)"));
    prst->setInt(1, userId);
    prst->setInt(2, wordSetId);
    prst->setInt(3, wordId);
    prst->execute();
}

void WordSetContentRepServ::deleteWordFromSetTable(
        int userId,
        int WordSetId,
        int WordId) {
    std::unique_lock l(*mutex_);
    stmt->execute("DELETE FROM " + tableName +
                  " WHERE userid=" + std::to_string(userId) + " AND wordSetId=" + std::to_string(WordSetId) +
                  " AND " + "wordId=" + std::to_string(WordId));
}

std::vector<std::pair<int,int>> WordSetContentRepServ::getUserWordSetsContents(int userId) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
            "SELECT * FROM " + tableName + " WHERE userId=" + std::to_string(userId) + " AND NOT wordSetId=1"));
    std::vector<std::pair<int,int>> wordset_word;
    while (reqRes->next()){
        wordset_word.emplace_back(
                static_cast<int>(reqRes->getInt("wordSetId")),
                static_cast<int>(reqRes->getInt("wordId")));
    }
    std::cout << wordset_word.size();
    return wordset_word;
}

void WordSetContentRepServ::deleteWordFromAllSets(int userId, int wordId) {
    std::unique_lock l(*mutex_);
    stmt->execute("DELETE FROM " + tableName +
                  " WHERE userId=" + std::to_string(userId) + " AND wordId=" + std::to_string(wordId));
}