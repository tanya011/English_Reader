#include "include/user.h"
#include <nlohmann/json.hpp>
#include "include/actCollectionsHistory.h"

User::User(BookRep *bookRep, UserRepLocal *userRepLocal) : bookRep_(bookRep), userRepLocal_(userRepLocal) {
}

void User::init(const std::string &username, const std::string &password) {
    httplib::Params params{{"name",     username},
                           {"password", password}};
    auto res = client_.Post("/init-user", params);
    if (res->status != 200)  // TODO: Check if smth went wrong
        return;
    token_ = res->body;
    isAuthorized_ = true;

    // табличка для синхронизации личной коллекции книг
    std::unique_ptr<sql::Statement> stmt(
            userRepLocal_->manager.getConnection().createStatement());
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
            "SELECT * FROM userCollHistory WHERE userId=" + token_));
    if (reqRes->next()) {

    } else {
        std::unique_ptr<sql::PreparedStatement> prst(
                userRepLocal_->manager.getConnection().prepareStatement(
                        "INSERT INTO userCollHistory (userId, lastCollection) VALUES "
                        "(?,?)"));
        prst->setString(1, token_);
        prst->setInt(2, 0);
        prst->execute();
    }
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
    userRepLocal_->newValue(1, userRepLocal_->getValue(1) + 1);
    return 1;
}

void User::deleteCollectionBook(int bookId) {
    std::cout << "Deleting book from server";

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
        userRepLocal_->newValue(1, userRepLocal_->getValue(1) + 1);
    }

}

void User::newActionInCollection(std::string action, int bookId) {
    std::cout << "Deleting book from server";
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


std::vector<ActCollectionsHistory> User::getNewActions(int startAt) {
    std::cout << "Try to get new actions from server";

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