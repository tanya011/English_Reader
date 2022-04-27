#define __GLIBCXX_DEBUG
#include <QApplication>
#include <sstream>
#include "include/authorizationWindow.h"
#include "include/connectingWindow.h"
#include "include/db_manager.h"
#include "include/learnWindow.h"
#include "include/libraryWindow.h"
#include "include/readNowWindow.h"

/* мы не знаем что это такое если бы мы знали что это такое
void load_books(DBManager& m){
    std::string path="/home/tatyana/Programming/Проект Весна
2022/English_Reader/client/src/"; std::ifstream f(path+"booknames.txt");
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
 */

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    DBManager dbManager;
    ConnectingWindow connectingWindow;

    // The order of things below is important. readNowWindow tries to connect to
    // dictionaryWindow, so it must exist

    auto dictionaryWindow = new DictionaryWindow(&connectingWindow);
    connectingWindow.windowIndexes.dictionary =
        connectingWindow.allWindows.addWidget(dictionaryWindow);

    auto readNowWindow = new ReadNowWindow(&connectingWindow);
    connectingWindow.windowIndexes.readNow =
        connectingWindow.allWindows.addWidget(readNowWindow);

    auto  libraryWindow = new LibraryWindow(&connectingWindow, dbManager);
    connectingWindow.windowIndexes.library =
        connectingWindow.allWindows.addWidget(libraryWindow);

    auto learnWindow = new LearnWindow(&connectingWindow);
    connectingWindow.windowIndexes.learn =
        connectingWindow.allWindows.addWidget(learnWindow);

    auto  authorizationWindow = new AuthorizationWindow(&connectingWindow);
    connectingWindow.windowIndexes.auth =
        connectingWindow.allWindows.addWidget(authorizationWindow);

    connectingWindow.allWindows.setCurrentIndex(
        connectingWindow.windowIndexes.readNow);

    //      Notes for curious people
    // Note 1: Though all windows had connectingWindow as parent first, they now
    // have StackedWidget as parent and so don't appear all at the same time.

    // Note 2: Windows have member 'parent', but it isn't true Qt parent, it's
    // connectionWindow.

    // Note 3: We can change parent of widget, as they all inherit QWidget, and
    // it's parent is QWidget
    connectingWindow.showMaximized();

    return a.exec();
}
