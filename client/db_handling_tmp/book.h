#ifndef YAFT_BOOK_H
#define YAFT_BOOK_H

struct Book {
private:
    std::string name;
    std::string author;
    std::string text;

public:
    Book(std::string name, std::string author, std::string text)
        : name(std::move(name)),
          author(std::move(author)),
          text(std::move(text)) {
    }
    std::string getName() {
        return name;
    }
    std::string getAuthor() {
        return author;
    }
    std::string getText() {
        return text;
    }
};

#endif
