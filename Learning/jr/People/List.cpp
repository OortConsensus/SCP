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
List<T>* List<T>::Tail(){
  return tl;

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


template <class T>
List<T>* List<T>::Filter(bool (*f)(T *a)) {
  List<T> * p = this;
  List<T> * ret = new List();
  while (p->tl != NULL) {
    if (f((p->item))) {
      ret = new List(p->item, ret);
    }
    p = p->tl;
  }
  return ret;

}

template class List<Person>;
