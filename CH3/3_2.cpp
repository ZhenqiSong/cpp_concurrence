//
// Created by Zhenqi Song on 2023/3/29.
// 意外地向外传递引用，指向受保护的共享数据
// 不得向锁所在的作用域之外传递指针和引用，指向受保护的共享数据，
// 无论是通过函数返回值将它们保存到对外可见的内存，还是将它们作为参数传递给使用者提供的函数。

#include <mutex>

class some_data{
    int a;
    std::string b;

public:
    void do_something();
};


class data_wrapper{
private:
    some_data data;
    std::mutex m;

public:

    template<typename Function>
    void process_data(Function func){
        std::lock_guard<std::mutex> l(m);
        func(data); // 向使用者提供的函数传递受保护共享数据
    }
};

some_data* unprotected;

void malicious_function(some_data& protected_data){
    unprotected = &protected_data;
}

data_wrapper x;

// 由于漏掉了foo，未能实现数据的真正互斥
void foo(){
    // 传入恶意函数
    x.process_data(malicious_function);
    // 以无保护方式访问本应该收到保护的数据
    unprotected->do_something();
}

