#ifndef WORDREP_H
#define WORDREP_H

// DB includes
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <mutex>
#include <vector>
#include "dbManager.h"
#include "../include/word.h"


struct WordRepServ{

private:
    DBManager &manager_;
    std::unique_ptr<sql::Statement> stmt;
    std::string tableName = "wordsServer";
    std::mutex* mutex_;

public:
    explicit WordRepServ(DBManager &m, std::mutex *mutex);

    void addWord(int userId,
                 int id,
                 const std::string &original,
                 const std::string &translation,
                 const std::string &context = "");

    void deleteWordById(int userId,
                        int id);

    std::vector<Word> getUserWords(int userId);


};

#endif  // WORDREP_H