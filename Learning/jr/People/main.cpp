#include<iostream>
#include "Person.h"
#include "List.h"
bool isEleven (Person* p) {
  return p->Age() == 11;
}
int main(){

  Person * a  = new Person("Timmy", 10);
  Person * b  = new Person("Joe", 11);
  List<Person> * t = new List<Person>();
  List<Person>  * t2 = new List<Person>(a, t);
  List<Person> * l = new List<Person>(b, t2);
  printf("Ready to filter:\n");
  List<Person> * v = l->Filter(isEleven);
  printf("Filtered:\n");
  v->print();
  std::cout << "\n\n\n";
  l->print();

}
