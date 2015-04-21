#include "Person.h"
#include<iostream>
using namespace std;

Person::Person(string n, int a) {
  name =  n;
  age = a;
}

void Person::print(){
  


  std::cout << name <<" is "<<age<<" years old\n";

}

Person::~Person(){
  std::cout << name << " Destructor Called\n";


}

int Person::Age(){
  return age;
}
