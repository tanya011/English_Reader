#include "include/bookRep.h"
#include <filesystem>
#include <sstream>

BookRep::BookRep(DBManager &m, std::mutex *mutex) : manager_(m), mutex_(mutex) {
    std::unique_lock l(*mutex_);
    std::unique_ptr<sql::Statement> stmt(
        manager_.getConnection().createStatement());
    stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName_ +
                  "("
                  "id INT NOT NULL UNIQUE,"
                  "name TINYTEXT,"
                  "author TINYTEXT,"
                  "filename TINYTEXT NOT NULL"
                  ")");

    std::unique_ptr<sql::ResultSet> maxId(
        stmt->executeQuery("SELECT MAX(id) FROM " + tableName_));
    if (maxId->next()) {
        freeId_ = maxId->getInt(1) + 1;
    }
}

int BookRep::addBook(const std::string &bookName,
                     const std::string &author,
                     const std::string &filename) {
    std::ifstream file(filename);
    if (!file.good())
        return -1;
    file.close();
    std::filesystem::path pathToBook = filename;
    pathToBook = std::filesystem::absolute(pathToBook);
    try {
        std::unique_lock l(*mutex_);
        std::unique_ptr<sql::PreparedStatement> prst(
            manager_.getConnection().prepareStatement(
                "INSERT INTO " + tableName_ +
                " (id, name, author, filename) VALUES "
                "(?,?,?,?)"));
        prst->setInt(1, freeId_);
        prst->setString(2, bookName);
        prst->setString(3, author);
        prst->setString(4, pathToBook.string());

        prst->execute();
        return freeId_++;
    } catch (sql::SQLException &e) {
        return -1;
    }
}

bool BookRep::deleteBookById(int id) {  // true if everything is ok
    try {
        std::unique_lock l(*mutex_);
        std::unique_ptr<sql::Statement> stmt(
            manager_.getConnection().createStatement());
        stmt->execute("DELETE FROM " + tableName_ +
                      " WHERE id=" + std::to_string(id));
        return true;
    } catch (sql::SQLException &e) {
        return false;
    }
}

Book BookRep::getBookById(int id) {
    mutex_->lock();
    std::unique_ptr<sql::Statement> stmt(
        manager_.getConnection().createStatement());
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
        "SELECT * FROM " + tableName_ + " WHERE id=" + std::to_string(id)));
    mutex_->unlock();
    if (reqRes->next()) {
        return Book{id, static_cast<std::string>(reqRes->getString("name")),
                    static_cast<std::string>(reqRes->getString("author")),
                    static_cast<std::string>(reqRes->getString("filename"))};
    } else {
        throw bookNotFoundException();
    }
}

std::vector<Book> BookRep::getAllBooks() {
    mutex_->lock();
    std::unique_ptr<sql::Statement> stmt(
        manager_.getConnection().createStatement());
    std::unique_ptr<sql::ResultSet> reqRes(
        stmt->executeQuery("SELECT id, name, author, filename  FROM " +
                           tableName_ + " ORDER BY name"));
    std::vector<Book> books;
    mutex_->unlock();
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
