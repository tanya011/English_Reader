#ifndef YAFR_USER_H
#define YAFR_USER_H

#include "include/httplib.h"

// Here http used, so we can't send password to server safely
struct User {
private:
    bool isAuthorized_ = false;
    std::string token_;
    httplib::Client client_{"localhost:8080"};

public:
    void init(const std::string &username, const std::string &password);
    bool isAuthorized();
    void exit();
};

#endif  // YAFR_USER_H
