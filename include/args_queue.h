#pragma once

#include <list>
#include <string>

class ArgsQueue {
public:
    ArgsQueue();
    ~ArgsQueue();
    void Push(std::string s);
    std::string GetNext();
    void Clear();
    size_t Size();
    bool Empty();
private:
    std::list<std::string> args_;
};
