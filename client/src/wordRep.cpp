#include "include/wordRep.h"

WordRep::WordRep(DBManager &m)
        : manager_(m), stmt_(manager_.getConnection().createStatement()) {
    stmt_->execute("CREATE TABLE IF NOT EXISTS " + tableName_ +
                  "("
                  "id INT NOT NULL UNIQUE,"
                  "original TINYTEXT,"
                  "translation TINYTEXT,"
                  "context TINYTEXT"
                  ")");

    std::unique_ptr<sql::ResultSet> maxId(
            stmt_->executeQuery("SELECT MAX(id) FROM " + tableName_));
    if (maxId->next()) {
        freeId_ = maxId->getInt(1) + 1;
    }
}

int WordRep::addWord(const std::string &original,
                     const std::string &translation,
                     const std::string &context) {
    try {
        std::unique_ptr<sql::PreparedStatement> prst1(
                manager_.getConnection().prepareStatement("SELECT id FROM " + tableName_ + " WHERE original=?" +
                                                          " AND translation=?"));
        prst1->setString(1, original);
        prst1->setString(2, translation);
        std::unique_ptr<sql::ResultSet> reqRes(prst1->executeQuery());
        if (reqRes->next()) {
            return static_cast<int>(reqRes->getInt("id"));
        } else {
            std::unique_ptr<sql::PreparedStatement> prst2(
                    manager_.getConnection().prepareStatement(
                            "INSERT INTO " + tableName_ +
                            " (id, original, translation, context) VALUES "
                            "(?,?,?,?)"));
            freeId_++;
            prst2->setInt(1, freeId_);
            prst2->setString(2, original);
            prst2->setString(3, translation);
            prst2->setString(4, context);
            prst2->execute();
            historyChanges_.push_front({"wordAdded", freeId_, original, translation, context});
            emit wordAdded(1, freeId_);
            return freeId_;
        }
    } catch (sql::SQLException &e) {
        return -1;
    }
}

int WordRep::addWord(Word &word) {
    try {
        std::unique_ptr<sql::PreparedStatement> prst1(manager_.getConnection().prepareStatement("SELECT id FROM " + tableName_ + " WHERE id=?" + " AND original=?" +
                                                                                                " AND translation=?"));
        prst1->setInt(1, word.getId());
        prst1->setString(2, word.getOriginal());
        prst1->setString(3, word.getTranslation());
        std::unique_ptr<sql::ResultSet> reqRes(prst1->executeQuery());
        if (reqRes->next()) {
            return static_cast<int>(reqRes->getInt("id"));
        } else {
            std::unique_ptr<sql::PreparedStatement> prst2(
                    manager_.getConnection().prepareStatement(
                            "INSERT INTO " + tableName_ +
                            " (id, original, translation, context) VALUES "
                            "(?,?,?,?)"));
            prst2->setInt(1, word.getId());
            prst2->setString(2, word.getOriginal());
            prst2->setString(3, word.getTranslation());
            prst2->setString(4, word.getContext());

            prst2->execute();
            freeId_ = std::max(freeId_, word.getId());
            emit wordAdded(1, word.getId());
            return word.getId();
        }
    } catch (sql::SQLException &e) {
        return -1;
    }
}

void WordRep::deleteWordById(int id) {
    stmt_->execute("DELETE FROM " + tableName_ +
                   " WHERE id=" + std::to_string(id));
    historyChanges_.push_front({"wordDeleted", id});
}

void WordRep::clear(){
    stmt_->execute("TRUNCATE " + tableName_);
}

void WordRep::clearHistory(){
    historyChanges_.clear();
}

std::deque<HistoryChangeWordRep> WordRep::getHistoryChanges() {
    return historyChanges_;
}

Word WordRep::getWordById(int wordId) {
    std::unique_ptr<sql::ResultSet> reqRes(
            stmt_->executeQuery("SELECT * FROM " + tableName_ + " WHERE id=" + std::to_string(wordId)));
    if (reqRes->next()) {
        Word word = Word(static_cast<int>(reqRes->getInt("id")),
                         static_cast<std::string>(reqRes->getString("original")),
                         static_cast<std::string>(reqRes->getString("translation")),
                         static_cast<std::string>(reqRes->getString("context")));
        return word;
    }
    else{
        throw WordNotFoundException();
    }
}

std::vector<Word> WordRep::getWords() {
    std::unique_ptr<sql::ResultSet> reqRes(stmt_->executeQuery(
            "SELECT * FROM " + tableName_));
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

WordRepException::WordRepException(const std::string &message)
        : std::runtime_error(message) {
}
WordNotFoundException::WordNotFoundException()
        : WordRepException("Word is not found") {
}