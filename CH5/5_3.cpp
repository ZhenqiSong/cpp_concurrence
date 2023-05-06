//
// Created by Zhenqi Song on 2023/5/6.
// 函数调用需要求出所传入参数的值，但求值运算次序并不明确

#include <iostream>

void foo(int a, int b){
    std::cout << a << "," << b << std::endl;
}

int get_num(){
    static int i = 0;
    return ++i;
}

int main(){
    foo(get_num(), get_num()); // get_num 发声两次调用，但没有明确的先后顺序，但实际测试通常是从右到左
}