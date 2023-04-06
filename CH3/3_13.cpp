//
// Created by Zhenqi Song on 2023/4/6.
// 使用std::shared_mutex保护数据结构

#include <map>
#include <string>
#include <mutex>
#include <shared_mutex>

class dns_entry;
class dns_cache{
    std::map<std::string, dns_entry> entries;
    mutable std::shared_mutex entry_mutex;

public:
    dns_entry find_entry(std::string const& domain) const{
        // 使用std::shared_lock保护共享的，只读访问，所以多个线程得以同时调用find_entry
        std::shared_lock<std::shared_mutex> lk(entry_mutex);
        std::map<std::string, dns_entry>::const_iterator const it = entries.find(domain);
        return (it == entries.end()) ? dns_entry() : it->second;
    }

    void update_or_add_entry(std::string const& domain, dns_entry const& dns_details){
        // 更新时采用排他访问，如果其他线程同时调用，将会被堵塞，且读取线程也会被堵塞
        std::lock_guard<std::shared_mutex> lk(entry_mutex);
        entries[domain] = dns_details;
    }
};
