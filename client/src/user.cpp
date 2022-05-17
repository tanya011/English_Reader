#include "include/user.h"

void User::init(const std::string &username, const std::string &password) {
    httplib::Params params{{"name", username}, {"password", password}};
    auto res = client_.Post("/init-user", params);
    token_ = res->body;
   // auto x=res->status;
    isAuthorized_ = true;
    // TODO: Check if smth went wrong
}
bool User::isAuthorized() {
    return isAuthorized_;
}
void User::exit() {
    token_ = "";
    isAuthorized_ = false;
}
