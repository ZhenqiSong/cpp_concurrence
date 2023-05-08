//
// Created by Zhenqi Song on 2023/5/8.
// 宽松原子操作几乎不要求服从任何次序

#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;


void write_x_then_y(){
//    y.store(true, std::memory_order_relaxed);
//    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    x.store(true, std::memory_order_relaxed);
    y.store(true, std::memory_order_relaxed);
}

void read_y_then_x(){
    while (!y.load(std::memory_order_relaxed));
    if (x.load(std::memory_order_relaxed))
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
    assert(z.load() != 0); // 书上说，会发生断言错误，但测试中并不会，除非按照注释的情况
}