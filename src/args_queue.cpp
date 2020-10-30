#include "args_queue.h"

ArgsQueue::ArgsQueue() {

}

ArgsQueue::~ArgsQueue() {

}

void ArgsQueue::Push(std::string s) {
    args_.push_back(s);
}

/* GetNext() returns an empty string if the ArgsQueue is empty */
std::string ArgsQueue::GetNext() {
    std::string s;
    if (!args_.empty()) {
        s = args_.front();
        args_.pop_front();
    }
    return s;
}

void ArgsQueue::Clear() {
    args_.clear();
}

size_t ArgsQueue::Size() {
    return args_.size();
}

bool ArgsQueue::Empty() {
    return args_.empty();
}
