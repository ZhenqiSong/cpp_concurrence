//
// Created by Zhenqi Song on 2023/4/3.
// 用互斥实现线程安全的延迟初始化

#include <memory>
#include <mutex>

std::shared_ptr<some_resource> resource_ptr;
std::mutex resource_mutex;

void foo(){
    std::unique_lock<std::mutex> lk(resource_mutex); // 此处所有线程都被迫循序运行

    if (!resource_ptr){
        resource_ptr.reset(new some_resource); // 仅有初始化需要保护
    }
    lk.unlock();

    resource_ptr->do_something();
}
