#include "include/wordSetContentRep.h"

WordSetContentRep::WordSetContentRep(DBManager &m)
        : manager(m), stmt(manager.getConnection().createStatement()) {
    stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName +
                  "("
                  "wordSetId INT NOT NULL,"
                  "wordId INT NOT NULL"
                  ")");
}

void WordSetContentRep::addWordToSetTable(const int wordSetId,
                                          const int wordId) {
    std::unique_ptr<sql::PreparedStatement> prst(
            manager.getConnection().prepareStatement("INSERT INTO " + tableName +
                                                     " (wordSetId, wordId) VALUES "
                                                     "(?,?)"));
    prst->setInt(1, wordSetId);
    prst->setInt(2, wordId);
    prst->execute();
}

bool WordSetContentRep::deleteWordFromSet(
        int wordSetId,
        int wordId) {  // true if everything is ok
    try {
        stmt->execute("DELETE FROM " + tableName +
                      " WHERE wordSetId=" + std::to_string(wordSetId) +
                      " AND " + "wordId=" + std::to_string(wordId));
        historyChanges_.push_front({"wordDeleted", wordSetId, wordId});
        return true;
    } catch (sql::SQLException &e) {
        // std::cout << e.what();
        return false;
    }
}

std::vector<std::pair<int, int>>
WordSetContentRep::downloadWordSetsContentData() {
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
            "SELECT wordSetId, wordId FROM " + tableName + " ORDER BY wordSetId"));
    std::vector<std::pair<int, int>> data;
    while (reqRes->next()) {
        data.emplace_back(static_cast<int>(reqRes->getInt("wordSetId")),
                          static_cast<int>(reqRes->getInt("wordId")));
    }
    return data;
}

void WordSetContentRep::clear(){
    stmt->execute("TRUNCATE " + tableName);
}

std::vector<int> WordSetContentRep::getWordSetContent(int wordSetId) {
    std::unique_ptr<sql::ResultSet> reqRes(
            stmt->executeQuery("SELECT wordId FROM " +
                               tableName + " WHERE wordSetId=" + std::to_string(wordSetId)));
    std::vector<int> words;
    while (reqRes->next()) {
        words.push_back(
                static_cast<int>(reqRes->getInt("wordId")));
    }
    return words;
}

int WordSetContentRep::getWordSetSize(int wordSetId) {
    std::unique_ptr<sql::ResultSet> reqRes(
            stmt->executeQuery("SELECT COUNT(*) FROM " +
                               tableName + " WHERE wordSetId=" + std::to_string(wordSetId)));
    if (reqRes->next()) {
        int size = reqRes->getInt("COUNT(*)");
        return size;
    }
    else{
        throw WordSetContentRepException("count * bug");
    }
}

void WordSetContentRep::deleteWordFromAllSets(int wordId) {
    stmt->execute("DELETE FROM " + tableName +
                  " WHERE wordId=" + std::to_string(wordId));
    historyChanges_.push_front({"wordDeletedFromDictionary", -1, wordId});
}

void WordSetContentRep::saveHistoryAddWordToSet(int wordSetId, int wordId){
    historyChanges_.push_front({"wordAdded", wordSetId, wordId});
}

std::deque<HistoryChangeWordSetContentRep> WordSetContentRep::getHistoryChanges() {
    return historyChanges_;
}

void WordSetContentRep::clearHistory(){
    historyChanges_.clear();
}

WordSetContentRepException::WordSetContentRepException(const std::string &message)
        : std::runtime_error(message) {
}