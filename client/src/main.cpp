#include <QApplication>
#include <sstream>
#include "include/sectionbase.h"
#include "include/db_manager.h"

void load_books(DBManager& m){
    std::string path="/home/ekaterina/project-yafr/English_Reader/client/src/";
    std::ifstream f(path+"booknames.txt");
    std::vector<std::string> names;
    std::string t;
    while(std::getline(f, t)){
        names.push_back(path+"books/"+t);
    }
    BookRep rep(m);
    for(auto n: names){
        std::ifstream f(n);
        std::string name;
        std::getline(f, name);
        std::string author;
        std::getline(f, author);
        std::stringstream text;
        text<<f.rdbuf();
        int id = rep.addBook(name, author, text.str());
        std::cout<< id<<", ";
    }
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    DBManager dbManager;
    load_books(dbManager);

    SectionBase mainWindow(dbManager);
    mainWindow.showMaximized();
    mainWindow.show();


    return a.exec();
}
