//
// Created by Zhenqi Song on 2023/3/28.
// 因为thread支持移动语意，所以只要容器同样知悉移动意图，就可以状态thread对象

#include <thread>
#include <vector>

void do_work(unsigned id);

void f(){
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 20; ++i){
        // 生成线程
        threads.emplace_back(do_work, i);
    }

    // 依次在各线程上调用join
    for (auto& entry: threads)
        entry.join();
}