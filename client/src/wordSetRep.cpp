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
    if (maxId->next()) {
        freeId = maxId->getInt(1) + 1;
    }
}

int WordSetRep::addWordSet(const std::string &wordSetName) {
    try {
        std::unique_ptr<sql::ResultSet> reqRes(
                stmt->executeQuery("SELECT id FROM " + tableName + " WHERE name='" +
                                   wordSetName + "'"));
        if (reqRes->next()) {
            return static_cast<int>(reqRes->getInt("id"));
        } else {
            std::unique_ptr<sql::PreparedStatement> prst(
                    manager.getConnection().prepareStatement("INSERT INTO " +
                                                             tableName +
                                                             " (id, name) VALUES "
                                                             "(?,?)"));
            prst->setInt(1, freeId);
            prst->setString(2, wordSetName);

            prst->execute();
            historyChanges_.push_front({"wordSetAdded", freeId, wordSetName});
            emit wordSetWasCreated(freeId, wordSetName);
            return freeId++;
        }
    } catch (sql::SQLException &e) {
        return -1;
    }
}

int WordSetRep::addWordSet(WordSet &wordSet) {
    try {
        std::unique_ptr<sql::ResultSet> reqRes(
                stmt->executeQuery("SELECT id FROM " + tableName + " WHERE id=" + std::to_string(wordSet.getId()) + " AND name='" +
                                   wordSet.getTitle() + "'"));
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
            emit wordSetWasCreated(wordSet.getId(), wordSet.getTitle());
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