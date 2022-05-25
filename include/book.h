#ifndef YAFR_BOOK_H
#define YAFR_BOOK_H
#include <fstream>
#include <sstream>
#include <string>
#include <utility>

struct Book {
private:
    int id;
    std::string name;
    std::string author;
    std::string filename = "";

public:
    Book(int id, std::string name, std::string author, std::string filename)
        : id(id),
          name(std::move(name)),
          author(std::move(author)),
          filename(std::move(filename)) {
    }
    Book(int id, std::string name, std::string author)
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

    std::string getText() {
        std::ifstream file(filename);
        if (!file.good())
            return "File Not found";
        std::stringstream buf;
        buf << file.rdbuf();
        std::string text(buf.str());
        file.close();
        return text;
    }
};

#endif
