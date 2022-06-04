#ifndef YAFT_DB_MANAGER_H
#define YAFT_DB_MANAGER_H

#include "../include/config.h"

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
    sql::Driver *driver_;   // don't need delete
    sql::Connection *con_;  // not thread safe
    std::string dbName_;

public:
    DBManager()  // TODO: no connection options
        : driver_(get_driver_instance()) {
        Config config(CONFIG_PATH);
        dbName_ = config.get("DB_NAME");
        con_ = driver_->connect(config.get("DB_ADDRESS"), config.get("DB_USER"),
                                config.get("DB_PASSWORD"));
        std::unique_ptr<sql::Statement> stmt(con_->createStatement());
        stmt->execute("CREATE DATABASE IF NOT EXISTS " + dbName_);
        con_->setSchema(dbName_);
    }
    ~DBManager() {
        delete con_;
    }
    sql::Connection &getConnection() {
        return *con_;
    }

    bool reconnect() {
        return con_->reconnect();
    }
};

#endif
