cmake_minimum_required(VERSION 3.15)
project(code)
set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")

add_executable(code src_BPT/main.cpp src_BPT/BPlusTree.h
        src_BPT/BPlusTreeString.h src_BPT/MemoryPool.h
        src_BPT/BookStoreHeader.h src_BPT/BookStore.cpp
        src_BPT/CommandManager.cpp src_BPT/MyException.h
        src_BPT/Administrator.cpp src_BPT/Administrator.h
        src_BPT/BPlusTree_BookStore.h)
