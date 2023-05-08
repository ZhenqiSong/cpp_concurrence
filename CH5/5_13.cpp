//
// Created by Zhenqi Song on 2023/5/8.
// 向非原子操作强制施行内存次序

#include <atomic>
#include <thread>
#include <assert.h>

bool x = false; // x是普通的非原子变量
std::atomic<bool> y;
std::atomic<int> z;

void write_x_then_y(){
    x = true; // 变量x的存储操作位于栅栏前面
    std::atomic_thread_fence(std::memory_order_release);
    y.store(true, std::memory_order_relaxed); // 变量y的存储操作位于栅栏后面
}

void read_y_then_x(){
    while (!y.load(std::memory_order_relaxed)); // 一直循环等待，直到看到2处写出true值才停止
    std::atomic_thread_fence(std::memory_order_acquire);
    if (x) // 这里读取1处写出的值
        ++z;
}

int main(){
    x = false;
    y = false;
    z = 0;

    std::thread a(write_x_then_y);
    std::thread b(read_y_then_x);
    a.join();
    b.join();
    assert(z.load() != 0); // 此断言不会触发
}
