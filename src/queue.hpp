
#ifndef QUEUE_H
#define QUEUE_H

#include <mutex>
#include <queue>
#include <cstdlib>

template <class T>
class Queue {

  struct internal {
    T& i;
  };
  std::mutex mtx;
  std::queue<internal> elements;
  public:
    Queue();
    void Add(T& value);
    T& Get();
    size_t Size();
    bool Empty();

};

#endif 
