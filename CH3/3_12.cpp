//
// Created by Zhenqi Song on 2023/4/6.
// 利用std::call_once()函数对类X的数据成员实施线程安全的延迟初始化

#include <mutex>

class X{
private:
    conection_info connection_details;
    conection_handle connection;

    std::once_flag connection_init_flag;
    void open_connection(){
        connection = connection_manager.open(connection_details);
    }

public:
    X(connection_info const& connection_details_):connection_details(connection_details_){}

    void send_data(data_packet const& data){
        // 在这里或者receive_data中进行首次调用，需要将this作为第一个参数
        std::call_once(connection_init_flag, &X::open_connection, this);
        connection.send_data(data);
    }

    data_packet receive_data(){
        std::call_once(connection_init_flag, &X::open_connection, this);
        return connection.receive_data();
    }
};