//
// Created by Zhenqi Song on 2023/4/19.
// 处理用户登录的简单函数，其中的操作按串行方式顺次执行

// 使用串行的方式会造成主进程阻塞
void process_login(std::string const& username, std::string const& password){
    try{
        user_id const id = backend.authenticate_user(username, password);
        user_data const info_to_display = backend.request_current_info(id);
        update_display(info_to_display);
    } catch(std::expection& e){
        display_error(e);
    }
}
