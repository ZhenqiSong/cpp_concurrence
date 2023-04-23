//
// Created by Zhenqi Song on 2023/4/11.
// 快排的串行实现

#include <list>
#include <algorithm>

template<typename T>
std::list<T> sequential_quick_sort(std::list<T> input){
    if (input.empty()){
        return input;
    }

    std::list<T> result;

    result.splice(result.begin(), input, input.begin()); // 将整个input复制到result中
    T const& pivot = *result.begin(); // 定义基准点

    auto divide_point = std::partition(input.begin(), input.end(), [&](T const& t){return t < pivot;});

    std::list<T> lower_part;
    lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

    auto new_lower(sequential_quick_sort(std::move(lower_part)));
    auto new_higher(sequential_quick_sort(std::move(input)));

    result.splice(result.end(), new_higher);
    result.splice(result.begin(), new_lower);
    return result;
}
