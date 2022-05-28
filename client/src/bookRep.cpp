#include "include/bookRep.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <utility>

BookRep::BookRep(DBManager &m, std::filesystem::path appFolder)
    : manager_(m), appFolder_(std::move(appFolder)) {
    create_directories(appFolder_ / "books");
    std::unique_ptr<sql::Statement> stmt(
        manager_.getConnection().createStatement());
    stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName_ +
                  "("
                  "id INT NOT NULL UNIQUE,"
                  "name TINYTEXT,"
                  "author TINYTEXT,"
                  "filename TINYTEXT NOT NULL"
                  ")");
}

void BookRep::addBook(int id,
                      const std::string &bookName,
                      const std::string &author,
                      const std::string &filename) {
    std::ifstream file(filename);
    if (!file.good())
        throw std::runtime_error("Bad file");
    std::filesystem::path pathToBook = filename;
    pathToBook = std::filesystem::absolute(pathToBook);
    std::unique_ptr<sql::PreparedStatement> prst(
        manager_.getConnection().prepareStatement(
            "INSERT INTO " + tableName_ +
            " (id, name, author, filename) VALUES "
            "(?,?,?,?)"));
    prst->setInt(1, id);
    prst->setString(2, bookName);
    prst->setString(3, author);
    prst->setString(4, pathToBook.string());
    prst->execute();
}

void BookRep::addAndSaveBook(int id,
                             const std::string &bookName,
                             const std::string &author,
                             const std::string &text) {
    std::string filename = appFolder_.string() + std::to_string(id) + "|" + bookName +
                           "|" + author + ".txt";
    std::cout << "new_file_name : "<< filename << std::endl;
    std::ofstream file(appFolder_ / filename);
    if(!file.good())
        throw std::runtime_error("Problems with app directory");
    file<<text;
    file.close();
    this->addBook(id, bookName, author, filename);
}

void BookRep::deleteBookById(int id) {  // true if everything is ok
    std::unique_ptr<sql::Statement> stmt(
        manager_.getConnection().createStatement());
    stmt->execute("DELETE FROM " + tableName_ +
                  " WHERE id=" + std::to_string(id));
}

Book BookRep::getBookById(int id) {
    std::unique_ptr<sql::Statement> stmt(
        manager_.getConnection().createStatement());
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
        "SELECT * FROM " + tableName_ + " WHERE id=" + std::to_string(id)));

    if (reqRes->next()) {
        return Book{id, static_cast<std::string>(reqRes->getString("name")),
                    static_cast<std::string>(reqRes->getString("author")),
                    static_cast<std::string>(reqRes->getString("filename"))};
    } else {
        throw bookNotFoundException();
    }
}

std::vector<Book> BookRep::getAllBooks() {
    std::unique_ptr<sql::Statement> stmt(
        manager_.getConnection().createStatement());
    std::unique_ptr<sql::ResultSet> reqRes(
        stmt->executeQuery("SELECT id, name, author, filename  FROM " +
                           tableName_ + " ORDER BY name"));
    std::vector<Book> books;

    while (reqRes->next()) {
        books.emplace_back(
            static_cast<int>(reqRes->getInt("id")),
            static_cast<std::string>(reqRes->getString("name")),
            static_cast<std::string>(reqRes->getString("author")),
            static_cast<std::string>(reqRes->getString("filename")));
    }
    return books;
}

BookRepException::BookRepException(const std::string &message)
    : std::runtime_error(message) {
}
bookNotFoundException::bookNotFoundException()
    : BookRepException("Book is not found") {
}
