#ifndef YAFR_BOOKREP_H
#define YAFR_BOOKREP_H

#include <mutex>
#include "../include/book.h"
#include "dbManager.h"

struct BookRepException : std::runtime_error {
    explicit BookRepException(const std::string &message = "BookRep exception");
};

struct bookNotFoundException : BookRepException {
    bookNotFoundException();
};

struct BookRep {
private:
    DBManager &manager_;
    std::string tableName_ = "library";
    int freeId_ = 0;
    std::mutex *mutex_;

public:
    explicit BookRep(DBManager &m, std::mutex *mutex);

    std::vector<Book> getAllBooks();

    int addBook(const std::string &bookName,
                const std::string &author,
                const std::string &filename);

    bool deleteBookById(int id);

    Book getBookById(int id);
};

#endif  // YAFR_LIBRARY_REP_H
