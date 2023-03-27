//
// Created by SongZhenqi on 2023/3/27.
// 当前线程的函数已返回，新线程却仍能访问其局部变量

#include <thread>

struct func{
    int &i;

    func(int& i_):i(i_){}

    void operator()(){
        for(unsigned j = 0; j < 10000; j++){
            cout << i + 1 << endl;
            // 如果oops运行结束，而新线程还在运行，就可能返回已经被销毁的局部变量 some_local_state,也就是i的引用值
            // 解决方法就是令线程函数自含，将数据复制到新线程内部，而不是共享数据。

        }
    }
};

void oops(){
    int some_local_state = 0;

    func my_func(some_local_state);

    std::thread my_thread(my_func);

    my_thread.detach(); // 明确不等待新线程结束

    // 还有一种方法是，汇合新线程，确保其执行结束
    // 对于某个给定的线程，仅能调用一次join
    // my_thread.join();
} // oops结束的话，新线程可能还在婞，

