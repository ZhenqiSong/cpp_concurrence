//
// Created by Zhenqi Song on 2023/3/29.
//

#include <queue>

template<typename T, typename Container=std::queue<T>>
        class stack{
        public:
            explicit stack(const Container&);
            explicit stack(Container&& = Container());

            template<class Alloc> explicit stack<const Alloc&>;
            template<class Alloc> stack(const Container&, const Alloc&);

            template<class Alloc> stack(Container&&, const Alloc&);

            template<class Alloc> stack(stack&&, const Alloc&);

            // empty和size的结果不可信，因为一旦函数返回，其他线程就可以自由的访问栈容器，修改内容
            bool empty() const;
            size_t size() const;
            T& top();
            T const& top() const;
            void push(T const&);
            void push(T &&);
            void pop();
            void swap(stack&&);

            template<class... Args> void emplace(Args&&... args);
        };

static stack<int> a;

f(){
    if (!a.empty()){
        // 两句之间容易造成竞争
        auto res = a.top();
        a.pop();
    }
}
