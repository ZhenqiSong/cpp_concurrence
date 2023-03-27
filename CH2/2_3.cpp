//
// Created by SongZhenqi on 2023/3/27.
// 通过RAII，确保线程join, 当f执行结束或发生异常，会销毁局部变量，这时候就会调用thread_guard的析构函数，确保join

#include <thread>

class thread_guard{
    std::thread& t;

public:
    explicit thread_guard(std::thread& t_): t(t_){}

    ~thread_guard(){
        if (t.joinable()){ // 判断对象是否能够汇合，因为join只能被执行一次
            t.join();
        }
    }

    thread_guard(thread_guard const&) = delete;
    thread& operator=(thread_guard const&) = delete;
};

struct func;

void f(){
    int some_local_state = 0;

    func my_func(some_local_state);
    std::thread t(my_func);
    thread_guard g(t);
    do_something_in_current_thread();
}