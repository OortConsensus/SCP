#include<iostream>

template <class T>
class List {
  List<T> *tl;
  T * item;
 public:
  List();
  List(T * l, List<T>*  tl_);
  void print();
  List<T>* Tail();
  T Head();
  virtual ~List<T>();
   List<T>* Filter(bool (*f)(T* a));

};


