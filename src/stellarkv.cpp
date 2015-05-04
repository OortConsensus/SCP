#include "stellarkv.hpp"
#include <random>

using namespace DISTPROJ;
using namespace DISTPROJ::Application::StellarKV;
using namespace std;

StellarKV::StellarKV(RPCLayer* rpc){
  std::random_device rd ;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<NodeID> dist(0, ~0);
  node = new LocalNode(dist(gen), *rpc, Quorum{});
  slot = 0;
  t = new std::thread(&StellarKV::Tick, this);
}
void StellarKV::Tick() {

  std::istringstream ss;
  for (;;std::this_thread::sleep_for(std::chrono::seconds(1))){
    lock_guard<mutex> lock(mtx);
    auto p = node->View(slot);
    if (p.second) {
      shared_ptr<PutMessage> m;
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
  std::ostringstream ss;
  Version v;
  try {
    v = log.at(k).first;
  } catch (out_of_range){
    v = 0;
  }
  {
    cereal::JSONOutputArchive archive(ss);
    archive(CEREAL_NVP(PutMessage(k,val,v+1)));
  }
  node->Propose(ss.str());
}
