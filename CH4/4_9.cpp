//
// Created by Zhenqi Song on 2023/4/10.
// 使用std::packaged_task在GUI的线程上运行代码

#include <deque>
#include <mutex>
#include <future>
#include <thread>
#include <utility>

std::mutex m;
std::deque<std::packaged_task<void()>> tasks;
bool gui_shutdown_message_received();
void get_and_process_gui_message();

// GUI线程
void gui_thread(){
    // 如果消息指示界面不关闭，则一直遍历任务队列
    while (!gui_shutdown_message_received()){
        get_and_process_gui_message();
        std::packaged_task<void()> task;
        {
            std::lock_guard<std::mutex> lk(m);
            if (tasks.empty()) // 如果队列为空，则继续
                continue;

            // 否则取出任务，释放锁，随即运行任务
            task = std::move(tasks.front());
            tasks.pop_front();
        }
        task(); // 任务完成，对应的future就进入就绪状态
    }
}

// 向队列布置任务
std::thread gui_bg_thread(gui_thread);
template<typename Func>
std::future<void> post_task_for_gui_thread(Func f){
    std::packaged_task<void()> task(f); // 创建新任务，将函数包装在内
    std::future<void> res = task.get_future(); // 取得任务关联的future
    std::lock_guard<std::mutex> lk(m); // 向关联的GUI线程投递信息
    tasks.push_back(std::move(task));
    return res;
}