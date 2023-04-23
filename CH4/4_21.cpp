//
// Created by Zhenqi Song on 2023/4/19.
// 处理用户登录的函数，实现真正彻底的异步操作


#include <experimental/future>
#include <string>

std::experimetal::future<void> process_login(
        std::string const& username, std::string const& password
        ){
    return backend.async_authenticate_user(usernamem password
    ).then([](std::experimental::future<user_id> id){
        return backend.async_request_current_info(id.get());
    }).then([](std::experimental::future<user_data> info_to_display){
        try{
            update_display(info_to_display.get());
        }catch(std::exception& e){
            display_error(e);
        }
    })
}


int main(){

}





