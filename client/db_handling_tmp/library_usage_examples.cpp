#include "library.h"
#include <iostream>

using namespace std;
int main() {
    try {
        dbManager d;  // Connecting to server and creating db "yaft" if it isn't
                      // exists yet
        Library library(d);

        int id1 = library.addBook("Book1", "Author1", "Text of book1");
        int id2 = library.addBook("Book2", "Author2", "Text of book2");
        cout << "Ids of added books: " << id1 << ", " << id2 << endl;

        try {
            Book b1 = library.getBookById(id1);
            cout << "first book:    " << b1.getName() << ", " << b1.getAuthor()
                 << ", " << b1.getText() << endl;
            Book b2 = library.getBookById(id2);
            cout << "second book:   " << b2.getName() << ", " << b2.getAuthor()
                 << ", " << b2.getText() << endl;
            cout << "third book:  ";
            Book b3 = library.getBookById(100500);  // Doesn't exist
        } catch (exception &e) {
            cout << e.what() << endl;
        }

        if (library.deleteBookById(id1)) {
            cout << "Book with id1 deleted" << endl;
        }
        if (library.deleteBookById(100500)) {
            cout << "Book with id=100500 is deleted, though is didn't existed"
                 << endl;
        }
    } catch (exception &e) {
        cout << e.what() << endl;
    }
}