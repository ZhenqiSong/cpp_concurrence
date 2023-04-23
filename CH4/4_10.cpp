//
// Created by Zhenqi Song on 2023/4/10.
// 利用多个promise在单个线程中处理多个连接
// https://blog.csdn.net/qq_23350817/article/details/116460138

#include <future>
#include <vector>

class connection{
public:
    bool has_incoming_data(){}
};
using connection_set = std::vector<connection>;


void process_connections(connection_set& connections){
    while(!done(connections)){
        // 依次检查每个连接
        for (auto connection = connections.begin(), end = connections.end();
        connection != end; ++connection){
            // 若有数据传入，则接收
            if (connection->has_incoming_data()){
                data_packet data = connection->incoming();
                std::promise<payload_type>& p = connection->get_promise(data.id); // 获取数据关联的promise

                p.set_value(data.payload); // 将其相关值设置为数据包的有效载荷
            }

            if (connection->has_outgoing_data()){
                outgoing_packet data = connection->top_of_outgoing_queue();
                connection->send(data.payload); // 发送队列中有数据，则向外发送
                data.promise.set_value(true); // 设置为true，表示发送成功
            }
        }
    }
}
