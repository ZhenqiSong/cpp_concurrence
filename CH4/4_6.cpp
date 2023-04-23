//
// Created by Zhenqi Song on 2023/4/7.
// 运用std::future 取得异步任务的函数返回值

#include <future>
#include <iostream>

int find_the_answer_to_ltuae(){
    return 1;
}

void do_other_stuff(){
    std::cout << "do other stuff" << std::endl;
}

int main(){
    // 使用异步方式启动任务，获取一个future对象，一旦任务结束，返回值就由该对象所有
    std::future<int> the_answer = std::async(find_the_answer_to_ltuae);

    do_other_stuff();

    // 使用get获取任务的返回值
    std::cout << "The answer is " << the_answer.get() << std::endl;
}
