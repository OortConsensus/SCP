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
pair<string, bool> StellarKV::Get(string k){
  lock_guard<mutex> lock(mtx);
  try {
    return pair<string, bool>(log.at(k), true);
  } catch (out_of_range){

    return pair<string, bool>("", false);
  }
}
void StellarKV::Put(string k, string v){
  lock_guard<mutex> lock(mtx);
  std::ostringstream ss;
  {
    cereal::JSONOutputArchive archive(ss);
    archive(CEREAL_NVP(PutMessage(k,v)));
  }
  node->Propose(ss.str());
}

