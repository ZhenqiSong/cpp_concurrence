//
// Created by Zhenqi Song on 2023/4/10.
// 定义特化的std::packaged_task<>类模板（不完整）

#include <future>
#include <string>
#include <vector>

using namespace std;

template<>
class packaged_task<std::string(std::vector<char>*, int)>{
public:
    template<typename Callable>
    explicit packaged_task(Callable&& f);

    std::future<std::string> get_future();
    void operator()(std::vector<char>*, int);
};
