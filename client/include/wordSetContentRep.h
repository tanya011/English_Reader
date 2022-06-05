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
    DBManager &manager_;
    std::unique_ptr<sql::Statement> stmt_;
    std::string tableName_ = "wordSetContent";

    std::deque<HistoryChangeWordSetContentRep> historyChanges_;

public slots:
    void addWordToSetTable(int wordSetId, int WordId);

    void deleteWordFromSet(int wordSetId, int WordId);


public:
    explicit WordSetContentRep(DBManager &m);

    void deleteWordFromAllSets(int wordId);

    void clear();

    void clearHistory();

    std::vector<int> getWordSetContent(int wordSetId);

    int getWordSetSize(int wordSetId);

    void saveHistoryAddWordToSet(int wordSetId, int wordId);

    std::deque<HistoryChangeWordSetContentRep> getHistoryChanges();
};

#endif  // WORDSETCONTENTREP_H