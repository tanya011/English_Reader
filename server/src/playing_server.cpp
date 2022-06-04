#include <openssl/sha.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../include/httplib.h"
#include "include/bookRep.h"
#include "include/collectionsHistoryRep.h"
#include "include/collectionsRep.h"
#include "include/dbManager.h"
#include "include/userRep.h"

std::string SHA256(std::string password, std::string salt = "") {
    if (!salt.empty())
        for (int i = 0; i < password.size(); i++) {
            password[i] += salt[i % salt.size()];
        }
    auto input = password.c_str();
    size_t length = password.size();
    unsigned char md[SHA256_DIGEST_LENGTH];
    SHA256_CTX context;
    if (!SHA256_Init(&context))
        throw std::runtime_error("SHA256_Init failed");

    if (!SHA256_Update(&context, (unsigned char *)input, length))
        throw std::runtime_error("SHA256_Update failed");

    if (!SHA256_Final(md, &context))
        throw std::runtime_error("SHA256_Final failed");
    std::stringstream tmp;
    tmp << md;
    return tmp.str();
}

int main() {
    std::ifstream configFile("/home/ekaterina/project-yafr/English_Reader/server/config.json");
    std::stringstream buf;
    buf<<configFile.rdbuf();
    nlohmann::json config = nlohmann::json::parse(buf.str());

    DBManager dbManagerUserRep(config["DB_NAME"], config["DB_ADDRESS"],
                               config["DB_USER"], config["DB_PASSWORD"]);
    std::mutex mutexUserRep;
    UserRep userRep(dbManagerUserRep, &mutexUserRep);

    DBManager dbManagerBookRep(config["DB_NAME"], config["DB_ADDRESS"],
                               config["DB_USER"], config["DB_PASSWORD"]);
    std::mutex mutexBookRep;
    BookRep bookRep(dbManagerBookRep, &mutexUserRep);

    DBManager dbManagerCollectionsRep(config["DB_NAME"], config["DB_ADDRESS"],
                                      config["DB_USER"], config["DB_PASSWORD"]);
    std::mutex mutexCollectionsRep;
    CollectionsRep collectionsRep(dbManagerCollectionsRep,
                                  &mutexCollectionsRep);

    DBManager dbManagerCollectionsHistoryRepRep(
        config["DB_NAME"], config["DB_ADDRESS"], config["DB_USER"],
        config["DB_PASSWORD"]);
    std::mutex mutexCollectionsHistoryRep;
    CollectionsHistoryRep collectionsHistoryRep(
        dbManagerCollectionsHistoryRepRep, &mutexCollectionsHistoryRep);

#if 0  // =1 on first run to load books
    int id1 = bookRep.addBook(
            "The Beatles", "Paul Shipton",
            "/home/tatyana/Programming/Проект Весна 2022/English_Reader/client/src/books/Beatles.txt");
    int id2 = bookRep.addBook("Hatiko", "Nicole Irving",
                              "/home/tatyana/Programming/Проект Весна 2022/English_Reader/client/src/books/book_Hachiko.txt");
    int id3 = bookRep.addBook("Harry Potter and the Philosopher's Stone",
                              "Joanne Katheline Rowling",
                              "/home/tatyana/Programming/Проект Весна 2022/English_Reader/client/src/books/Harry_Potter1.txt");
#endif

    std::thread t([&]() {
        std::cout << "To add book write 'add <name>, <author>, <path>"
                  << std::endl;
        // std::cin>>
    });
    t.detach();

    httplib::Server svr;
    svr.Post("/init-user",
             [&](const httplib::Request &req, httplib::Response &res) {
                 std::cout << "/init-user" << std::endl;
                 if (req.has_param("name") && req.has_param("password")) {
                     auto name = req.get_param_value("name");
                     auto password = req.get_param_value("password");
                     auto hash = SHA256(password);
                     std::string token;
                     if (!userRep.isUserExist(name, hash)) {
                         token = SHA256(name);
                         userRep.addUser(name, hash, token);
                     } else {
                         token = userRep.getUserToken(name);
                     }
                     res.set_content(token, "text/plain");
                     std::cout << "Got request: " << name << ", " << password
                               << std::endl;
                 } else {
                     throw std::runtime_error("Wrong /init-user request");
                 }
             });

    svr.Post("/library",
             [&](const httplib::Request &req, httplib::Response &res) {
                 std::cout << "/library" << std::endl;
                 std::vector<Book> books(bookRep.getAllBooks());
                 std::cout << "Now there are " << books.size()
                           << " books in Library" << std::endl;
                 nlohmann::json params;
                 for (auto &book : books) {
                     params.push_back({{"id", book.getId()},
                                       {"name", book.getName()},
                                       {"author", book.getAuthor()}});
                 }
                 res.set_content(params.dump(), "text/plain");
             });

    svr.Post("/collection",
             [&](const httplib::Request &req, httplib::Response &res) {
                 std::cout << "/collection" << std::endl;
                 if (req.has_param("token")) {
                     auto token = req.get_param_value("token");
                     int userId = userRep.getUserId(token);
                     std::vector<Book> books;
                     for (auto bookId : collectionsRep.getUserBookId(userId)) {
                         books.push_back(bookRep.getBookById(bookId));
                     }
                     std::cout << "Now there are " << books.size()
                               << " books in Library" << std::endl;
                     nlohmann::json params;
                     for (auto &book : books) {
                         params.push_back({{"id", book.getId()},
                                           {"name", book.getName()},
                                           {"author", book.getAuthor()},
                                           {"text", book.getText()}});
                     }
                     res.set_content(params.dump(), "text/plain");
                 } else {
                     throw std::runtime_error("No token given");
                 }
             });

    svr.Post("/add-book",
             [&](const httplib::Request &req, httplib::Response &res) {
                 std::cout << "/add_book" << std::endl;
                 if (req.has_param("token") && req.has_param("bookId")) {
                     auto token = req.get_param_value("token");
                     int userId = userRep.getUserId(token);
                     int bookId = std::stoi(req.get_param_value("bookId"));
                     collectionsRep.addBookToUser(userId, bookId);
                     Book book = bookRep.getBookById(bookId);
                     nlohmann::json param{
                         {"id", book.getId()},
                         {"name", book.getName()},
                         {"author", book.getAuthor()},
                         {"text", book.getText()},
                         //{"filename", book.getFileName()}
                     };
                     res.set_content(param.dump(), "text/plain");
                 } else {
                     throw std::runtime_error("No token or bookId given");
                 }
             });

    svr.Post("/delete-book",
             [&](const httplib::Request &req, httplib::Response &res) {
                 std::cout << "/delete_book" << std::endl;
                 if (req.has_param("token") && req.has_param("bookId")) {
                     auto token = req.get_param_value("token");
                     int userId = userRep.getUserId(token);
                     int bookId = std::stoi(req.get_param_value("bookId"));
                     collectionsRep.deleteBookFromUser(userId, bookId);
                     res.set_content(token, "text/plain");
                 } else {
                     throw std::runtime_error("No token or bookId given");
                 }
             });

    svr.Post("/new-collections-action", [&](const httplib::Request &req,
                                            httplib::Response &res) {
        std::cout << "/new_action_in_histCollection" << std::endl;
        if (req.has_param("token") && req.has_param("action") &&
            req.has_param("bookId")) {
            auto token = req.get_param_value("token");
            int userId = userRep.getUserId(token);
            int bookId = std::stoi(req.get_param_value("bookId"));
            std::string action = req.get_param_value("action");
            std::cout << userId << " " << bookId << " " << action << std::endl;
            collectionsHistoryRep.addInHistory(userId, {action, bookId});
            res.set_content(token, "text/plain");
        } else {
            throw std::runtime_error("No token or bookId given");
        }
    });

    svr.Post("/user-id",
             [&](const httplib::Request &req, httplib::Response &res) {
                 if (req.has_param("token")) {
                     auto token = req.get_param_value("token");
                     int userId = userRep.getUserId(token);
                     nlohmann::json param{{"userId", userId}};
                     res.set_content(param.dump(), "text/plain");
                 } else {
                     throw std::runtime_error("No token or bookId given");
                 }
             });

    svr.Post("/new-actions", [&](const httplib::Request &req,
                                 httplib::Response &res) {
        std::cout << "/new-actions" << std::endl;
        if (req.has_param("token") && req.has_param("startAt")) {
            auto token = req.get_param_value("token");
            int userId = userRep.getUserId(token);
            int startAt = std::stoi(req.get_param_value("startAt"));

            std::vector<ActCollectionsHistory> books(
                collectionsHistoryRep.giveHistoryById(userId, startAt));

            nlohmann::json params;

            for (auto &book : books) {
                params.push_back(
                    {{"type", book.type}, {"bookId", book.bookId}});
            }
            res.set_content(params.dump(), "text/plain");
        } else {
            throw std::runtime_error("No token or bookId given");
        }
    });

    svr.Post("/get-last-collection-action",
             [&](const httplib::Request &req, httplib::Response &res) {
                 if (req.has_param("token")) {
                     auto token = req.get_param_value("token");
                     int userId = userRep.getUserId(token);
                     int lastAction = collectionsHistoryRep.lastAction(userId);
                     nlohmann::json param{{"lastAct", lastAction}};
                     res.set_content(param.dump(), "text/plain");
                 } else {
                     throw std::runtime_error("No token given");
                 }
             });

    svr.set_exception_handler(
        [](const auto &req, auto &res, std::exception &e) {
            res.status = 500;
            std::cerr << e.what() << std::endl;
            res.set_content("Error", "text/hplain");
        });

    svr.listen("localhost", 8080);
    // std::cout << 2;
    return 0;
}