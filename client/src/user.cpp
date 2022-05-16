#include "include/user.h"

void User::init(const std::string &username, const std::string &password) {
    httplib::Params params;
    params.emplace(username, password);
    auto res = client_.Post("/init-user", params);
    token_ = res->body;
    isAuthorized_ = true;
    // TODO: Check if smth went wrong
}
bool User::isAuthorized() {
    return isAuthorized_;
}
