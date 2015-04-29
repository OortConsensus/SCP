
#ifndef QUEUE_H
#define QUEUE_H

#include <mutex>
#include <queue>
#include <cstdlib>

template <class T>
class Queue {

  std::mutex mtx;
  std::queue<T> elements;

  public:
    Queue();
    void Add(T value);
    T Get();
    size_t Size();
    bool Empty();

};

#endif 
