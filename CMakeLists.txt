cmake_minimum_required(VERSION 3.15)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code src_MERGE/main.cpp src_MERGE/BPlusTree.h
        src_MERGE/BPlusTreeString.h src_MERGE/MemoryPool.h
        src_MERGE/BookStoreHeader.h src_MERGE/BookStore.cpp
        src_MERGE/CommandManager.cpp src_MERGE/MyException.h
        src_MERGE/Administrator.cpp src_MERGE/Administrator.h
        src_MERGE/BPlusTree_BookStore.h
        src_MERGE/UnrolledLinkedList.cpp src_MERGE/UnrolledLinkedList.h)
