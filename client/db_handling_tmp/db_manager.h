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

struct dbManager {  // throws sql::SQLException&
private:
    sql::Driver *driver;                   // don't need delete
    std::unique_ptr<sql::Connection> con;  // not thread safe
    std::string dbName = "yaft";

public:
    dbManager()  // TODO: no connection options
        : driver(get_driver_instance()),
          con(driver->connect("tcp://127.0.0.1:3306", "root", "")) {
        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        stmt->execute("CREATE DATABASE IF NOT EXISTS " + dbName);
        con->setSchema(dbName);
    }

    sql::Connection &getConnection() {
        return *con;
    }

    bool reconnect() {
        return con->reconnect();
    }
};


#endif
