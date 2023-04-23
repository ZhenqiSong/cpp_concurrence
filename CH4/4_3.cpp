//
// Created by Zhenqi Song on 2023/4/6.
// 线程安全的队列容器 threadsafe_queue

#include <memory>

template<typename T>
class threadsafe_queue{
public:
    threadsafe_queue();
    threadsafe_queue(const threadsafe_queue&);
    threadsafe_queue& operator=(const threadsafe_queue&) = delete; // 禁止赋值操作

    void push(T new_value);
    // try_pop 弹出总是立即返回，即使为空
    bool try_pop(T& value);
    std::shared_ptr<T> try_pop();
    // 直到有数据才返回
    void wait_and_pop(T& value);
    std::shared_ptr<T> wait_and_pop();
    bool empty() const;
};
