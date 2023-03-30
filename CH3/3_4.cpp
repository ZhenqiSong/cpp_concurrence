//
// Created by Zhenqi Song on 2023/3/30.
// 线程安全的栈容器类 简要定义

#include <exception>
#include <memory>

struct empty_stack: std::exception{
    const char* what() const noexcept;
};


template<typename T>
class threadsafe_stack{
public:
    threadsafe_stack();
    // 删除赋值运算符，栈容器本身不能赋值
    threadsafe_stack(const threadsafe_stack&) = delete;
    // 没有swap函数

    void push(T new_value);
    std::shared_ptr<T> pop();

    void pop(T& value);
    bool empty() const;
};