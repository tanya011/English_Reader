cmake_minimum_required(VERSION 3.5)

project(YAFR VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)


include_directories(${MYSQL_CONCPP_DIR}/include)
link_directories(${MYSQL_CONCPP_DIR}/lib64)

set(PROJECT_SOURCES
        src/playing_server.cpp
        src/userRep.cpp
        )

add_executable(YAFR
        ${PROJECT_SOURCES}
        )

target_link_libraries(YAFR PRIVATE mysqlcppconn)


target_link_libraries(YAFR PRIVATE ${CURL_LIBRARIES})

target_link_libraries(YAFR PRIVATE Threads::Threads)

