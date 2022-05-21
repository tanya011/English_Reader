#include "include/book_rep.h"


int BookRep::addBook(
    const std::string &bookName,
    const std::string &author,
    const std::string &text) {
    std::vector<BookPreview> bookPreviews;
    for(int i = 0; i < 20; i++){
        bookPreviews.emplace_back(i, &"Autor "[i], &"Title " [ i]);
    }
}

bool BookRep::deleteBookById(int id) {  // true if everything is ok

}

BookPreview BookRep::getBookById(int id) {
    return getBookPreview[id];
}


BookRepException::BookRepException(const std::string &message)
    : std::runtime_error(message) {
}
bookNotFoundException::bookNotFoundException()
    : BookRepException("Book is not found") {
}
