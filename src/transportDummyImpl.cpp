
#include <vector>
#include "queue.h"
#include "transport.h"
#include "transportDummyImpl.h"
#include "Node.h"

using namespace DISTPROJ;

template <class T>
FakeRPCLayer<T>::FakeRPCLayer(std::vector<Node> nodes) {
  for (auto node : nodes) {
    messageQueues[node.id] = new Queue<T>();
  }
}

template <class T>
typename FakeRPCLayer<T>::MessageClient FakeRPCLayer<T>::GetClient(uint64_t id) {
  return MessageClient(id); 
}

template <class T>
FakeRPCLayer<T>::MessageClient::MessageClient(uint64_t id) : id(id) {}

template <class T>
void FakeRPCLayer<T>::FakeRPCLayer::MessageClient::Send(T msg, uint64_t peerID) {
  this.messageQueues[peerID].Add(msg);
}


template <class T>
bool FakeRPCLayer<T>::FakeRPCLayer::MessageClient::Recieve(T* msg) {
  // We only have 1 thread dequeing so this is chill.
  if (this.messageQueues[this.id].Empty()) {
    return false;
  } else {
    *msg = this.messageQueues[this.id].Get();
    return true;
  }
}

template <class T>
void FakeRPCLayer<T>::FakeRPCLayer::MessageClient::Broadcast(T msg) {
  // Client messages itself.
  for (auto peerID : this.messageQueues) {
    this.Send(msg, peerID);
  }
}

