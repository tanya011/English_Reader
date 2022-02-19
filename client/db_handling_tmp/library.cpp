#include "library.h"


Library::Library(dbManager &m)
    : manager(m), stmt(manager.getConnection().createStatement()) {
    stmt->execute("CREATE TABLE IF NOT EXISTS " + tableName +
                  "("
                  "bookId INT NOT NULL UNIQUE,"
                  "bookName TINYTEXT,"
                  "author TINYTEXT,"
                  "text MEDIUMTEXT NOT NULL"
                  ")");

    std::unique_ptr<sql::ResultSet> maxId(
        stmt->executeQuery("SELECT MAX(bookId) FROM " + tableName));
    if (maxId->next()) {
        freeId = maxId->getInt(1) + 1;
    }
}

int Library::addBook(const std::string &bookName,
                     const std::string &author,
                     const std::string &text) {  // returns id of added book, or -1
    // if smth went wrong
    // TODO: May be better to use "LOAD" from some file for book text to
    // awoid copying
    try {
        std::unique_ptr<sql::PreparedStatement> prst(
            manager.getConnection().prepareStatement(
                "INSERT INTO " + tableName +
                " (bookId, bookName, author, text) VALUES "
                "(?,?,?,?)"));
        prst->setInt(1, freeId);
        prst->setString(2, bookName);
        prst->setString(3, author);
        prst->setString(4, text);

        prst->execute();
        return freeId++;
    } catch (sql::SQLException &e) {
        std::cout << e.what();
        return -1;
    }
}

bool Library::deleteBookById(int id) {  // true if everything is ok
    try {
        stmt->execute("DELETE FROM " + tableName +
                      " WHERE bookId=" + std::to_string(id));
        return true;
    } catch (sql::SQLException &e) {
        std::cout << e.what();
        return false;
    }
}
Book Library::getBookById(
    int id) {
    std::unique_ptr<sql::ResultSet> book(stmt->executeQuery(
        "SELECT * FROM " + tableName + " WHERE bookId=" + std::to_string(id)));
    if (book->next()) {
        return Book{static_cast<std::string>(book->getString("bookName")),
                    static_cast<std::string>(book->getString("author")),
                    static_cast<std::string>(book->getString("text"))};
    } else {
        throw bookNotFoundException();
    }
}
