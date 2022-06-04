#include "include/wordRep.h"
#include <qdebug.h>

WordRep::WordRep(DBManager &m)
        : manager(m), stmt(manager.getConnection().createStatement()) {
    stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName +
                  "("
                  "id INT NOT NULL UNIQUE,"
                  "original TINYTEXT,"
                  "translation TINYTEXT,"
                  "context TINYTEXT"
                  ")");

    std::unique_ptr<sql::ResultSet> maxId(
            stmt->executeQuery("SELECT MAX(id) FROM " + tableName));
    if (maxId->next()) {
        freeId = maxId->getInt(1) + 1;
    }
}

int WordRep::addWord(const std::string &original,
                     const std::string &translation,
                     const std::string &context) {
    try {
        std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
                "SELECT id FROM " + tableName + " WHERE original='" + original +
                "' AND translation='" + translation + "'"));
        if (reqRes->next()) {
            return static_cast<int>(reqRes->getInt("id"));
        } else {
            std::unique_ptr<sql::PreparedStatement> prst(
                    manager.getConnection().prepareStatement(
                            "INSERT INTO " + tableName +
                            " (id, original, translation, context) VALUES "
                            "(?,?,?,?)"));
            prst->setInt(1, freeId);
            prst->setString(2, original);
            prst->setString(3, translation);
            prst->setString(4, context);

            prst->execute();
            historyChanges_.push_front({"wordAdded", freeId, original, translation, context});
            emit wordCreated(1, freeId);
            return freeId++;
        }
    } catch (sql::SQLException &e) {
        return -1;
    }
}

int WordRep::addWord(Word &word) {
    try {
        std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
                "SELECT id FROM " + tableName + " WHERE id=" + std::to_string(word.getId()) + " AND original='" + word.getOriginal() +
                "' AND translation='" + word.getTranslation() + "'"));
        if (reqRes->next()) {
            return static_cast<int>(reqRes->getInt("id"));
        } else {
            std::unique_ptr<sql::PreparedStatement> prst(
                    manager.getConnection().prepareStatement(
                            "INSERT INTO " + tableName +
                            " (id, original, translation, context) VALUES "
                            "(?,?,?,?)"));
            prst->setInt(1, word.getId());
            prst->setString(2, word.getOriginal());
            prst->setString(3, word.getTranslation());
            prst->setString(4, word.getContext());

            prst->execute();
            freeId = std::max(freeId, word.getId());
            emit wordCreated(1, word.getId());
            return word.getId();
        }
    } catch (sql::SQLException &e) {
        return -1;
    }
}

void WordRep::deleteWordById(int id) {
    stmt->execute("DELETE FROM " + tableName +
                  " WHERE id=" + std::to_string(id));
    historyChanges_.push_front({"wordDeleted", id});
}

std::vector<Word> WordRep::downloadWords() {
    std::unique_ptr<sql::ResultSet> reqRes(
            stmt->executeQuery("SELECT id, original, translation, context  FROM " +
                               tableName + " ORDER BY original"));
    std::vector<Word> words;
    while (reqRes->next()) {
        words.emplace_back(
                static_cast<int>(reqRes->getInt("id")),
                static_cast<std::string>(reqRes->getString("original")),
                static_cast<std::string>(reqRes->getString("translation")),
                static_cast<std::string>(reqRes->getString("context")));
    }
    return words;
}

void WordRep::clear(){
    stmt->execute("TRUNCATE " + tableName);
}

Word WordRep::getWordById(int wordId) {
    std::unique_ptr<sql::ResultSet> reqRes(
            stmt->executeQuery("SELECT * FROM " + tableName + " WHERE id=" + std::to_string(wordId)));
    if (reqRes->next()) {
        Word word = Word(static_cast<int>(reqRes->getInt("id")),
                         static_cast<std::string>(reqRes->getString("original")),
                         static_cast<std::string>(reqRes->getString("translation")),
                         static_cast<std::string>(reqRes->getString("context")));
        return word;
    }
    else{
        throw WordRepException("trying to get word, which doesnt exists");
    }
}

std::deque<HistoryChangeWordRep> WordRep::getHistoryChanges() {
    return historyChanges_;
}

void WordRep::clearHistory(){
    historyChanges_.clear();
}

WordRepException::WordRepException(const std::string &message)
        : std::runtime_error(message) {
}
WordNotFoundException::WordNotFoundException()
        : WordRepException("Word is not found") {
}