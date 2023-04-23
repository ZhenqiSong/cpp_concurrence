//
// Created by Zhenqi Song on 2023/4/21.
// 使用std::experimental::latch实例等待多个目标事件

#include <experimental/latch>
#include <vector>
#include <future>


void foo(){
    unsigned const thread_count = ...;
    latch done(thread_count); // 构建线程闩
    my_data data[thread_count];
    std::vector<std::future<void>> threads;

    for (unsigned i=0; i<thread_count; ++i){
        // 发起相同数量的线程
        // 使用引用访问共享变量，但是i不能是引用，因为它每个循环时变量
        threads.push_back(std::async(std::launch::async,
                                     [&, i]{
            data[i] = make_data(i);
            done.count_down(); // 各线程负责生成相关的数据块，在完成时令计数器减持
            do_more_stuff();
        }));
        done.wait(); //使用线程闩等待
        process_data(data, thread_count); // 主进程处理数据
    }
}
