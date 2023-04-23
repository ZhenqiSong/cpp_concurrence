//
// Created by Zhenqi Song on 2023/4/6.
// 线程安全队列的完整的类定义，其中采用了条件变量

#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

template<typename T>
class threadsafe_queue{
private:
    mutable std::mutex mut; // 互斥必须使用mutable修饰，针对const对象，准许其数据成员发生变动
    std::queue<T> data_queue;
    std::condition_variable data_cond;

public:
    threadsafe_queue(){}

    threadsafe_queue(threadsafe_queue const& other){
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue = other.data_queue;
    }

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

    std::shared_ptr<T> wait_and_pop(){
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]{return !data_queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }

    bool try_pop(T& value){
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = data_queue.front();
        data_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop(){
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;

        std::shared_ptr<T> res = std::make_shared<T>(data_queue.front());
        data_queue.pop();
        return res;
    }

    bool empty() const{
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};


// 如果几个线程都在等待同一个目标事件，那么还存在另一种可能的行为方式：它们全部需要做出响应。
// 以上行为会在两种情形下发生：
//     一是共享数据的初始化，所有负责处理的线程都用到同一份数据，但都需要等待数据初始化完成；
//     二是所有线程都要等待共享数据更新（如定期执行的重新初始化）。
// 尽管条件变量适用于第一种情形，但我们可以选择其他更好的处理方式，如std::call_once()函数
// 前文中，负责准备的线程原本在条件变量上调用notify_one()，针对第二种情况只需改为调用成员函数notify_all()。
// 顾名思义，该函数通知当前所有执行wait()而正在等待的线程，让它们去查验所等待的条件。