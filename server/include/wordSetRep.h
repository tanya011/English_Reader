#ifndef WORDSETREP_H
#define WORDSETREP_H

// DB includes
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <mutex>
#include "dbManager.h"
#include "../include/wordset.h"

struct WordSetRepServ{

private:
    DBManager &manager_;
    std::unique_ptr<sql::Statement> stmt_;
    std::string tableName_ = "wordSetsServer";
    std::mutex* mutex_;

public:
    explicit WordSetRepServ(DBManager &m, std::mutex *mutex);

    void addWordSet(int userId, int id, const std::string &wordSetName);

    std::vector<WordSet> getUserWordSets(int userId);
};

#endif  // WORDSETREP_H