#include "include/user.h"
#include <nlohmann/json.hpp>
#include "include/actCollectionsHistory.h"

namespace userRepLocal{
    void newValue(int value);
}

User::User(BookRep *bookRep) : bookRep_(bookRep) {
}

void User::init(const std::string &username, const std::string &password) {
    httplib::Params params{{"name",     username},
                           {"password", password}};
    auto res = client_.Post("/init-user", params);
    if (res->status != 200)  // TODO: Check if smth went wrong
        return;
    token_ = res->body;
    isAuthorized_ = true;

    int lastCollectionAction = getLastCollectionAction();
    std::cout << "lastcoll = " << lastCollectionAction << std::endl;
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
    for (auto &p: params) {
        books.emplace_back(p["id"], p["name"], p["author"]);
    }
    std::cout << "Got " << params.size() << " books" << std::endl;
    return books;
}

std::vector<Book> User::getCollectionBooks() {
    return bookRep_->getAllBooks();
}

int User::addBookToCollection(int bookId) {
    std::unique_ptr<sql::Statement> stmt(
            bookRep_->manager_.getConnection().createStatement());
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
            "SELECT * FROM collection WHERE id=" + std::to_string(bookId)));

    if (reqRes->next()) {
        return 0;
    }

    std::cout << "Adding book to collection" << std::endl;
    httplib::Params params;
    params.emplace("token", token_);
    params.emplace("bookId", std::to_string(bookId));

    auto res = client_.Post("/add-book", params);

    if (res->status != 200)
        throw std::runtime_error("Can't add book, error code: " +
                                 std::to_string(res->status));

    nlohmann::json book = nlohmann::json::parse(res->body);
    bookRep_->addAndSaveBook(book["id"], book["name"], book["author"], book["text"]);

    sync_collect();
    newActionInCollection("add", bookId);
    userRepLocal::newValue(userRepLocal::getValue() + 1);
    return 1;
}

void User::deleteCollectionBook(int bookId) {
    std::cout << "Deleting book from server" << std::endl;

    httplib::Params params;
    params.emplace("token", token_);
    params.emplace("bookId", std::to_string(bookId));

    auto res = client_.Post("/delete-book", params);

    if (res->status != 200) {
        throw std::runtime_error("Can't add book, error code: " +
                                 std::to_string(res->status));
    } else {
        sync_collect();
        newActionInCollection("delete", bookId);
        userRepLocal::newValue(userRepLocal::getValue() + 1);
    }

}

void User::newActionInCollection(std::string action, int bookId) {
    std::cout << "Deleting book from server" << std::endl;
    httplib::Params params;
    params.emplace("token", token_);
    params.emplace("action", action);
    params.emplace("bookId", std::to_string(bookId));
    auto res = client_.Post("/new-collections-action", params);
    if (res->status != 200) {
        throw std::runtime_error("Can't add action, error code: " +
                                 std::to_string(res->status));
    }
}


int User::getLastCollectionAction() {
    std::cout << "Getting last collection action" << std::endl;
    httplib::Params params;
    params.emplace("token", token_);
    auto res = client_.Post("/get-last-collection-action", params);
    nlohmann::json param = nlohmann::json::parse(res->body);
    return param["lastAct"];
}

std::vector<ActCollectionsHistory> User::getNewActions(int startAt) {
    std::cout << "Try to get new actions from server" << std::endl ;

    httplib::Params params;
    params.emplace("token", token_);
    params.emplace("startAt", std::to_string(startAt));

    auto res = client_.Post("/new-actions", params);
    if (res->status != 200)
        throw std::runtime_error("Can't load new actions, error code: " +
                                 std::to_string(res->status));

    nlohmann::json param = nlohmann::json::parse(res->body);

    std::vector<ActCollectionsHistory> books;
    for (auto &p: param) {
        books.push_back({p["type"], p["bookId"]});
    }
    return books;
}

void User::sync_collect() {
    std::vector<ActCollectionsHistory> vec = this->getNewActions(userRepLocal::getValue() + 1);
    for (auto action: vec) {
        std::cout << " action = " << action.type << std::endl;
        if (action.type == "delete") {
            bookRep_->deleteBookById(action.bookId);
        }
        else {
            this->addBookToCollection(action.bookId);
        }
    }
    userRepLocal::newValue(userRepLocal::getValue() + vec.size());
}