
#include <iostream>
#include <set>

#include "message.hpp"
#include "fakeRPC.hpp"
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
  printf("Printing quorum set for node %llu \n", id);
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
  std::shared_ptr<Message> m;
  while (true){
    std::lock_guard<std::mutex> lock(mtx);
    if (ReceiveMessage(&m)) {
      ProcessMessage(m);


    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
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
void LocalNode::RemoveNodeFromQuorum(NodeID v) {
  quorumSet.members.erase(v);
}
int LocalNode::QuorumSize(){
  return quorumSet.members.size();

}
void LocalNode::SetThreshold(int t){
  if (t > QuorumSize()) {
    quorumSet.threshold = QuorumSize();
  }
  else{
    quorumSet.threshold = t;
  }
}
int LocalNode::GetThreshold(){
  return quorumSet.threshold;
}

SlotNum LocalNode::Propose(std::string value){
  std::lock_guard<std::mutex> lock(mtx);
  auto i = NewSlot();
  auto b = Ballot{1, value};
  printf("Finding Nonce\n");
  auto nonce = generateNonce(&b, i);
  printf("Nonce Found %llu\n", nonce);
  auto m = std::make_shared<PrepareMessage>(id, i, b, Ballot{}, Ballot{}, Ballot{}, quorumSet, 0); /* TODO; resending etc */
  SendMessage(m);
  printf("messages sent\n");
  return i;
}
SlotNum LocalNode::NewSlot(){
  auto a = maxSlot;
  maxSlot++;
  return a;
}

void LocalNode::SendMessage(std::shared_ptr<Message> msg) {
  // TODO : interface with FakeRPC.
  mc->Broadcast(msg);
}

void LocalNode::SendMessageTo(std::shared_ptr<Message> msg, NodeID i) {
  // TODO : interface with FakeRPC.
  mc->Send(msg, i);
}

bool LocalNode::ReceiveMessage(std::shared_ptr<Message>* msg) {
  bool received = mc->Receive(msg);
  if (received && msg) {

    // PRINT here just to show we got it 
    printf("Got a message\n");
    return true;
  }
  return false;
}

void LocalNode::ProcessMessage(std::shared_ptr<Message> msg) {
  auto slot = msg -> getSlot();
  if (log.find(slot) == log.end()) {
    log[slot] =std::make_shared<Slot>(slot, this);
    if (slot > maxSlot) {
      maxSlot = slot;
    }
  }
  log[msg->getSlot()]->handle(msg);
}

void LocalNode::DumpLog() {
  for (auto slot : log) {
    slot.second->Dump();
  }
}



std::pair<std::string, bool> LocalNode::View(SlotNum s){
  std::lock_guard<std::mutex> lock(mtx);
  try{
    printf("VIEW: %s\n", log.at(s)->Phase_s().c_str());
    return std::pair<std::string, bool>(log.at(s)->GetValue(), log.at(s)->GetPhase() == EXTERNALIZE);
  } catch (std::out_of_range){
    return std::pair<std::string, bool>("", false);
  }
}
