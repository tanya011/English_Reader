#ifndef YAFT_DB_MANAGER_H
#define YAFT_DB_MANAGER_H


// DB includes
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>

// This class handles the app DB, which is supposed to contain two tables:
// library and users.
// TODO: may be it is a bad idea to keep both library and users at the same DB

struct DBManager {  // throws sql::SQLException&
private:
    sql::Driver *driver_;// don't need delete
    std::unique_ptr<sql::Connection> con_;  // not thread safe
    const std::string dbName_;

public:

    DBManager(std::string dbName, std::string dbAddress, std::string dBUser, std::string dbPassword)
        : dbName_(dbName), driver_(get_driver_instance()),
          con_(driver_->connect(dbAddress, dBUser, dbPassword)) {
        std::unique_ptr<sql::Statement> stmt(con_->createStatement());
        stmt->execute("CREATE DATABASE IF NOT EXISTS " + dbName_);
        con_->setSchema(dbName_);

    }

    sql::Connection &getConnection() {
        return *con_;
    }

    bool reconnect() {
        return con_->reconnect();
    }
};


#endif
