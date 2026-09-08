cmake_minimum_required(VERSION 3.30)
project(password_manager LANGUAGES CXX)

find_package(SQLite3 REQUIRED)

add_subdirectory(src)

