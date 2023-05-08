//
// Created by Zhenqi Song on 2023/5/8.
// 运用获取-释放次序传递同步

#include <atomic>
#include <iostream>
#include <thread>
#include <assert.h>

std::atomic<int> data[5];
std::atomic<bool> sync1(false), sync2(false);

void thread_1(){
    data[0].store(42, std::memory_order_relaxed);
    data[1].store(97, std::memory_order_relaxed);
    data[2].store(17, std::memory_order_relaxed);
    data[3].store(-141, std::memory_order_relaxed);
    data[4].store(2003, std::memory_order_relaxed);
    sync1.store(true, std::memory_order_release); // 设置sync1成立
}

void thread_2(){
    while (!sync1.load(std::memory_order_acquire)); // 循环知道sync1成立
    sync2.store(true, std::memory_order_release); // 设置sync2成立
}

void thread_3(){
    while (!sync2.load(std::memory_order_acquire)); // 循环直到sync2成立
    assert(data[0].load(std::memory_order_relaxed) == 42);
    assert(data[1].load(std::memory_order_relaxed) == 97);
    assert(data[2].load(std::memory_order_relaxed) == 17);
    assert(data[3].load(std::memory_order_relaxed) == -141);
    assert(data[4].load(std::memory_order_relaxed) == 2003);
}


int main(){
    std::thread thread1(thread_1);
    std::thread thread2(thread_2);
    std::thread thread3(thread_3);

    thread1.join();
    thread2.join();
    thread3.join();
}