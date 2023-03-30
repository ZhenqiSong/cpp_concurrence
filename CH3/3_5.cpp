//
// Created by Zhenqi Song on 2023/3/30.
// 线程安全的栈容器类，详细版

#include <exception>
#include <memory>
#include <mutex>
#include <stack>

struct empty_stack: std::exception{
    const char* what() const throw();
};

template<typename T>
class threadsafe_stack{
private:
    std::stack<T> data;
    mutable std:mutex m;

public:
    threadsafe_stack(){}

    // 栈可以复制，因为拷贝构造在源对象上锁住了互斥
    threadsafe_stack(const threadsafe_stack& other){
        std::lock_guard<std::mutex> lock(other.m);
        data = other.data; // 在构造函数的函数体内进行复制操作
    }

    threadsafe_stack& operator = (const threadsafe_stack&) = delete;

    void push(T new_value){
        std::lock_guard<std::mutex> lock(m);
        data.push(std::move(new_value));
    }

    std::shared_ptr<T> pop(){
        std::lock_guard<std::mutex> lock(m);
        if (data.empty()) throw empty_stack(); // 试图弹出前检查是否为空栈
        std::shared_ptr<T> const res(std::make_shared<T>(data.top())); // 改动栈容器前设置返回值
        data.pop();
        return res;
    }

    void pop(T& value){
        std::lock_guard<std::mutex> lock(m);
        if (data.empty()) throw empty_stack();
        value = data.top();
        data.pop();
    }

    bool empty() const{
        std::lock_guard<std::mutex> lock(m);
        return data.empty();
    }
};
