
#include <iostream>
#include "message.hpp"
#include "transportDummyImpl.hpp"
#include "quorum.hpp"
#include "slot.hpp"

#include "node.hpp"

using namespace DISTPROJ;


Node::Node(NodeID _id, RPCLayer& _rpc, Quorum& _quorumSet) : id(_id), rpc(_rpc), quorumSet(_quorumSet) {
// NodeID d =getNodeID();
// rpc.AddNode(d);
}

NodeID Node::getNodeID() { 
return id; 
}

Quorum& Node::getQuorumSet() {
return quorumSet;
}
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
log[msg.getSlot()] = new Slot(msg.getSlot());
}
log[msg.getSlot()]->handle(msg);
}

void LocalNode::DumpLog() {
for (auto slot : log) {
// slot.Dump();
}
}


