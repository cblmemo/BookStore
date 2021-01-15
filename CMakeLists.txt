cmake_minimum_required(VERSION 3.17)
project(BookStore)

set(CMAKE_CXX_STANDARD 14)

add_executable(BookStore src/main.cpp
        src/UnrolledLinkedList.cpp src/UnrolledLinkedList.h
        src/BookStoreHeader.h src/BookStore.cpp
        src/CommandManager.cpp src/MyException.h src/Administrator.cpp src/Administrator.h)