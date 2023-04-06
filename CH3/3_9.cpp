//
// Created by Zhenqi Song on 2023/4/3.
// 运用std::lock()函数和std::unique_lock<>类模板在对象间互换内部数据

#include <mutex>

class some_big_object;

void swap(some_big_object& lhs, some_big_object& rhs);

class X{
private:
    some_big_object some_detail;
    std::mutex m;

public:
    X(some_big_object const& sd): some_detail(sd){}

    friend void swap(X& lhs, X& rhs){
        if (&lhs == &rhs)
            return;

        // 使用unique_lock会栈用更大的内存，相比于lock_guard有更大的损失
        // 第二个参数 std::adopt_lock 指明unique_lock对象管理互斥上的锁
        // std::defer_lock 使互斥在完成构造时处于无锁状态，等以后有需要时才调用初始加锁
        std::unique_lock<std::mutex> lock_a(lhs.m, std::defer_lock);
        std::unique_lock<std::mutex> lock_b(rhs.m, std::defer_lock);

        std::lock(lock_a, lock_b); // 这里才对互斥加锁

        swap(lhs.some_detail, rhs.some_detail);
    }
};