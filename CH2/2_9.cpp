//
// Created by Zhenqi Song on 2023/3/28.
//

#include <algorithm>
#include <thread>
#include <numeric>
#include <vector>


template<typename Iterator, typename T>
struct accumulate_back{
    void operator()(Iterator first, Iterator last, T& result){
        result = std::accumulate(first, last, result);
    }
};

// 并行实现累加计算
template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init){
    unsigned long const length = std::distance(first, last);

    if (!length)
        return init; // 如果容器为空，直接返回初始值

    // 根据每个线程最少处理量，基础出线程最大数
    unsigned long const min_per_thread = 25;
    unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

    // 结合物理机线程数量，计算出最终线程数
    // 通过hardware_concurrency获取硬件支持的线程数量，线程越多，上下问交换越频繁，导致性能降低
    unsigned long const hardware_threads = std::thread::hardware_concurrency();
    unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
    // 通过最终线程数，计算每个线程处理的数据量
    unsigned long const block_size = length / num_threads;

    // 创建n-1个线程
    std::vector<T> results(num_threads);
    std::vector<std::thread> threads(num_threads - 1);

    // 将数据分块，每个线程处理一部分数据
    Iterator block_start = first;
    for (unsigned long i = 0; i < (num_threads - 1); ++i){
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        threads[i] = std::thread(accumulate_back<Iterator, T>(), block_start, block_end, std::ref(results[i]));
        block_start = block_end;
    }

    // 主线程处理最后一个数据块
    accumulate_back<Iterator, T>()(block_start, last, results[num_threads - 1]);

    // 等待所有线程执行结束
    for (auto & entry: threads){
        entry.join();
    }

    // 把每个进程的结果进行累加，得到最终结果
    return std::accumulate(results.begin(), results.end(), init);
}