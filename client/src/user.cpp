#include "include/user.h"
#include <nlohmann/json.hpp>

User::User(BookRep *bookRep) : bookRep_(bookRep) {
}

void User::init(const std::string &username, const std::string &password) {
    httplib::Params params{{"name", username}, {"password", password}};
    auto res = client_.Post("/init-user", params);
    if (res->status != 200)  // TODO: Check if smth went wrong
        return;
    token_ = res->body;
    isAuthorized_ = true;
}

bool User::isAuthorized() {
    return isAuthorized_;
}

void User::exit() {
    token_ = "";
    isAuthorized_ = false;
    // TODO: drop tables with books and words
}

std::vector<Book> User::getLibraryBooks() {
    std::cout << "Getting Books..." << std::endl;
    auto res = client_.Post("/library");
    if (res->status != 200)
        throw std::runtime_error("Can't load library, error code: " +
                                 std::to_string(res->status));
    nlohmann::json params = nlohmann::json::parse(res->body);
    std::vector<Book> books;
    for (auto &p : params) {
        books.emplace_back(p["id"], p["name"], p["author"]);
    }
    std::cout << "Got " << params.size() <<" books" << std::endl;
    return books;
}
void User::addBookToCollection(int bookId) {
    std::cout << "Adding book to collection" << std::endl;
    httplib::Params params;
    params.emplace("token", token_);
    params.emplace("bookId", std::to_string(bookId));
    auto res = client_.Post("/add-book", params);
    if (res->status != 200)
        throw std::runtime_error("Can't add book, error code: " +
                                 std::to_string(res->status));
    nlohmann::json book = nlohmann::json::parse(res->body);
    bookRep_->addBook(book["id"], book["name"], book["author"], book["text"]);
}
