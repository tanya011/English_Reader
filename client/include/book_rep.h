#ifndef YAFR_BOOK_REP_H
#define YAFR_BOOK_REP_H

// DB includes
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include "book.h"
#include "db_manager.h"

struct BookRepException : std::runtime_error {
    explicit BookRepException(const std::string &message = "BookRep exception");
};

struct bookNotFoundException : BookRepException {
    bookNotFoundException();
};

struct BookRep {  // throws sql::SQLException& and BookRepException(see above)
    // TODO: The possibility of interrupting the connection is not handled
    // TODO: Adding of large textes isn't supposed to work good. See TODO below
private:
    DBManager &manager;
    std::unique_ptr<sql::Statement> stmt;
    std::string tableName = "books";
    int freeId = 0;

public:
    explicit BookRep(DBManager &m);

    std::vector<BookPreview> getBookPreview();

    int addBook(const std::string &bookName,
                const std::string &author,
                const std::string &text);

    bool deleteBookById(int id);

    Book getBookById(
        int id);  // TODO: this method should be in class of Collection
};

#endif  // YAFR_LIBRARY_REP_H
