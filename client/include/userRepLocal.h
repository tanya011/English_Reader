#ifndef YAFR_USERREPLOCAL_H
#define YAFR_USERREPLOCAL_H

// DB includes
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <mutex>
#include "dbManager.h"


struct UserRepLocal {
    DBManager &manager;
private:
    std::string tableName = "userCollHistory";

public:
    UserRepLocal(DBManager &m) : manager(m) {
        std::unique_ptr<sql::Statement> stmt(
                manager.getConnection().createStatement());

        stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName +
                      "("
                      "userId INT UNIQUE,"
                      "lastCollection INT"
                      ")");
    }

};

#endif  // YAFR_USERREPLOCAL_H
