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
#include "include/periodic_function.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    std::string folderName = "yafr_files";
    std::filesystem::create_directories(folderName);
    std::filesystem::path appFolder = std::filesystem::absolute("./yafr_files");


    CallBackTimer timer;
    DBManager dbManager1;
    DBManager dbManager2;
    DBManager dbManager3;
    DBManager dbManager4;
    BookRep bookRep(dbManager1, appFolder);
    WordRep wordRep(dbManager2);
    WordSetRep wordSetRep(dbManager3);
    WordSetContentRep wordSetContentRep(dbManager4);
    User user(&wordRep, &wordSetRep, &wordSetContentRep, &bookRep);

    ConnectingWindow connectingWindow;
    connectingWindow.setUser(&user);

    /* bookRep.addBook(1, "The Beatles", "Paul Shipton",
                     "/home/tatyana/Programming/Проект Весна 2022/English_Reader/client/src/books/Beatles.txt");*/

    // The order of things below is important. readNowWindow tries to connect to
    // dictionaryWindow, so it must be initialized by that time
    while (true) {
        try {
            auto authorizationWindow = new AuthorizationWindow(&connectingWindow);
            connectingWindow.windowIndexes.auth =
                    connectingWindow.allWindows.addWidget(authorizationWindow);

            auto libraryWindow = new LibraryWindow(&connectingWindow);
            connectingWindow.windowIndexes.library =
                    connectingWindow.allWindows.addWidget(libraryWindow);

            auto dictionaryWindow = new DictionaryWindow(&wordRep, &wordSetRep, &wordSetContentRep, &connectingWindow);
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
            QPalette pal = a.palette();
            pal.setColor(QPalette::Window, Qt::white);
            a.setPalette(pal);

            a.setStyleSheet("QWidget{font-size:30px;}");

            connectingWindow.show();

            timer.start(100000, [&](){
               if (user.isAuthorized()){
                   user.updateDictionaryChanges();
                   std::cout << "send request" << std::endl;
               }
            });

            return a.exec();
        } catch (ServerProblemsExceptionAbort &e) {
            return 0;
        } catch (ServerProblemsExceptionReconnect &e){
            continue;
        }
        catch (std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
}
