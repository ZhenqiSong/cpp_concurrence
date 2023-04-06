//
// Created by Zhenqi Song on 2023/4/3.
// 在比较运算的过程中，每次只锁住一个互斥

#include <mutex>

class Y{
private:
    int some_detail;
    mutable std::mutex m;

    // 通过调用此函数，获取需要比较的值，该函数先加锁保护数据，再取值。
    int get_detail() const{
        std::lock_guard<std::mutex> lock_a(m);
        return some_detail;
    }

public:
    Y(int sd): some_detail(sd){}

    friend bool operator==(Y const& lhs, Y const& rhs){
        if (&lhs == &rhs)
            return true;

        // 在新场景下，一次只有一个锁
        int const lhs_value = lhs.get_detail();
        // 此时他们的值可能被改变，造成比较失去意义
        int const rhs_value = rhs.get_detail();

        // 通过比较运算符对比取得的值。
        return lhs_value == rhs_value;
    }
};
