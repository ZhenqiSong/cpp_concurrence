//
// Created by Zhenqi Song on 2023/4/6.
// 由代码清单4.1衍化出push()和wait_and_pop()

#include <queue>
#include <mutex>
#include <condition_variable>

template<typename T>
class threadsafe_queue{
private:
    std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

public:
    void push(T new_value){
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }

    void wait_and_pop(T& value){
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]{return !data_queue.empty();});

        value = data_queue.front();
        data_queue.pop();
    }
};


threadsafe_queue<int> data_queue;
void data_preparation_thread(){
    while (more_data_to_prepare()){
        int const data = prepare_data();
        data_queue.push(data);
    }
}

void data_processing_thread(){
    while (true){
        int data;
        data_queue.wait_and_pop(data);
        process(data);
        if (is_last_chunk(data))
            break;
    }
}