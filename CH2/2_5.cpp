//
// Created by Zhenqi Song on 2023/3/28.
// 函数可以便捷的向外部转移线程的归属权

#include <thread>

std::thread f(){
    void some_function();
    return std::thread(some_function); // 将线程作为返回值，可以将线程的归属权转移到线程之外
}

std::thread g(){
    void some_other_function(int);
    std::thread t(some_other_function, 42);
    return t;
}