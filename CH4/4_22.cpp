//
// Created by Zhenqi Song on 2023/4/20.
//

#include <future>
#include <vector>

class FinalResult{};
class ChunkResult{};
class MyData{};

std::future<FinalResult> process_data(std::vector<MyData>& vec){
    size_t const chunk_size = whatever;
    std::vector<std::future<ChunkResult>> results;

    for (auto begin = vec.begin(), end = vec.end(); begin != end;){
        size_t const  remaining_size = end - begin;
        size_t const this_chunk_size = std::min(remaining_size, chunk_size);

        results.push_back(std::async(process_chunk, begin, begin+this_chunk_size));
        begin += this_chunk_size;
    }

    // 单独一个进程用于接收所有的进程计算的结果
    return std::async([all_results = std::move(results)](){
        std::vector<ChunkResult> v;
        v.reserve(all_results.size());
        for (auto& f: all_results){
            v.push_back(f.get()); // 因为它等待每个任务，但凡有分项结果的出，就会被系统任务调度器反复唤醒
        }
        return gather_results(v);
    });
}
