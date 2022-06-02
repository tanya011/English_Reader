#ifndef WORDREP_H
#define WORDREP_H

// DB includes
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <QObject>
#include <vector>
#include <deque>
#include "dbManager.h"
#include "../include/word.h"

struct WordRepException : std::runtime_error {
    explicit WordRepException(const std::string &message = "WordRep exception");
};

struct WordNotFoundException : WordRepException {
    WordNotFoundException();
};



struct HistoryChangeWordRep{
    std::string operation;
    int wordId;
    std::string original, translation, context;
};

struct WordRep : public QObject {
Q_OBJECT

private:
    DBManager &manager;
    std::unique_ptr<sql::Statement> stmt;
    std::string tableName = "words";
    int freeId = 1;

    std::deque<HistoryChangeWordRep> historyChanges_;

public:
    explicit WordRep(DBManager &m);
    [[nodiscard]] std::vector<Word> downloadWords();

    int addWord(const std::string &original,
                const std::string &translation,
                const std::string &context = "");

    int addWord(Word &word);

    void deleteWordById(int id);

    void clear();

    void clearHistory();

    Word getWordById(int wordId);

    std::deque<HistoryChangeWordRep> getHistoryChanges();

signals:
    void wordCreated(int wordSetId, int wordId);
};

#endif  // WORDREP_H