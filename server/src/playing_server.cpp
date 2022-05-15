#include <iostream>
#include "include/dbManager.h"
#include "include/httplib.h"
#include "include/userRep.h"

using namespace std;

int main() {
    DBManager dbManager;
    UserRep userRep(dbManager);
    httplib::Server svr;
    svr.Get("/init-user",
            [&](const httplib::Request &req, httplib::Response &res) {
                if (req.has_param("name") && req.has_header("password")) {
                    auto name = req.get_param_value("name");
                    auto password = req.get_param_value("password");
                    auto hash = password;  // TODO: Hash password
                    std::string token;
                    if (!userRep.isUserExist(name, hash)) {
                        token = name;  // TODO: Hash name
                        userRep.addUser(name, hash, token);
                    } else {
                        token = userRep.getUserToken(name);
                    }
                    res.set_content(token, "text/plain");
                }
            });

    svr.listen("localhost", 1234);
    return 0;
}