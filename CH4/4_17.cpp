//
// Created by Zhenqi Song on 2023/4/18.
// 与std::async()等价的函数，其中运用了并发技术规约中的std::experimental::future类

#include <future>

template<typename Func>
std::future<decltype(std::declval<Func>()())> spawn_async(Func&& func){
    std::promise<decltype(std::declval<Func>()())> p;

    auto res = p.get_future();
    std::thread t([p = std::move(p), f = std::decay_t<Func>(func)]() mutable {
        try{
            p.set_value_at_thread_exit(f()); // 函数的运行结果保存在future对象内部，若任务函数抛出异常，则会被捕捉
        } catch(...){
            p.set_exception_at_thread_exit(std::current_exception());
        }
    });
    t.detach();
    return res;
}
