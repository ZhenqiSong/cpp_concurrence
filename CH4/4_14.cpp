//
// Created by Zhenqi Song on 2023/4/13.
// spawn_task()的简单实现

#include <future>
#include <type_traits>

template<typename F, typename A>
std::future<typename std::result_of<F(A&&)>::type> spawn_task(F&& f, A&& a){
    typedef typename std::result_of<F(A&&)>::type result_type;

    std::packaged_task<result_type(A&&)> task(std::move(f));
    std::future<result_type> res(task.get_future());
    std::thread t(std::move(task), std::move(a));
    t.detach();
    return res;
}