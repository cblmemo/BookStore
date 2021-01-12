cmake_minimum_required(VERSION 3.17)
project(BookStore)

set(CMAKE_CXX_STANDARD 14)

add_executable(BookStore main.cpp
        UnrolledLinkedList.h UnrolledLinkedList.cpp
        BookManager.h BookManager.cpp)