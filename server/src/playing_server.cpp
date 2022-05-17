#include <iostream>
#include "include/dbManager.h"
#include "include/httplib.h"
#include "include/userRep.h"

using namespace std;

int main() {
    DBManager dbManager;
    std::mutex mutex;
    UserRep userRep(dbManager, &mutex);
    httplib::Server svr;
    if (userRep.isUserExist("a", "b"))
        std::cout << "Cool!" << std::endl;
    svr.Post(
        "/init-user", [&](const httplib::Request &req, httplib::Response &res) {
            cout << "Got request\n";
            if (req.has_param("name") && req.has_param("password")) {
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
                cout << "Got request: " << name << ", " << password << '\n';
            } else {
                cout << "Bad\n";
            }
        });

    svr.listen("localhost", 8080);
    return 0;
}