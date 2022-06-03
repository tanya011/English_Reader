#ifndef WORDSETCONTENTREP_H
#define WORDSETCONTENTREP_H

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

struct WordSetContentRepException : std::runtime_error {
    explicit WordSetContentRepException(const std::string &message = "WordRep exception");
};


struct HistoryChangeWordSetContentRep{
    std::string operation;
    int wordSetId, wordId;
};


struct WordSetContentRep : public QObject {
Q_OBJECT

private:
    DBManager &manager;
    std::unique_ptr<sql::Statement> stmt;
    std::string tableName = "wordSetContent";

    std::deque<HistoryChangeWordSetContentRep> historyChanges_;

public:
    explicit WordSetContentRep(DBManager &m);

    std::vector<std::pair<int, int>> downloadWordSetsContentData();

    void clear();

    void clearHistory();

    std::vector<int> getWordSetContent(int wordSetId);

    int getWordSetSize(int wordSetId);

    void deleteWordFromAllSets(int wordId);

    void saveHistoryAddWordToSet(int wordSetId, int wordId);

    std::deque<HistoryChangeWordSetContentRep> getHistoryChanges();

public slots:
    void addWordToSetTable(int wordSetId, int WordId);

    bool deleteWordFromSet(int wordSetId, int WordId);

};

#endif  // WORDSETCONTENTREP_H