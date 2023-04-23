//
// Created by Zhenqi Song on 2023/4/23.
//


#include <thread>
#include <vector>

class data_block;

class data_source{
public:
    data_block get_next_data_block(){}
};
class data_sink{};
class data_chunk{};
extern class data_block{};
class result_block{};

void process_data(data_source &source, data_sink &sink){
    unsigned const concurrency = std::thread::hardware_concurrency();
    unsigned const num_threads = (concurrency > 0) ? concurrency : 2;

    std::vector<data_chunk> chunks;
    auto split_source = [&]{
        if (!source.done()){
            data_block current_block = source.get_next_data_block();
            chunks = divide_into_chunks(current_block, num_threads);
        }
    };

    split_source();
    result_block result;

    // 向构造函数传入线程数目和补全函数，当各个进程都到达线程卡后，它们当中的一个就会运行补全函数
    std::experimental::flex_barrier sync(num_threads, [&]{
        sink.write_data(std::move(result));
        split_source();
        return -1; // 返回-1，说明参与同步的线程数目不变，返回0或正数，则将其作为指标，设定参与下一同步周期的线程数目
    })

    std::vector<joining_thread> threads(num_threads);

    for (unsigned i = 0; i < num_threads; ++i){
        threads[i] = joining_thread([&, i]{
            while (!source.done()){ // 主循环得到了简化
                result.set_chunk(i, num_threads, process(chunks[i]));
                sync.arrive_and_wait(); // 只需要一个同步点， 相较于原来的不一定需要主进程执行
            }
        });
    }
}