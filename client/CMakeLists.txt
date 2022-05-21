cmake_minimum_required(VERSION 3.5)

project(YAFR VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

find_package(CURL REQUIRED)

pkg_check_modules(CURL libcurl REQUIRED)

include_directories(${MYSQL_CONCPP_DIR}/include)
link_directories(${MYSQL_CONCPP_DIR}/lib64)

include_directories(${CURL_INCLUDE_DIR})

set(PROJECT_SOURCES
        src/main.cpp
        src/word.cpp
        src/wordset.cpp
        src/cardsDisplay.cpp
        src/connectingWindow.cpp
        src/dictionary.cpp
        src/readNowWindow.cpp
        src/dictionaryWindow.cpp
        src/book_rep.cpp
        src/libraryWindow.cpp
        src/translator.cpp
        src/learnWindow.cpp

        include/book.h
        #include/book_rep.h
        #include/db_manager.h
        include/readNowWindow.h
        include/libraryWindow.h

        include/dictionary.h
        include/dictionaryWindow.h
        include/connectingWindow.h
        include/word.h
        include/wordset.h
        include/learnWindow.h
        include/authorizationWindow.h
        include/library.h
        )


if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(YAFR
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET client APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else ()
    if (ANDROID)
        add_library(YAFR SHARED
                ${PROJECT_SOURCES}
                src/readNowWindow.cpp
                include/readNowWindow.h
                )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(YAFR
                ${PROJECT_SOURCES}
                )
    endif ()
endif ()

target_link_libraries(YAFR PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(YAFR PRIVATE mysqlcppconn)
set_target_properties(YAFR PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)


target_link_libraries(YAFR PRIVATE ${CURL_LIBRARIES})

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(YAFR)
endif ()
