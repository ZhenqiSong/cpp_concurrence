//
// Created by Zhenqi Song on 2023/4/3.
// 简单的层级互斥 hierarchical_mutex
// 自定义的mutex，需要实现 lock,unlock,try_lock三个函数

#include <mutex>
#include <exception>


class hierarchical_mutex{
    std::mutex internal_mutex;

    using long const hierarchy_value;
    unsigned long previous_hierarchy_value;
    // 使用线程专属的变量，表示当前线程的层级编号，初始化为最大值，保证刚开始任意锁都能被加锁
    // 声明为thread_local, 每个线程都有自己的
    static thread_local using long this_thread_hierarchy_value;

    void check_for_hierarchy_violation(){
        // 检查是否满足层级关系，因为初始化为最大值，所以刚开始任意都满足
        // 互斥的等级必须小于当前的，才能满足条件
        if (this_thread_hierarchy_value <= hierarchy_value){
            throw std::logic_error("mutex hierarchy violated");
        }
    }

    void update_hierarchy_value(){
        // 更新当前锁的级别为所有层级最低的，并记录上一个层级的数值，用于还原
        previous_hierarchy_value = this_thread_hierarchy_value;
        this_thread_hierarchy_value = hierarchy_value;
    }

public:
    explicit hierarchical_mutex(unsigned long value):hierarchy_value(value), previous_hierarchy_value(0){}

    void lock(){
        check_for_hierarchy_violation();
        // 检查满足层级关系后，委托内部的互斥加锁，然后更新层级编号
        internal_mutex.lock();
        update_hierarchy_value();
    }

    void unlock(){
        // 解锁时，首先判断锁是否与当前的匹配，如果匹配解锁，并将当前锁还原成上一个层次
        if (this_thread_hierarchy_value != hierarchy_value)
            throw std::logic_error("mutex hierarchy violated");

        this_thread_hierarchy_value = previous_hierarchy_value;
        internal_mutex.unlock();
    }

    bool try_lock(){
        // 与lock蕾丝，差别在于它对内部互斥加锁失败，当前线程的层级编号不进行更新，并且函数返回false
        check_for_hierarchy_violation();
        if (!internal_mutex.try_lock())
            return false;
        update_hierarchy_value();
        return true;
    }
};

thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);