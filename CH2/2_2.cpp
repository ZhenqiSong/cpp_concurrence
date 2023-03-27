//
// Created by SongZhenqi on 2023/3/27.
// 发生异常的情况下，如果确保线程join?
//

#include <thread>

struct func{
    int &i;

    func(int& i_):i(i_){}

    void operator()(){
        for(unsigned j = 0; j < 10000; j++){
            cout << i + 1 << endl;
        }
    }
};

void f(){
    int some_local_state = 0;

    func my_func(some_local_state);
    std::thread t(my_func);

    try{
        do_something_in_current_thread();
    }
    catch (...){
        t.join(); // do_something... 发生异常时确保线程join
        throw;
    }

    t.join(); // 正常情况下确保join
}

// 这种方式略显冗余，并非理想方案