#include <QApplication>
#include <filesystem>
#include <sstream>
#include "include/authorizationWindow.h"
#include "include/collectionWindow.h"
#include "include/connectingWindow.h"
#include "include/dbManager.h"
#include "include/learnWindow.h"
#include "include/libraryWindow.h"
#include "include/readNowWindow.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    std::string folderName="yafr_files";
    std::filesystem::create_directories(folderName);
    std::filesystem::path appFolder=std::filesystem::absolute("./yafr_files");

    DBManager dbManager;
    BookRep bookRep(dbManager, appFolder);

    User user(&bookRep);

    ConnectingWindow connectingWindow;
    connectingWindow.setUser(&user);

   /* bookRep.addBook(1, "The Beatles", "Paul Shipton",
                    "/home/tatyana/Programming/Проект Весна 2022/English_Reader/client/src/books/Beatles.txt");*/

    // The order of things below is important. readNowWindow tries to connect to
    // dictionaryWindow, so it must be initialized by that time
    try {
        auto authorizationWindow = new AuthorizationWindow(&connectingWindow);
        connectingWindow.windowIndexes.auth =
            connectingWindow.allWindows.addWidget(authorizationWindow);

        auto libraryWindow = new LibraryWindow(&connectingWindow);
        connectingWindow.windowIndexes.library =
            connectingWindow.allWindows.addWidget(libraryWindow);

        auto dictionaryWindow = new DictionaryWindow(&connectingWindow);
        connectingWindow.windowIndexes.dictionary =
            connectingWindow.allWindows.addWidget(dictionaryWindow);

        auto readNowWindow = new ReadNowWindow(&connectingWindow);
        connectingWindow.windowIndexes.readNow =
            connectingWindow.allWindows.addWidget(readNowWindow);

        auto collectionWindow =
            new CollectionWindow(&connectingWindow, &bookRep);
        connectingWindow.windowIndexes.collection =
            connectingWindow.allWindows.addWidget(collectionWindow);

        auto learnWindow = new LearnWindow(&connectingWindow);
        connectingWindow.windowIndexes.learn =
            connectingWindow.allWindows.addWidget(learnWindow);

        connectingWindow.allWindows.setCurrentIndex(
            connectingWindow.windowIndexes.auth);

        //      Notes for curious people
        // Note 1: Though all windows had connectingWindow as parent first, they
        // now have StackedWidget as parent and so don't appear all at the same
        // time.

        // Note 2: Windows have member 'parent', but it isn't true Qt parent,
        // it's connectionWindow.

        // Note 3: We can change parent of widget, as they all inherit QWidget,
        // and it's parent is QWidget

        connectingWindow.show();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return a.exec();
}
