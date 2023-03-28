//
// Created by Zhenqi Song on 2023/3/28.
// 分离线程以处理新文件

#include <thread>

void edit_document(std::string const& filename){
    open_document_and_display_gui(filename);

    while (!done_editing()){
        user_command cmd = get_user_input();
        if (cmd.type == open_new_document){
            // 如果命令是打开一个新的文件，则创建一个新的线程来处理文件
            std::string const new_name = get_filename_from_user();
            // 创建并分离线程，新文件的处理与当前线程无关
            std::thread t(edit_document, new_name);
            t.detach();
        }else{
            process_user_input(cmd);
        }
    }
}
