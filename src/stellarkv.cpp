#include "stellarkv.hpp"
#include <random>

using namespace DISTPROJ;
using namespace DISTPROJ::Application::StellarKV;
using namespace std;

StellarKV::StellarKV(shared_ptr<RPCLayer> rpc, float tr)  : threshold_ratio(tr){
  random_device rd ;
  mt19937 gen(rd());
  uniform_int_distribution<NodeID> dist(0, ~0);
  node = new LocalNode(dist(gen), *rpc, Quorum{});
  slot = 0;
  node->Start();
  t = new thread(&StellarKV::Tick, this);
}
NodeID StellarKV::GetNodeID(){
  return node->GetNodeID();
}
void StellarKV::Tick() {

  for (;;std::this_thread::sleep_for(std::chrono::milliseconds(10))){
    {
      lock_guard<mutex> lock(mtx);
      auto p = node->View(slot);
      if (p.second) {
        shared_ptr<PutMessage> m;
        std::istringstream ss;
        ss.str(p.first);
        {
          cereal::JSONInputArchive archive(ss);
          archive(m);
        }
        m->apply(&log);
        slot++;
      }
    }
    
  }
}
pair<pair<Version,string>, bool> StellarKV::Get(string k){
  lock_guard<mutex> lock(mtx);
  try {
    return pair<pair<Version,string>, bool>(log.at(k), true);
  } catch (out_of_range){
    auto s =pair<Version, string>(0,"");
    return pair<pair<Version,string>, bool>(s, false);

  }
}
void StellarKV::Put(string k, string val){
  lock_guard<mutex> lock(mtx);
  Version v;
  try {
    v = log.at(k).first;
  } catch (out_of_range){
    v = 0;
  }
  ostringstream ss;
  {
    cereal::JSONOutputArchive archive(ss);
    auto putmessage = make_shared<PutMessage>(k,val,v+1);
    archive(CEREAL_NVP(putmessage));
  }
  node->Propose(ss.str());
  return;
}
int StellarKV::GetThreshold(){
  return node->GetThreshold();
}
void StellarKV::AddPeer(NodeID n) {
  node->AddNodeToQuorum(n);
  node->SetThreshold(node->QuorumSize() * threshold_ratio);
}
void StellarKV::AddPeers(set<NodeID> nodes){

  for (auto n : nodes) {
    AddPeer(n);
  }
}

void StellarKV::RemovePeer(NodeID n) {
  node->RemoveNodeFromQuorum(n);
  node->SetThreshold(node->QuorumSize() * threshold_ratio);
}


void StellarKV::RemovePeers(set<NodeID> nodes) {
  for (auto n : nodes) {
    RemovePeer(n);
  }
}
