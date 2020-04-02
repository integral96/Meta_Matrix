#pragma once

#include <matrix_mult.hpp>
#include <boost/array.hpp>
#include <boost/thread.hpp>

template<typename T, size_t maxsize>
class Circuliar
{
private:
    boost::array<T, maxsize> data_arr;
    size_t head_, tail_;
    size_t capacity() const {
        return maxsize;
    }
    size_t size() const {
        return (tail_ - head_);
    }
    mutable boost::mutex qlock;
    mutable boost::condition_variable can_read;
    mutable boost::condition_variable can_write;
public:
    Circuliar() : head_(0), tail_(0) {}
    void pop() {
        boost::unique_lock<boost::mutex> lock(qlock);
        if(size() == 0) can_read.wait(lock, [this](){
            return size() > 0;
        });
        ++head_;
        lock.unlock();
        can_write.notify_one();
    }
    T top() {
        boost::unique_lock<boost::mutex> lock(qlock);
        if(size() == 0) can_read.wait(lock, [this](){
            return size() > 0;
        });
        T ref = data_arr[head_% maxsize];
        lock.unlock();
        return ref;
    }
    void push(T&& obj) {
        boost::unique_lock<boost::mutex> lock(qlock);
        if(size() == capacity()) can_write.wait(lock, [this](){
            return size() < capacity();
        });
        data_arr[tail_++ % maxsize] = std::move(obj);
        lock.unlock();
        can_read.notify_one();
    }
    size_t head() { return head_; }
    size_t tail() { return tail_; }
    size_t count() const {
        boost::unique_lock<boost::mutex> lock(qlock);
        return (tail_ - head_);
    }
};
