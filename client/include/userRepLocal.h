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

    int getValue(int userId){
        std::unique_ptr<sql::Statement> stmt(
                manager.getConnection().createStatement());
        std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
                "SELECT lastCollection FROM " + tableName + " WHERE userId=" + std::to_string(userId)));
        int last = 0;
        if (reqRes->next()) {
            last = reqRes->getInt("lastCollection");
        }
        return last;
    }

    void newValue(int userId, int newValue){
        std::unique_ptr<sql::Statement> stmt(
                manager.getConnection().createStatement());
        // прибавление 1 к элементу:
        /*std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
                "SELECT lastCollection FROM " + tableName + " WHERE userId=" + std::to_string(userId)));
        int last = 0;
        if (reqRes->next()) {
            last = reqRes->getInt("lastCollection");
        }*/
        stmt->executeUpdate("UPDATE " + tableName +
                      " SET lastCollection = " + std::to_string(newValue) +" WHERE userId = " + std::to_string(userId));
    }

};

#endif  // YAFR_USERREPLOCAL_H
