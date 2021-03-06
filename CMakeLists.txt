cmake_minimum_required(VERSION 3.15)
project(code)
set(CMAKE_CXX_STANDARD 14)


add_executable(code src_BPT/main.cpp src_BPT/BPlusTree.h
        src_BPT/MemoryPool.h
        src_BPT/BookStoreHeader.h src_BPT/BookStore.cpp
        src_BPT/CommandManager.cpp src_BPT/MyException.h
        src_BPT/Administrator.cpp src_BPT/Administrator.h
        src_BPT/HashBPlusTree.h src_BPT/TokenScanner.h)
