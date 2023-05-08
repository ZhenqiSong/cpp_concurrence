//
// Created by Zhenqi Song on 2023/5/8.
// 栅栏可以令宽松操作服从一定的次序

#include <atomic>
#include <thread>
#include <assert.h>

std::atomic<bool> x,y;
std::atomic<int> z;

void write_x_then_y(){
    x.store(true, std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_release); // 加入释放栅栏
    y.store(true, std::memory_order_relaxed); // 3
}

void read_y_then_x(){
    while (!y.load(std::memory_order_relaxed)); // 需要读取3存储的值
    std::atomic_thread_fence(std::memory_order_acquire); // 加入获取栅栏，两个栅栏配合确保了x的写在载入操作之前
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
    assert(z.load() != 0);
}