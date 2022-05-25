#ifndef YAFR_BOOKREP_H
#define YAFR_BOOKREP_H

// DB includes
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <mysql_connection.h>
#include <filesystem>
#include <mutex>
#include "../include/book.h"
#include "dbManager.h"

struct BookRepException : std::runtime_error {
    explicit BookRepException(const std::string &message = "BookRep exception");
};

struct bookNotFoundException : BookRepException {
    bookNotFoundException();
};

struct BookRep {  // throws sql::SQLException& and BookRepException(see above)
    // TODO: The possibility of interrupting the connection is not handled
private:
    DBManager &manager_;
    std::string tableName_ = "collection";
    std::filesystem::path appFolder_;

public:
    BookRep(DBManager &m, std::filesystem::path appFolder);

    std::vector<Book> getAllBooks();

    void addBook(int id, const std::string &bookName,
                const std::string &author,
                const std::string &filename);

    void addAndSaveBook(int id, const std::string &bookName,
                const std::string &author,
                const std::string &text);

    void deleteBookById(int id);

    Book getBookById(int id);
};

#endif  // YAFR_LIBRARY_REP_H
