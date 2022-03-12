#ifndef YAFR_BOOK_H
#define YAFR_BOOK_H
#include <string>
#include <utility>

struct BookPreview {
private:
    int id;
    std::string name;
    std::string author;

public:
    BookPreview(int id, std::string name, std::string author)
        : id(id), name(std::move(name)), author(std::move(author)) {
    }

    [[nodiscard]] int getId() const {
        return id;
    }
    std::string getName() {
        return name;
    }
    std::string getAuthor() {
        return author;
    }
};

struct Book {
private:
    int id;
    std::string name;
    std::string author;
    std::string text;

public:
    Book(int id, std::string name, std::string author, std::string text)
        : id(id),
          name(std::move(name)),
          author(std::move(author)),
          text(std::move(text)) {
    }
    Book(BookPreview preview, std::string text)
        : id(preview.getId()),
          name(std::move(preview.getName())),
          author(std::move(preview.getAuthor())),
          text(std::move(text)) {
    }

    [[nodiscard]] int getId() const {
        return id;
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
