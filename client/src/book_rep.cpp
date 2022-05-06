#include "include/book_rep.h"

BookRep::BookRep(DBManager &m) : manager(m) {
    std::unique_ptr<sql::Statement> stmt(
        manager.getConnection().createStatement());
    stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName +
                  "("
                  "id INT NOT NULL UNIQUE,"
                  "name TINYTEXT,"
                  "author TINYTEXT,"
                  "text MEDIUMTEXT NOT NULL"
                  ")");

    std::unique_ptr<sql::ResultSet> maxId(
        stmt->executeQuery("SELECT MAX(id) FROM " + tableName));
    if (maxId->next()) {
        freeId = maxId->getInt(1) + 1;
    }
}

int BookRep::addBook(
    const std::string &bookName,
    const std::string &author,
    const std::string &text) {  // returns id of added book, or -1
    // if smth went wrong
    // TODO: May be better to use "LOAD" from some file for book text to
    // awoid copying
    try {
        std::unique_ptr<sql::PreparedStatement> prst(
            manager.getConnection().prepareStatement(
                "INSERT INTO " + tableName +
                " (id, name, author, text) VALUES "
                "(?,?,?,?)"));
        prst->setInt(1, freeId);
        prst->setString(2, bookName);
        prst->setString(3, author);
        prst->setString(4, text);

        prst->execute();
        return freeId++;
    } catch (sql::SQLException &e) {
        // std::cout << e.what();
        return -1;
    }
}

bool BookRep::deleteBookById(int id) {  // true if everything is ok
    try {
        std::unique_ptr<sql::Statement> stmt(
            manager.getConnection().createStatement());
        stmt->execute("DELETE FROM " + tableName +
                      " WHERE id=" + std::to_string(id));
        return true;
    } catch (sql::SQLException &e) {
        // std::cout << e.what();
        return false;
    }
}

Book BookRep::getBookById(int id) {
    std::unique_ptr<sql::Statement> stmt(
        manager.getConnection().createStatement());
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
        "SELECT * FROM " + tableName + " WHERE id=" + std::to_string(id)));
    if (reqRes->next()) {
        return Book{id, static_cast<std::string>(reqRes->getString("name")),
                    static_cast<std::string>(reqRes->getString("author")),
                    static_cast<std::string>(reqRes->getString("text"))};
    } else {
        throw bookNotFoundException();
    }
}

std::vector<BookPreview> BookRep::getBookPreview() {
    std::unique_ptr<sql::Statement> stmt(
        manager.getConnection().createStatement());
    std::unique_ptr<sql::ResultSet> reqRes(stmt->executeQuery(
        "SELECT id, name, author  FROM " + tableName + " ORDER BY name"));
    std::vector<BookPreview> bookPreviews;
    while (reqRes->next()) {
        bookPreviews.emplace_back(
            static_cast<int>(reqRes->getInt("id")),
            static_cast<std::string>(reqRes->getString("name")),
            static_cast<std::string>(reqRes->getString("author")));
    }
    return bookPreviews;
}

BookRepException::BookRepException(const std::string &message)
    : std::runtime_error(message) {
}
bookNotFoundException::bookNotFoundException()
    : BookRepException("Book is not found") {
}
