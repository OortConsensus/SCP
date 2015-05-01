
#include <iostream>
#include "quorum.h"
#include "node.h"


using namespace DISTPROJ;

Node::Node(uint64_t _id, const RPCLayer& _rpc) : id(_id) rpc(_rpc) {
  quorumSet = Quorum();
  _rpc->AddNode(_id);
}

Node::Node(uint64_t _id, const RPCLayer& _rpc, Quorum& _quorumSet) 
  : id(_id), rpc(_rpc), quorumSet(_quorumSet) {
    _rpc->AddNode(_id);
  };

void LocalNode::Tick() {
  std::cout << "Tick\n";
}

void LocalNode::AddKnownNode(NodeID v) {
  knownNodes.insert(v);
}

void LocalNode::UpdateQurorum(Quorum& _quorumSet) {
  quorumSet = _quorumSet;
}

void LocalNode::SendMessage(Message& msg) {
  // TODO : interface with FakeRPC.
}

void LocalNode::ProcessMessage(Message& msg) {
  if (log.find(msg.getSlot()) == log.end()) {
    log[msg.getSlot()] = Slot(msg.getSlot());
  }
  log[msg.getSlot()].handle(msg)
}

void LocalNode::DumpLog() {
  for (auto slot : log) {
    slot.Dump();
  }
}


