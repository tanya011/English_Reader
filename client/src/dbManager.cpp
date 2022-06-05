#include "include/dbManager.h"
#include <cppconn/exception.h>
#include <cppconn/statement.h>
#include "../include/config.h"

DBManager::DBManager()
    : driver_(get_driver_instance()),
      dbName_(config.get("DB_NAME")),
      con_(driver_->connect(config.get("DB_ADDRESS"),
                            config.get("DB_USER"),
                            config.get("DB_PASSWORD"))) {
    std::unique_ptr<sql::Statement> stmt(con_->createStatement());
    stmt->execute("CREATE DATABASE IF NOT EXISTS " + dbName_);
    con_->setSchema(dbName_);
}

sql::Connection &DBManager::getConnection() {
    return *con_;
}
