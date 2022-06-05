#include "include/wordSetContentRep.h"

WordSetContentRep::WordSetContentRep(DBManager &m)
        : manager_(m), stmt_(manager_.getConnection().createStatement()) {
    stmt_->execute("CREATE TABLE IF NOT EXISTS " + tableName_ +
                  "("
                  "wordSetId INT NOT NULL,"
                  "wordId INT NOT NULL"
                  ")");
}

void WordSetContentRep::addWordToSetTable(const int wordSetId,
                                          const int wordId) {
    std::unique_ptr<sql::PreparedStatement> prst(
            manager_.getConnection().prepareStatement("INSERT INTO " + tableName_ +
                                                     " (wordSetId, wordId) VALUES "
                                                     "(?,?)"));
    prst->setInt(1, wordSetId);
    prst->setInt(2, wordId);
    prst->execute();
}

void WordSetContentRep::deleteWordFromSet(
        int wordSetId,
        int wordId) {
    try {
        stmt_->execute("DELETE FROM " + tableName_ +
                       " WHERE wordSetId=" + std::to_string(wordSetId) +
                       " AND " + "wordId=" + std::to_string(wordId));
        historyChanges_.push_front({"wordDeleted", wordSetId, wordId});
    } catch (sql::SQLException &e) {
        std::cout << e.what();
    }
}

void WordSetContentRep::deleteWordFromAllSets(int wordId) {
    stmt_->execute("DELETE FROM " + tableName_ +
                   " WHERE wordId=" + std::to_string(wordId));
    historyChanges_.push_front({"wordDeletedFromDictionary", -1, wordId});
}

void WordSetContentRep::clear(){
    stmt_->execute("TRUNCATE " + tableName_);
}

void WordSetContentRep::clearHistory(){
    historyChanges_.clear();
}

std::vector<int> WordSetContentRep::getWordSetContent(int wordSetId) {
    std::unique_ptr<sql::ResultSet> reqRes(
            stmt_->executeQuery("SELECT wordId FROM " +
                                tableName_ + " WHERE wordSetId=" + std::to_string(wordSetId)));
    std::vector<int> words;
    while (reqRes->next()) {
        words.push_back(
                static_cast<int>(reqRes->getInt("wordId")));
    }
    return words;
}

int WordSetContentRep::getWordSetSize(int wordSetId) {
    std::unique_ptr<sql::ResultSet> reqRes(
            stmt_->executeQuery("SELECT COUNT(*) FROM " +
                                tableName_ + " WHERE wordSetId=" + std::to_string(wordSetId)));
    if (reqRes->next()) {
        int size = reqRes->getInt("COUNT(*)");
        return size;
    }
    else{
        throw WordSetContentRepException("count * bug");
    }
}

void WordSetContentRep::saveHistoryAddWordToSet(int wordSetId, int wordId){
    historyChanges_.push_front({"wordAdded", wordSetId, wordId});
}

std::deque<HistoryChangeWordSetContentRep> WordSetContentRep::getHistoryChanges() {
    return historyChanges_;
}



WordSetContentRepException::WordSetContentRepException(const std::string &message)
        : std::runtime_error(message) {
}