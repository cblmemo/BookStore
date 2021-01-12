cmake_minimum_required(VERSION 3.17)
project(BookStore)

set(CMAKE_CXX_STANDARD 14)

add_executable(BookStore main.cpp
        src/UnrolledLinkedList.cpp src/UnrolledLinkedList.h
        src/BookManager.cpp src/BookManager.h
        src/UserManager.cpp src/UserManager.h
        src/CommandManager.cpp src/CommandManager.h
        src/FileManager.cpp src/FileManager.h
        src/BookStore.cpp src/BookStore.h
        src/Administrator.cpp src/Administrator.h
        src/Exception.cpp src/Exception.h)