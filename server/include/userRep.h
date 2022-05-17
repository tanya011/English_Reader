#ifndef YAFR_USERREP_H
#define YAFR_USERREP_H

// DB includes
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <mutex>
#include "dbManager.h"

struct UserNotFoundException : std::runtime_error {
    UserNotFoundException(const std::string &charact, const std::string &value);
};

struct UserRep {  // throws sql::SQLException&
    // TODO: The possibility of interrupting the connection is not handled
    // TODO: Adding of large textes isn't supposed to work good. See TODO below
private:
    DBManager &manager;
    std::string tableName = "users";
    int freeId = 0;
    std::mutex* mutex_;

public:
    UserRep(DBManager &m, std::mutex* mutex);

    void addUser(const std::string &name,
                 const std::string &hash,
                 const std::string &token);

    std::string getUserToken(const std::string &name);

    std::string getUserName(const std::string &token);

    bool isUserExist(const std::string &name, const std::string &hash);
};

#endif  // YAFR_USERREP_H
