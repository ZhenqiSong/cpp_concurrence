//
// Created by Zhenqi Song on 2023/4/19.
// 使用一个异步任务处理用户登录

#include <future>


// 直接使用std::async(), 全部代码就会集中在后台的某一个线程上运行，该后台线程仍会消耗大量资源以逐一完成各任务，而发生阻塞
// 若这样的任务很多，将导致大量线程无所事事
std::future<void> process_login(std::string const& username,
                                std::string const& password){
    return std::async(std::launch::async,
                      [=](){
        try{
            user_id const id = backend.authenticate_user(username, password);
            user_data const info_to_display = backend.request_current_info(id);
            update_display(info_to_display);
        }catch (std::exception& e){
            dispaly_error(e);
        }
    });
}
