//
// Created by Zhenqi Song on 2023/4/19.
// 使用后续函数处理用户登录

#include <future>
#include <string>


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


std::future<void> process_login(std::string const& username,
                                std::string const& password){
    return spawn_async([=](){
        return backend.authenticate_user(username, password);
    }).then([](std::future<user_id> id){
        return backend.request_current_info(id.get());
    }).then([](std::future<user_data> info_to_display){
        try{
            update_display(info_to_display.get());
        }catch(std::exception){

        }
    });
}

