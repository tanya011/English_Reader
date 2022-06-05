#ifndef WORDSETCONTENTREP_H
#define WORDSETCONTENTREP_H

// DB includes
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <mutex>

#include "dbManager.h"

struct WordSetContentRepServ {

private:
    DBManager &manager_;
    std::unique_ptr<sql::Statement> stmt_;
    std::string tableName_ = "wordSetContentServer";
    std::mutex* mutex_;

public:
    explicit WordSetContentRepServ(DBManager &m, std::mutex *mutex);

    void addWordToSetTable(int userId, int wordSetId, int WordId);

    void deleteWordFromSetTable(int userId, int wordSetId, int WordId);

    void deleteWordFromAllSets(int userId, int wordId);

    std::vector<std::pair<int,int>> getUserWordSetsContents(int userId);
};

#endif  // WORDSETCONTENTREP_H