
#include <vector>
#include "queue.hpp"
#include "RPC.hpp"
#include "node.hpp"
#include "slot.hpp"
#include "fakeRPC.hpp"
#include "message.hpp"
using namespace DISTPROJ;


FakeRPCLayer::FakeRPCLayer(){}

void FakeRPCLayer::AddNode(NodeID node) {
  messageQueues[node] = new Queue<Message*>();
}

MessageClient* FakeRPCLayer::GetClient(NodeID id) {
  return new MessageClient(id, this); 
}






void FakeRPCLayer::Send(Message* msg, NodeID id, NodeID peerID) {
  messageQueues[peerID]->Add(msg);
}


bool FakeRPCLayer::Receive(Message** msg, NodeID id) {
  // We only have 1 thread dequeing so this is chill.
  if (messageQueues[id]->Empty()) {
    return false;
  } else {
    *msg = messageQueues[id]->Get();
    return *msg; // implicitly checks for validity
  }
}

void FakeRPCLayer::Broadcast(Message* msg, NodeID id) {
  // Client messages itself.
  for (auto peerID : messageQueues) {
    Send(msg, id, peerID.first);
  }
}







MessageClient::MessageClient(NodeID id, RPCLayer* r) : id(id), rpc(r) {}

void MessageClient::Send(Message* msg, NodeID peerID) {
  rpc->Send(msg, id, peerID);
}


bool MessageClient::Receive(Message** msg) {
  return rpc->Receive(msg, id);
}

void MessageClient::Broadcast(Message* msg) {
  rpc->Broadcast(msg, id);
}

