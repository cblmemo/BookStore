cmake_minimum_required(VERSION 3.15)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code src/main.cpp
        src/UnrolledLinkedList.cpp src/UnrolledLinkedList.h
        src/BookStoreHeader.h src/BookStore.cpp
        src/CommandManager.cpp src/MyException.h
        src/Administrator.cpp src/Administrator.h)
