
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

template <class T>
Queue<T>::Queue() {}

template <class T>
void Queue<T>::Add(T value) {
  this->mtx.lock();
  this->elements.push(value);
  this->mtx.unlock();
}

template <class T>
T Queue<T>::Get() {
  this->mtx.lock();
  T element = this->elements.front();
  this->elements.pop();
  this->mtx.unlock();
  return element;
}

template <class T>
size_t Queue<T>::Size() {
  this->mtx.lock();
  size_t size = this->elements.size();
  this->mtx.unlock();
  return size;
}

template <class T>
bool Queue<T>::Empty() {
  return Size() == 0;
}

#endif 
