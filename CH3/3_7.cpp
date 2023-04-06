//
// Created by Zhenqi Song on 2023/3/31.
// 使用层级锁防范死锁

#include <mutex>

// 3个不同等级的锁，如果已在某互斥上加了锁，只能由相对低等级的互斥取锁，从而限制代码的行为
hierarchical_mutex high_level_mutex(10000);
hierarchical_mutex low_level_mutex(5000);
hierarchical_mutex other_mutex(6000);


// 该函数位于最低等级
int do_low_level_stuff();
int low_level_func(){
    std::lock_guard<hierarchical_mutex> lk(low_level_mutex);
    return do_low_level_stuff();
}

void high_level_stuff(int some_param);
void high_level_func(){
    // 高等级的函数，先锁住了高级锁再调用低等级的函数
    std::lock_guard<hierarchical_mutex> lk(high_level_mutex);
    high_level_stuff(low_level_func());
}

void thread_a(){
    high_level_func();
}

void do_other_stuff();
void other_stuff(){
    high_level_func();
    do_other_stuff();
}

void thread_b(){
    // 先对中等级的加锁，再尝试对高等级的加锁，违反了层级规则，会报错
    std::lock_guard<hierarchical_mutex> lk(other_mutex);
    other_stuff();
}