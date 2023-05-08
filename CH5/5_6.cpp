//
// Created by Zhenqi Song on 2023/5/8.
// 多个线程上的宽松原子操作

#include <thread>
#include <atomic>
#include <iostream>

// 5个线程共同操作3个全局原子变量，每个线程分别循环10次，每一次按照relaxed次序读取3个原子变量的值，并存入数组
std::atomic<int> x(0), y(0), z(0); // 1
std::atomic<bool> go(false); // 2

unsigned const loop_count = 10;

struct read_values{
    int x, y, z;
};

read_values values1[loop_count];
read_values values2[loop_count];
read_values values3[loop_count];
read_values values4[loop_count];
read_values values5[loop_count];

void increment(std::atomic<int>* var_to_inc, read_values* values){
    while (!go) // 自旋以等待信号
        std::this_thread::yield();

    for (unsigned i = 0; i < loop_count; ++i){
        values[i].x = x.load(std::memory_order_relaxed);
        values[i].y = y.load(std::memory_order_relaxed);
        values[i].z = z.load(std::memory_order_relaxed);
        var_to_inc->store(i + 1, std::memory_order_relaxed);
        std::this_thread::yield();
    }
}

void read_vals(read_values* values){
    while (!go)
        std::this_thread::yield();

    for (unsigned i = 0; i < loop_count; ++i){
        values[i].x = x.load(std::memory_order_relaxed);
        values[i].y = y.load(std::memory_order_relaxed);
        values[i].z = z.load(std::memory_order_relaxed);
        std::this_thread::yield();
    }
}

void print(read_values* v){
    for (unsigned i = 0; i < loop_count; ++i){
        if (i)
            std::cout << ",";
        std::cout << "(" << v[i].x << "," << v[i].y << "," << v[i].z << ")";
    }
    std::cout << std::endl;
}

int main(){
    std::thread t1(increment, &x, values1);
    std::thread t2(increment, &y, values2);
    std::thread t3(increment, &z, values3);
    std::thread t4(read_vals, values4);
    std::thread t5(read_vals, values5);

    go = true; // 只有所有线程准备就绪，go才为true
    t5.join();
    t4.join();
    t3.join();
    t2.join();
    t1.join();

    print(values1); // 第一个线程中x递增
    print(values2); // 第二个线程中y递增
    print(values3);
    print(values4);
    print(values5);
}

// 线程t3并没有看见变量x和y的任何更新，它仅仅看见自己对变量z的更新。
// 尽管如此，这并不妨碍其他线程看见变量z的更新，它们还一并看见变量x和y的更新。