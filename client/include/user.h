#ifndef YAFR_USER_H
#define YAFR_USER_H

#include "include/httplib.h"

// Here http used, so we can't send password to server safely
struct user {
    int token;  // int?
    httplib::Client cli{"localhost:1234"};

    void init(const std::string& username, const std::string& password);
};

#endif  // YAFR_USER_H
