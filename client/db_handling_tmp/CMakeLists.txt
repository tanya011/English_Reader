cmake_minimum_required(VERSION 3.21)
project(db_handling_tmp)

set(CMAKE_CXX_STANDARD 17)

include_directories(${MYSQL_CONCPP_DIR}/include)
link_directories(${MYSQL_CONCPP_DIR}/lib64)

add_executable(db_handling_tmp library_usage_examples.cpp library.cpp)

target_link_libraries(db_handling_tmp mysqlcppconn)


