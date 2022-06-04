cmake_minimum_required(VERSION 3.5)

project(YAFR VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(nlohmann_json 3.2.0 REQUIRED)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})

pkg_search_module(CRYPTO libcrypto REQUIRED)
include_directories(${CRYPTO_INCLUDE_DIRS})

include_directories(${MYSQL_CONCPP_DIR}/include)
link_directories(${MYSQL_CONCPP_DIR}/lib64)

set(PROJECT_SOURCES
        src/playing_server.cpp
        src/userRep.cpp
        src/bookRep.cpp
        src/collectionsRep.cpp
        src/wordRep.cpp
        src/wordSetRep.cpp
        src/wordSetContentRep.cpp
        include/collectionsHistoryRep.h
        include/actCollectionsHistory.h)

add_executable(YAFR
        ${PROJECT_SOURCES}
        )

target_link_libraries(YAFR PRIVATE mysqlcppconn)


target_link_libraries(YAFR PRIVATE ${CURL_LIBRARIES})

target_link_libraries(YAFR PRIVATE Threads::Threads)

target_link_libraries(YAFR PRIVATE nlohmann_json::nlohmann_json)

target_link_libraries(YAFR PRIVATE OpenSSL::Crypto)
target_link_libraries(YAFR PRIVATE OpenSSL::SSL)

target_link_libraries(YAFR PRIVATE cryptopp)

add_compile_definitions(CONFIG_PATH="../config.json")


