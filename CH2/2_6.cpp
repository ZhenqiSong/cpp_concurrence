//
// Created by Zhenqi Song on 2023/3/28.
//

#include <thread>

class scoped_thread{
    std::thread t;

public:
    // 移动线程所有权给t
    explicit scoped_thread(std::thread t_): t(std::move(t_)){
        // 在构造函数中判断t是否还能汇合，在2.3中是在析构函数中
        if (!t.joinable())
            throw std::logic_error("No thread");
    }

    ~scoped_thread(){
        t.join();
    }

    scoped_thread(scoped_thread const&) = delete;
    scoped_thread operator=(scoped_thread const&) = delete;
};

struct func;

void f(){
    int some_local_state;

    // 传入构造函数的是一个右值
    scoped_thread t{std::thread(func(some_local_state))};

    do_something_in_current_thread();
    // 函数模型，会销毁t，与之关联的线程就会join
}