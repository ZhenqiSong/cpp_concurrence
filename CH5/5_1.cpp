//
// Created by Zhenqi Song on 2023/5/4.
// 采用std::atomic_flag实现自旋锁互斥

// atomic_flag 是一种简单的标准原子类型，表示一个布尔标志。只有两种状态。

#include <atomic>


class spinlock_mutex{
    std::atomic_flag flag;

public:
    // 首先进行初始化，初始化为置零状态
    spinlock_mutex():flag(ATOMIC_FLAG_INIT){}

    void lock(){
        while (flag.test_and_set(std::memory_order_acquire));
    }

    void unlock(){
        flag.clear(std::memory_order_release); // 显式的使用采用释放语义将标志清零
    }
};