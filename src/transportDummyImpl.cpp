
#include <vector>
#include "queue.h"
#include "transport.h"
#include "transportDummyImpl.h"
#include "node.h"

using namespace DISTPROJ;

FakeRPCLayer::AddNode(Node& node) {
  messageQueues[node.id] = new Queue();
}

FakeRPCLayer::MessageClient FakeRPCLayer::GetClient(uint64_t id) {
  return MessageClient(id); 
}

FakeRPCLayer::MessageClient::MessageClient(uint64_t id) : id(id) {}

void FakeRPCLayer::FakeRPCLayer::MessageClient::Send(Message msg, uint64_t peerID) {
  messageQueues[peerID].Add(msg);
}


bool FakeRPCLayer::FakeRPCLayer::MessageClient::Recieve(Message& msg) {
  // We only have 1 thread dequeing so this is chill.
  if (messageQueues[id].Empty()) {
    return false;
  } else {
    *msg = messageQueues[id].Get();
    return true;
  }
}

void FakeRPCLayer::FakeRPCLayer::MessageClient::Broadcast(Message msg) {
  // Client messages itself.
  for (auto peerID : messageQueues) {
    Send(msg, peerID);
  }
}

