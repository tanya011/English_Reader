#include "include/wordSetRep.h"
#include <qdebug.h>

WordSetRep::WordSetRep(DBManager &m)
        : manager(m), stmt(manager.getConnection().createStatement()) {
    stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName +
                  "("
                  "id INT NOT NULL UNIQUE,"
                  "name TINYTEXT"
                  ")");

    std::unique_ptr<sql::ResultSet> maxId(
            stmt->executeQuery("SELECT MAX(id) FROM " + tableName));
    makeWordSetAllWords();
    if (maxId->next()) {
        freeId = maxId->getInt(1) + 1;
    }
}

int WordSetRep::addWordSet(const std::string &wordSetName) {
    try {
        std::unique_ptr<sql::PreparedStatement> prst1(
                manager.getConnection().prepareStatement("SELECT id FROM " + tableName + " WHERE name=?"));
        prst1->setString(1, wordSetName);

        std::unique_ptr<sql::ResultSet> reqRes(prst1->executeQuery());

        if (reqRes->next()) {
            return static_cast<int>(reqRes->getInt("id"));
        } else {
            std::unique_ptr<sql::PreparedStatement> prst2(
                    manager.getConnection().prepareStatement("INSERT INTO " +
                                                             tableName +
                                                             " (id, name) VALUES "
                                                             "(?,?)"));
            prst2->setInt(1, freeId);
            prst2->setString(2, wordSetName);

            prst2->execute();
            historyChanges_.push_front({"wordSetAdded", freeId, wordSetName});
            return freeId++;
        }
    } catch (sql::SQLException &e) {
        std::cout << e.what();
        return -1;
    }
}

int WordSetRep::addWordSet(WordSet &wordSet) {
    try {
        std::unique_ptr<sql::PreparedStatement> prst1(
                manager.getConnection().prepareStatement("SELECT id FROM " + tableName + " WHERE id=?" + " AND name=?'"));
        prst1->setInt(1, wordSet.getId());
        prst1->setString(2, wordSet.getTitle());
        std::unique_ptr<sql::ResultSet> reqRes(prst1->executeQuery());
        if (reqRes->next()) {
            return static_cast<int>(reqRes->getInt("id"));
        } else {
            std::unique_ptr<sql::PreparedStatement> prst(
                    manager.getConnection().prepareStatement("INSERT INTO " +
                                                             tableName +
                                                             " (id, name) VALUES "
                                                             "(?,?)"));
            prst->setInt(1, wordSet.getId());
            prst->setString(2, wordSet.getTitle());

            prst->execute();
            freeId = std::max(freeId, wordSet.getId());
            return freeId++;
        }
    } catch (sql::SQLException &e) {
        return -1;
    }
}

void WordSetRep::makeWordSetAllWords() {
    stmt->execute("INSERT INTO " + tableName +
                  "(id, name) SELECT 1, 'Все группы' FROM dual WHERE NOT "
                  "EXISTS (SELECT 1 FROM " +
                  tableName + ")");
    freeId++;
}

bool WordSetRep::deleteWordSetById(int id) {
    try {
        stmt->execute("DELETE FROM " + tableName +
                      " WHERE id=" + std::to_string(id));
        return true;
    } catch (sql::SQLException &e) {
        return false;
    }
}

WordSetRepException::WordSetRepException(const std::string &message)
        : std::runtime_error(message) {
}
WordSetNotFoundException::WordSetNotFoundException()
        : WordSetRepException("Word set is not found") {
}

std::vector<WordSet> WordSetRep::getWordSets() {
    std::unique_ptr<sql::ResultSet> reqRes(
            stmt->executeQuery("SELECT id, name FROM " + tableName));
    std::vector<WordSet> wordSets;
    while (reqRes->next()) {
        wordSets.emplace_back(
                static_cast<int>(reqRes->getInt("id")),
                static_cast<std::string>(reqRes->getString("name")));
    }
    return wordSets;
}

void WordSetRep::clear(){
    stmt->execute("TRUNCATE " + tableName);
}

void WordSetRep::clearHistory(){
    historyChanges_.clear();
}

std::deque<HistoryChangeWordSetRep> WordSetRep::getHistoryChanges() {
    return historyChanges_;
}