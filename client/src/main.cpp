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
#include "include/serverProblemsException.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    std::string folderName = "yafr_files";
    std::filesystem::create_directories(folderName);
    std::filesystem::path appFolder = std::filesystem::absolute("./yafr_files");


    DBManager dbManager1;
    DBManager dbManager2;
    DBManager dbManager3;
    DBManager dbManager4;
    BookRep bookRep(dbManager1);
    WordRep wordRep(dbManager2);
    WordSetRep wordSetRep(dbManager3);
    WordSetContentRep wordSetContentRep(dbManager4);
    User user(&wordRep, &wordSetRep, &wordSetContentRep, &bookRep);

    ConnectingWindow connectingWindow;
    connectingWindow.setUser(&user);

    // The order of things below is important. readNowWindow tries to connect to
    // dictionaryWindow, so it must be initialized by that time
    while (true) {
        try {
            auto authorizationWindow =
                new AuthorizationWindow(&connectingWindow);
            connectingWindow.windowIndexes.auth =
                connectingWindow.allWindows.addWidget(authorizationWindow);

            auto libraryWindow = new LibraryWindow(&connectingWindow);
            connectingWindow.windowIndexes.library =
                connectingWindow.allWindows.addWidget(libraryWindow);

            auto dictionaryWindow = new DictionaryWindow(
                &wordRep, &wordSetRep, &wordSetContentRep, &connectingWindow);
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

            QPalette pal = a.palette();
            pal.setColor(QPalette::Window, Qt::white);
            a.setPalette(pal);

            a.setStyleSheet("QWidget{font-size:30px;}");

            connectingWindow.show();
            user.startRequestThread();

            return a.exec();
        } catch (ServerProblemsExceptionAbort &e) {
            return 0;
        } catch (ServerProblemsExceptionReconnect &e) {
            continue;
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
}
