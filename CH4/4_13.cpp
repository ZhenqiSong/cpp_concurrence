//
// Created by Zhenqi Song on 2023/4/13.
// 运用future实现并行快速排序

#include <list>
#include <algorithm>
#include <future>

template<typename T>
std::list<T> parallel_quick_sort(std::list<T> input){
    if (input.empty()){
        return input;
    }

    std::list<T> result;
    result.splice(result.begin(), input, input.begin());

    T const& pivot = *result.begin();
    auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t){return t < pivot;});

    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

    // 获取链表的前半部分通过另一个线程获得，递归调用
    // 如果展开的线程太多，可能转为按同步方式生成
    std::future<std::list<T>> new_lower(std::async(&parallel_quick_sort<T>, std::move(lower_part)));
    auto new_higher(parallel_quick_sort(std::move(input)));

    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower.get());

    return result;
}