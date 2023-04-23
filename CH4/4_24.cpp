//
// Created by Zhenqi Song on 2023/4/20.
//

#include <thread>
#include <vector>
#include <atomic>
#include <memory>

class MyData{};
class FinalResult{};

std::experimental::future<FinalResult> find_and_process_value(std::vector<MyData>& data){
    unsigned const concurrency = std::thread::hardware_concurrency();
    unsigned const num_tasks = (concurrency > 0) ? concurrency : 2;

    std::vector<std::experimental::future<MyData*>> results;
    auto const chunk_size = (data.size() + num_tasks - 1) / num_tasks;
    auto chunk_begin = data.begin();
    std::shared_ptr<std::atomic<bool>> done_flag = std::make_shared<std::atomic<bool>>(false);

    // 产生多个异步任务
    for (unsigned i = 0; i < num_tasks; ++i){
        auto chunk_end = (i < (num_tasks - 1)) ? chunk_begin + chunk_size : data.end();
        // 每个任务都有自己的chunk_begin值和chunk_end值，还具有共享指针done_flag的副本
        results.push_back(spawn_async([=]{
            for (auto entry = chunk_begin; !*done_flag && (entry != chunk_end); ++entry){
                if (matches_find_criteria(*entry)){
                    *done_flag = true;
                    return &*entry;
                }
            }
            return (MyData*) nullptr;
        }));
        chunk_begin = chunk_end;
    }

    std::shared_ptr<std::experimenal::promise<FinalResult>> final_result = std::make_shared<std::experimental::promise<FinalResult>>();

    struct DoneCheck{
        std::shared_ptr<std::experimental::promise<FinalResult>> final_result;
        DoneCheck(std::shared_ptr<std::experimental::promise<FinalResult>> final_result_):final_result(std::move(final_result_)){}

        void operator()(std::experimental::future<std::experimental::when_any_result<std::vector<std::experimental::future<MyData*>>>> results_param){
            auto results = results_param.get(); // when any result 具有索引
            MyData *const ready_result = results.futures[results.index].get(); // 从就绪的future中提取结果

            if (ready_result)
                final_result->set_value(process_found_value(*ready_result)); // 若找到目标值，则做出处理，返回
            else{
                results.futures.erase(result.futures.begin() + results.index); // 如果没有找到目标值，从集合中将已就绪的future丢弃
                // 如果还有future需要检查，则发起新的whenany调用，那么下一个future就绪时，后续函数就会被触发
                if (!results.futures.empty()){
                    std::experimental::when_any(results.futures.begin(), results.futures.end()).then(std::move(*this));
                }else{ // 如果没有剩余的future等待处理，则所有任务都没有找到目标值，向最终结果存入异常
                    final_result->set_exception(std::make_exception_ptr(std::runtime_error("Not found")));
                }
            }
        }
    };

    // 通过连锁调用的形式添加后续函数
    std::experimental::when_any(results.begin(), results.end()).then(DoneCheck(final_result));
    return final_result->get_future();
}

// 用到的std::experimental::when_all()和std::experimental::when_any()都是基于迭代器范围的重载函数，
// 它们都接收一对迭代器作为参数，代表容器范围的开头和结尾，我们需要等待其范围内的future。
// 这两个函数还具有可变参数的重载形式，能接收多个future直接作为参数，都返回future对象，
// 而不是vector容器：std::experimental::when_all()所返回的future持有元组（tuple），
// 而when_any()返回的future持有when_any_result实例。