//
// Created by Zhenqi Song on 2023/5/8.
// 通过原子操作从共享队列容器读取值

#include <atomic>
#include <thread>
#include <vector>

std::vector<int> queue_data;
std::atomic<int> count;

void populate_queue(){
    unsigned const number_of_items = 20;
    queue_data.clear();
    for (unsigned i = 0; i < number_of_items; ++i){
        queue_data.push_back(i);
    }
    count.store(number_of_items, std::memory_order_release); // 最初的存储操作
}

void consume_queue_items(){
    while (true){
        int item_index;
        // 一项 读-改-写 操作
        // 让其他线程知悉已有数据可用，一旦计数器的值为0，则无数据可用，则一直等待
        if ((item_index = count.fetch_sub(1, std::memory_order_acquire)) <= 0){
            wait_for_more_items(); // 等待队列容器装入新数据项
            continue;
        }
        process(queue_data[item_index - 1]); // 从内部容器queue_data，读取数据项是安全行为
    }
}

int main(){
    std::thread a(populate_queue);
    std::thread b(consume_queue_items);
    std::thread c(consume_queue_items);

    a.join();
    b.join();
    c.join();
}