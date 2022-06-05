#ifndef YAFR_BOOKREP_H
#define YAFR_BOOKREP_H

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

struct BookRep {
private:
    DBManager &manager_;
    std::string tableName_ = "collection";
    std::filesystem::path pathToBooks_;

public:
    BookRep(DBManager &m);

    std::vector<Book> getAllBooks();

    void addBook(int id,
                 const std::string &bookName,
                 const std::string &author,
                 const std::string &filename);

    void addAndSaveBook(int id,
                        const std::string &bookName,
                        const std::string &author,
                        const std::string &text);

    void deleteBookById(int id);

    Book getBookById(int id);

    void clear();
};

#endif  // YAFR_BOOKREP_H
