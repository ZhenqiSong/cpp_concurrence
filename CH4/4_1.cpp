//
// Created by Zhenqi Song on 2023/4/6.
// 用std::condition_variable等待处理数据

// std::condition_variable仅限于与std::mutex一起使用；然而，只要某一类型符合成为互斥的最低标准，足以充当互斥，
// std::condition_variable_any即可与之配合使用，因此它的后缀是“_any”。
// 由于std::condition_variable_any更加通用，它可能产生额外开销，涉及其性能、自身的体积或系统资源等，
// 因此std::condition_variable应予优先采用，除非有必要令程序更灵活。

#include <mutex>
#include <condition_variable>
#include <queue>

class data_chunk{};
std::mutex mut;
std::queue<data_chunk> data_queue; // 使用队列在两个线程中间传递数据
std::condition_variable data_cond;

// 线程A
void data_preparation_thread(){
    while (more_data_to_prepare()){
        data_chunk const data = prepare_data();
        {
            // 一旦线程A准备好数据，就锁住互斥保护队列，并压入数据
            std::lock_guard<std::mutex> lk(mut);
            data_queue.push(data);
        }
        // 通知线程B
        data_cond.notify_one();
    }
}

// 线程B
void data_processing_thread(){
    while (true){
        // 先加锁，使用unique_lock是为了方便解锁
        std::unique_lock<std::mutex> lk(mut);
        // 传入一个锁对象和一个lambda函数，后者用于表达需要等待成立的条件
        // 如果lambda结果为false，解除互斥则线程进入线程阻塞，等待线程A通知解除阻塞
        // 收到通知重新获取锁，检查，直到结果为True，然后锁住互斥继续执行
        data_cond.wait(lk, []{return !data_queue.empty();});

        data_chunk data = data_queue.front();
        data_queue.pop();
        lk.unlock();
        process(data);

        if (is_last_chunk(data))
            break;
    }
}