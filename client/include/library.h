#ifndef YAFT_LIBRARY_H
#define YAFT_LIBRARY_H

#include <utility>
#include <vector>
#include "book.h"

struct Library {
private:
    std::vector<BookPreview> books;

public:
    explicit Library(std::vector<BookPreview> books)
            : books(std::move(books)) {
    }

    std::vector<BookPreview>& getBooks(){
        return books;
    }

};

#endif