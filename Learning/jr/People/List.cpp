#include "List.h"
#include "Person.h"
template <class T>
List<T>::List() {
  tl = NULL;
  item = NULL;
}
template <class T>
List<T>::List(T * t, List<T> * tl_) {
  tl = tl_;
  item = t;
  
}

template <class T>
List<T>::~List() {
  
}
template <class T>
List<T> List<T>::Tail(){

  return *tl;

}

template <class T>
T List<T>::Head(){

  return *item;

}
template <class T>
void List<T>::print() {
  List<T> * p = this;
  while (p->tl != NULL) {
    p->item->print();
    p = p->tl;
    
  }

}

template class List<Person>;
