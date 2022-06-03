#ifndef WORDSETREP_H
#define WORDSETREP_H

// DB includes
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <QObject>
#include <deque>
#include "dbManager.h"
#include "../include/wordset.h"

struct WordSetRepException : std::runtime_error {
    explicit WordSetRepException(
            const std::string &message = "BookRep exception");
};

struct WordSetNotFoundException : WordSetRepException {
    WordSetNotFoundException();
};


struct HistoryChangeWordSetRep{
    std::string operation;
    int wordSetId;
    std::string wordSetName;
};


struct WordSetRep : public QObject {
Q_OBJECT

private:
    DBManager &manager;
    std::unique_ptr<sql::Statement> stmt;
    std::string tableName = "wordSets";
    int freeId = 1;

    std::deque<HistoryChangeWordSetRep> historyChanges_;

public:
    explicit WordSetRep(DBManager &m);

    int addWordSet(const std::string &wordSetName);

    int addWordSet(WordSet &wordSet);

    bool deleteWordSetById(int id);

    void makeWordSetAllWords();

    [[nodiscard]] std::vector<WordSet> getWordSets();

    std::deque<HistoryChangeWordSetRep> getHistoryChanges();

    void clear();

    void clearHistory();

signals:
    void wordSetWasCreated(int wordsetId, std::string title);
    void wordSetWasDeleted(int wordSetId);
};

#endif  // WORDSETREP_H