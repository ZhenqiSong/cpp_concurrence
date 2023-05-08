//
// Created by Zhenqi Song on 2023/5/8.
// 获取-释放操作不会构成单一的全局总操作序列

#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x, y;
std::atomic<int> z;


void write_x(){
    x.store(true, std::memory_order_release);
}

void write_y(){
    y.store(true, std::memory_order_release);
}

void read_x_then_y(){
    while (!x.load(std::memory_order_acquire));
    if (y.load(std::memory_order_acquire)) // 1
        ++z;
}

void read_y_then_x(){
    while (!y.load(std::memory_order_acquire));
    if (x.load(std::memory_order_acquire)) // 2
        ++z;
}

int main(){
    x = false;
    y = false;
    z = 0;
    std::thread a(write_x);
    std::thread b(write_y);
    std::thread c(read_x_then_y);
    std::thread d(read_y_then_x);
    a.join();
    b.join();
    c.join();
    d.join();
    assert(z.load() != 0); // 1, 2 都有可能false，从而断言失败
}
