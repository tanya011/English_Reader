#include "include/bookRep.h"
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <sstream>

BookRep::BookRep(DBManager &m)
    : manager_(m), pathToBooks_(APP_FOLDER "/" + tableName_) {
    std::filesystem::create_directories(pathToBooks_);
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
        throw std::runtime_error("File of book is bad");

    std::filesystem::path pathToBook = std::filesystem::absolute(filename);

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
    std::string filename =
        std::to_string(id) + "|" + bookName + "|" + author + ".txt";
    std::filesystem::path folder(pathToBooks_ / filename);
    std::cout << "Saving book to: " + folder.string() << std::endl;

    std::ofstream file(folder, std::ios::out);
    if (!file.good())
        throw std::runtime_error("Can't open/create file to save book");
    file << text;

    this->addBook(id, bookName, author, folder);
}

void BookRep::deleteBookById(int id) {
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
        throw std::runtime_error("Book is not found");
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

void BookRep::clear() {
    std::filesystem::remove_all(pathToBooks_);
    std::filesystem::create_directories(pathToBooks_);

    std::unique_ptr<sql::Statement> stmt(
        manager_.getConnection().createStatement());
    stmt->execute("DELETE FROM " + tableName_);

    std::cout << "All books deleted" << std::endl;
}
