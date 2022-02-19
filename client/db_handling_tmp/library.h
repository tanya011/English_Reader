#ifndef YAFT_LIBRARY_H
#define YAFT_LIBRARY_H

// All this stuff is written following these instructions:
//

// DB includes
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>


#include "book.h"
#include "db_manager.h"

struct libraryException : std::runtime_error {
    libraryException(const std::string &message = "Library exception")
        : std::runtime_error(message) {
    }
};

struct bookNotFoundException : libraryException {
    bookNotFoundException() : libraryException("Book is not found") {
    }
};

struct Library {  // throws sql::SQLException& and libraryException(see above)
    // TODO: The possibility of interrupting the connection is not handled
    // TODO: Adding of large textes isn't supposed to work good. See TODO below
private:
    dbManager &manager;
    std::unique_ptr<sql::Statement> stmt;
    std::string tableName = "library";
    int freeId = 0;

public:
    Library(dbManager &m);
    int addBook(const std::string &bookName,
                const std::string &author,
                const std::string &text);

    bool deleteBookById(int id);

    Book getBookById(int id); // TODO: this method should be in class of Collection
};

#endif
