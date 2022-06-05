#ifndef YAFT_DB_MANAGER_H
#define YAFT_DB_MANAGER_H

#include <cppconn/driver.h>
#include <mysql_connection.h>

struct DBManager {
private:
    sql::Driver *driver_;                   // doesn't need delete
    std::unique_ptr<sql::Connection> con_;  // not thread safe
    std::string dbName_;

public:
    DBManager();
    sql::Connection &getConnection();
};

#endif
