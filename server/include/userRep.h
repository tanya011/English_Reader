#ifndef YAFR_USERREP_H
#define YAFR_USERREP_H

#include <mutex>
#include "dbManager.h"

struct UserNotFoundException : std::runtime_error {
    UserNotFoundException(const std::string &charact, const std::string &value);
};

struct UserRep {
private:
    DBManager &manager;
    std::string tableName = "users";
    int freeId = 0;
    std::mutex *mutex_;

public:
    UserRep(DBManager &m, std::mutex *mutex);

    void addUser(const std::string &name,
                 const std::string &hash,
                 const std::string &token);

    std::string getUserToken(const std::string &name,
                             const std::string &passwordHash);

    std::string getUserName(const std::string &token);

    int getUserId(const std::string &token);

    bool isUserExist(const std::string &name, const std::string &hash);
};

#endif  // YAFR_USERREP_H
