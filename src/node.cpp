
#include <iostream>
#include "node.h"

using namespace DISTPROJ;

Node::Node(uint64_t id, Quorum quorumSet) : id(id), quorumSet(quorumSet) {};

template <class T>
void LocalNode<T>::Tick() {
  std::cout << "Tick\n";
}
