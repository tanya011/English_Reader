#include "book_rep.h"
#include <iostream>

using namespace std;
int main() {
    try {
        dbManager d;  // Connecting to server and creating db "yaft" if it isn't
                      // exists yet
        BookRep bookRep(d);

        int id1 = bookRep.addBook("Book1", "Author1", "Text of book1");
        int id2 = bookRep.addBook("Book2", "Author2", "Text of book2");
        cout << "Ids of added books: " << id1 << ", " << id2 << endl;

        try {
            std::vector<BookPreview> bookPrev(bookRep.getBookPreview());
            cout<<"Book previews:\n";
            for(auto b: bookPrev){
                cout<<b.getName()<<" "<<b.getAuthor()<<'\n';
            }
            Book b1 = bookRep.getBookById(id1);
            cout << "first book:    " << b1.getName() << ", " << b1.getAuthor()
                 << ", " << b1.getText() << endl;
            Book b2 = bookRep.getBookById(id2);
            cout << "second book:   " << b2.getName() << ", " << b2.getAuthor()
                 << ", " << b2.getText() << endl;
            cout << "third book:  ";
            Book b3 = bookRep.getBookById(100500);  // Doesn't exist
        } catch (exception &e) {
            cout << e.what() << endl;
        }

        if (bookRep.deleteBookById(id1)) {
            cout << "Book with id1 deleted" << endl;
        }
        if (bookRep.deleteBookById(100500)) {
            cout << "Book with id=100500 is deleted, though is didn't existed"
                 << endl;
        }
    } catch (exception &e) {
        cout << e.what() << endl;
    }
}