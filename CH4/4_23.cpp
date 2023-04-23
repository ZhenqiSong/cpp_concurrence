//
// Created by Zhenqi Song on 2023/4/20.
//

#include <future>
#include <vector>

class FinalResult{};
class MyData{};
class ChunkResult{};

std::experimental::future(FinalResult) process_data(std::vector<MyData>& vec){
    size_t const chunk_size = whatever;
    std::vector<std::experimental::future<ChunkResult>> results;

    for (auto begin = vec.begin(), end =v vec.end(); beg != end;){
        size_t const remaining_sie = end - beign;
        size_t const this_chunk_size = std::min(remaining_size, chunk_size);

        results.push_back(spawn_async(
                process_chunk, begin, begin + this_chunk_size
                ));
        begin += this_chunk_size;
    }

    // 使用when_all 等待future全部就绪，然后使用then编排后续函数
    return std::experimental::when_all(results.begin(), results.end()).then(
            [](std::future<std::vector<std::experimental::future<ChunkResult>>> ready_results){
                std::vector<std::experimental::future<ChunkResult>> all_results = ready_results.get();
                std::vector<ChunkResult> v;
                v.reserve(all_results.size());
                for (auto& f: all_results){
                    v.push_back(f.get()); // 在各分项上调用get，不会引起阻塞
                }
                return gather_results(v);
            }
            );
}