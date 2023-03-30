//
// Created by Zhenqi Song on 2023/3/30.
// 使用std::lock, lock_guard 避免死锁

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
        // 确保两个参数指向不同实例，因为重复获取锁会导致为定义行为
        if (&lhs == &rhs)
            return;

        // 锁定两个互斥，如果一个加锁失败，就全员失败，不会造成有的被锁，有的没有
        std::lock(lhs.m, rhs.m);
        // 使用adopt_lock告诉互斥已被锁住，避免试图另行加锁
        std::lock_guard<std::mutex> lock_a(lhs.m, std::adopt_lock);
        std::lock_guard<std::mutex> lock_b(rhs.m, std::adopt_lock);

        // c++17, 可以用这一个代替
//        std::scoped_lock guard(lhs.m, rhs.m);

        swap(lhs.some_detail, rhs.some_detail);
    }
};