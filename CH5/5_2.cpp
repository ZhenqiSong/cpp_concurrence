//
// Created by Zhenqi Song on 2023/5/6.
// 在一份数据上进行非原子化的读

#include <vector>
#include <atomic>
#include <iostream>
#include <thread>
#include <chrono>

std::vector<int> data;
std::atomic<bool> data_ready(false);

void reader_thread(){
    while (!data_ready.load()){ // 等待数据备妥
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    std::cout << "The answer=" << data[0] << "\n"; // 在一份数据上进行非原子化的读
}

void writer_thread(){
    data.push_back(42); // 在一份数据上进行非原子化的写, 写操作必然在读操作之前发生，确保了次序
    data_ready = true;
}


int main(){
    std::thread t1(reader_thread);
    std::thread t2(writer_thread);

    t1.join();
    t2.join();
}