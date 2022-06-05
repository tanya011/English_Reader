#include "include/wordSetRep.h"

WordSetRep::WordSetRep(DBManager &m)
        : manager_(m), stmt_(manager_.getConnection().createStatement()) {
    stmt_->execute("CREATE TABLE IF NOT EXISTS " + tableName_ +
                  "("
                  "id INT NOT NULL UNIQUE,"
                  "name TINYTEXT"
                  ")");

    std::unique_ptr<sql::ResultSet> maxId(
            stmt_->executeQuery("SELECT MAX(id) FROM " + tableName_));
    makeWordSetAllWords();
    if (maxId->next()) {
        freeId_ = std::max(maxId->getInt(1) + 1, 2);
    }
}

int WordSetRep::addWordSet(const std::string &wordSetName) {
    try {
        std::unique_ptr<sql::PreparedStatement> prst1(
                manager_.getConnection().prepareStatement("SELECT id FROM " + tableName_ + " WHERE name=?"));
        prst1->setString(1, wordSetName);

        std::unique_ptr<sql::ResultSet> reqRes(prst1->executeQuery());

        if (reqRes->next()) {
            return static_cast<int>(reqRes->getInt("id"));
        } else {
            std::unique_ptr<sql::PreparedStatement> prst2(
                    manager_.getConnection().prepareStatement("INSERT INTO " +
                                                              tableName_ +
                                                             " (id, name) VALUES "
                                                             "(?,?)"));
            prst2->setInt(1, freeId_);
            prst2->setString(2, wordSetName);

            prst2->execute();
            historyChanges_.push_front({"wordSetAdded", freeId_, wordSetName});
            return freeId_++;
        }
    } catch (sql::SQLException &e) {
        std::cout << e.what();
        return -1;
    }
}

int WordSetRep::addWordSet(WordSet &wordSet) {
    try {
        std::unique_ptr<sql::PreparedStatement> prst1(
                manager_.getConnection().prepareStatement("SELECT id FROM " + tableName_ + " WHERE id=?" + " AND name=?"));
        prst1->setInt(1, wordSet.getId());
        prst1->setString(2, wordSet.getTitle());
        std::unique_ptr<sql::ResultSet> reqRes(prst1->executeQuery());
        if (reqRes->next()) {
            return static_cast<int>(reqRes->getInt("id"));
        } else {
            std::unique_ptr<sql::PreparedStatement> prst2(
                    manager_.getConnection().prepareStatement("INSERT INTO " +
                                                              tableName_ +
                                                             " (id, name) VALUES "
                                                             "(?,?)"));
            prst2->setInt(1, wordSet.getId());
            prst2->setString(2, wordSet.getTitle());

            prst2->execute();
            freeId_ = std::max(freeId_, wordSet.getId());
            return freeId_++;
        }
    } catch (sql::SQLException &e) {
        return -1;
    }
}

void WordSetRep::makeWordSetAllWords() {
    stmt_->execute("INSERT INTO " + tableName_ +
                  "(id, name) SELECT 1, 'Все группы' FROM dual WHERE NOT "
                  "EXISTS (SELECT 1 FROM " +
                   tableName_ + ")");
    freeId_++;
}

void WordSetRep::clear(){
    stmt_->execute("TRUNCATE " + tableName_);
}

void WordSetRep::clearHistory(){
    historyChanges_.clear();
}

std::deque<HistoryChangeWordSetRep> WordSetRep::getHistoryChanges() {
    return historyChanges_;
}


std::vector<WordSet> WordSetRep::getWordSets() {
    std::unique_ptr<sql::ResultSet> reqRes(
            stmt_->executeQuery("SELECT id, name FROM " + tableName_));
    std::vector<WordSet> wordSets;
    while (reqRes->next()) {
        wordSets.emplace_back(
                static_cast<int>(reqRes->getInt("id")),
                static_cast<std::string>(reqRes->getString("name")));
    }
    return wordSets;
}


WordSetRepException::WordSetRepException(const std::string &message)
        : std::runtime_error(message) {
}