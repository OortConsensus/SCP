#ifndef STELLAR_KV_H
#define STELLAR_KV_H
#include "node.hpp"
#include <string>
#include <thread>
#include <set>
#include <mutex>
#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"
namespace DISTPROJ{
  class LocalNode;
  class RPCLayer;
  namespace Application {
    
    namespace StellarKV{

      class ActionMessage {
      public:
        ActionMessage(){};
        virtual void apply(std::map<std::string, std::pair<Version,std::string>>* log) {};
      };
      class PutMessage : public ActionMessage{
        std::string Key;
        std::string Value;
        Version v;
      public:
        PutMessage() : PutMessage("","",0){};
        PutMessage(std::string k, std::string v, Version ver) : Key(k), Value(v), v(ver){};
        void apply(std::map<std::string, std::pair<Version,std::string>>* log) {
          try {
            Version v_ = (*log).at(Key).first;
            if (v_ < v )  {
              (*log)[Key] = std::pair<Version, std::string>(v,Value);
            }
          } catch (std::out_of_range){
            (*log)[Key] = std::pair<Version, std::string>(v,Value);
          }
        };
        template<class Archive>
        void serialize(Archive & archive) {
          archive(CEREAL_NVP(Key), CEREAL_NVP(Value));
        };
      };
       
      class StellarKV {
        LocalNode * node;
        std::map<std::string, std::pair<Version, std::string>> log;
        std::thread * t;
        std::mutex mtx;
        unsigned int slot;
        void Tick();
        float threshold_ratio;

      public:
        StellarKV(std::shared_ptr<RPCLayer> rpc, float tr);
        std::pair<std::pair<Version,std::string>,bool> Get(std::string k);
        void Put(std::string k, std::string v);
        NodeID GetNodeID();
        int GetThreshold();
        void AddPeer(NodeID node);
        void RemovePeer(NodeID node);
        void AddPeers(std::set<NodeID> node);
        void RemovePeers(std::set<NodeID> node);

      };
    }

  }
}
CEREAL_REGISTER_TYPE_WITH_NAME(DISTPROJ::Application::StellarKV::PutMessage, "PutMessage");
#endif


