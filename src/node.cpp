
#include <iostream>
#include <set>

#include "message.hpp"
#include "transportDummyImpl.hpp"
#include "quorum.hpp"
#include "slot.hpp"
#include "node.hpp"
#include <chrono>
#include <stdio.h>
using namespace DISTPROJ;

Node::Node(NodeID _id, RPCLayer& _rpc) 
  : id(_id), rpc(_rpc), t(nullptr) {
  rpc.AddNode(id);
}

Node::Node(NodeID _id, RPCLayer& _rpc, Quorum _quorumSet) 
  : id(_id), rpc(_rpc), quorumSet(_quorumSet), t(nullptr) {
  rpc.AddNode(id);
}

NodeID Node::GetNodeID() { 
  return id; 
}

Quorum Node::GetQuorumSet() {
  return quorumSet;
}

void Node::PrintQuorumSet() {
  printf("Printing quorum set for node %i \n", id);
  printf("Threshold: %i ", quorumSet.threshold);
  printf("Quorum members : \n");
  std::set<NodeID>::iterator iter;
  for (iter=quorumSet.members.begin(); iter != quorumSet.members.end(); ++iter) {
    std::cout << (*iter) << "\n";
  }
}

LocalNode::LocalNode(NodeID _id, RPCLayer& _rpc)
  : Node(_id, _rpc) {
    mc = _rpc.GetClient(_id);
  };
LocalNode::LocalNode(NodeID _id, RPCLayer& _rpc, Quorum _quorumSet) 
  : Node(_id, _rpc, _quorumSet) {
    mc = _rpc.GetClient(_id);
  }; 

void LocalNode::Tick() {
  while (true){
  
    printf("%i Tick\n",id);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    // std::this_thread::yield();
  }
}

void LocalNode::Start() {
  printf(" Start\n");
  if (t == nullptr) {
    t = new std::thread(&LocalNode::Tick, this);
  }
}

void LocalNode::AddKnownNode(NodeID v) {
  knownNodes.insert(v);
}

void LocalNode::UpdateQurorum(Quorum _quorumSet) {
  quorumSet = _quorumSet;
}

void LocalNode::AddNodeToQuorum(NodeID v) {
  quorumSet.members.insert(v);
}

void LocalNode::SendMessage(Message& msg) {
// TODO : interface with FakeRPC.
  mc->Broadcast(&msg);
}

bool LocalNode::ReceiveMessage() {
  Message* msg = nullptr;
  bool received = mc->Receive(msg);
  if (received) {
    // PRINT here just to show we got it 
    std::cout << "Got a message\n";
  }
  return received;
}

void LocalNode::ProcessMessage(Message& msg) {
  if (log.find(msg.getSlot()) == log.end()) {
    log[msg.getSlot()] = new Slot(msg.getSlot());
  }
  log[msg.getSlot()]->handle(msg);
}

void LocalNode::DumpLog() {
  for (auto slot : log) {
    slot.second->Dump();
  }
}


