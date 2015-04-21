#include<iostream>
#include "Person.h"
#include "List.h"

int main(){

  Person a  = Person("Timmy", 10);
  Person b  = Person("Joe", 11);
  List<Person>  t = List<Person>();
  List<Person>  t2 = List<Person>(&a, &t);
  List<Person> l = List<Person>(&b, &t2);
  l.print();

}
