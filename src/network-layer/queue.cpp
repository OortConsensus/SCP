#include "queue.h"

template <class T>
Queue<T>::Queue() {}

template <class T>
void Queue<T>::Add(T value) {
  this.mtx.lock();
  this.elements.push(value);
  this.mtx.unlock();
}

template <class T>
T Queue<T>::Get() {
  this.mtx.lock();
  T element = this.elements.pop();
  this.mtx.unlock();
  return element;
}

template <class T>
size_t Queue<T>::Size() {
  this.mtx.lock();
  size_t size = this.elements.size();
  this.mtx.unlock();
  return size;
}

template <class T>
bool Queue<T>::Empty() {
  return this.Size() == 0;
}

