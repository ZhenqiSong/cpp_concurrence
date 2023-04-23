//
// Created by Zhenqi Song on 2023/4/7.
// 通过std::async 向任务函数传递参数


#include <string>
#include <future>

struct X{
    void foo(int, std::string const&);
    std::string bar(std::string const&);
};

struct Y{
    double operator()(double );
};

X baz(X&);
auto res = std::async(baz, std::ref(x)); // 调用baz(x)

class move_only{
public:
    move_only();
    move_only(move_only&&);
    move_only(move_only const&) = delete;
    move_only& operator=(move_only&&);
    move_only& operator=(move_only const&) = delete;
    void operator()();
};

// 能够给std::async()补充一个参数，以指定采用哪种运行方式。
// 参数的类型是std::launch，其值可以是std::launch::deferred或std::launch::async[8]。
// 前者指定在当前线程上延后调用任务函数，等到在future上调用了wait()或get()，任务函数才会执行；
// 后者指定必须另外开启专属的线程，在其上运行任务函数。
// 该参数的值还可以是std::launch::deferred | std::launch:: async，表示由std::async()的实现自行选择运行方式。最后这项是参数的默认值。
int main() {
    X x;
    auto f1 = std::async(&X::foo, &x, 42, "hello"); // 调用&x->foo(42, "hello")
    auto f2 = std::async(&X::bar, x, "goodbye"); // 调用 tempx.bar("goodbye"), tempx是x的副本

    Y y;
    // 调用tempy(3.141),其中Y()生成一个匿名变量，传递给std::async(),进而发生移动构造。
    // 在std::async()内部产生对象tmpy，在tmpy上执行Y::operator()
    auto f3 = std::async(Y(), 3.141);
    auto f4 = std::async(std::ref(y), 2.718); // 调用y(2.718)

    auto f5 = std::async(move_only()); // 调用tep(), 其中tmp等价于std::move(move_only())

    auto f6 = std::async(std::launch::async, Y(), 1.2); // 启动新线程
    auto f7 = std::async(std::launch::deferred, baz, std::ref(x)); // 在wait 或 get内部运行任务函数
    auto f8 = std::async(std::launch::async | std::launch::deferred, baz, std::ref(x));
    auto f9 = std::async(baz, std::ref(x)); // 交由实现自行选择
    f7.wait(); // f7到此时才执行
}

