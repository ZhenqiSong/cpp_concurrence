//
// Created by Zhenqi Song on 2023/4/21.
// 使用 barrier

#include <barrier>
#include <vector>
#include <thread>

// 线程在完成自身的处理后，就运行到线程卡处，通过在线程卡对象上调用arrive_and_wait()等待同步组的其他线程。
// 只要组内最后一个线程也运行至此，所有线程即被释放，线程卡会自我重置。
// 接着，同步组的线程视具体情况各自继续，或处理下一项数据，或进行下一阶段的处理

class data_chunk{};
class result_chunk{};

result_chunk process(data_chunk);

std::vector<data_chunk>
divide_into_chunks(data_block data, unsigned num_threads);

void process_data(data_source& source, data_sink& sink){
    unsigned const concurrency = std::thread::hardware_concurrency();
    unsigned const num_threads = (concurrency > 0) ? concurrency : 2;

    std::barrier sync(num_threads);
    std::vector<joining_thread> threads(num_threads);
    std::vector<data_chunk> chunks;
    result_block result;

    for (unsigned i = 0; i < num_threads; ++i){
        threads[i] = joining_thread([&, i]{
            while (!source.done()){
                if (!i){ // 切分动作须以串行方式完成，
                    data_block current_block = source.get_next_data_block();
                    chunks = divide_into_chunks(current_block, num_threads);
                }
                sync.arrive_and_wait(); // 所有线程在线程卡上等待
                result.set_chunk(i, num_threads, process(chunks[i]));
                sync.arrive_and_wait(); // 继续等待，保证多有线程到达，否则任何线程不能继续前行
                if (!i){
                    sink.write_dat(std::move(result));
                }
            }
        });
    }
}

int main(){

}