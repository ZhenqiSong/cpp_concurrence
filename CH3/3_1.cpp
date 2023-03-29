//
// Created by Zhenqi Song on 2023/3/29.
// 使用互斥+数据组成一个类

#include <list>
#include <mutex>
#include <algorithm>

std::list<int> some_list; // 独立的全局变量
std::mutex some_mutex;

void add_to_list(int new_value){
    // 使用lock_guard<std::mutex> 对链表进行互斥访问
    // 当add_to_list正在修改链表，list_contains绝对看不到该状态
    std::lock_guard<std::mutex> guard(some_mutex);
    some_mutex.push_back(new_value);
}

bool list_contains(int value_to_find){
    std::lock_guard<std::mutex> guard(some_mutex);
    return std::find(some_list.begin(), some_list.end(), value_to_find) != some_list.end();
}

