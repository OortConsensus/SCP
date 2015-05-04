#ifndef STELLAR_KV_H
#define STELLAR_KV_H
#include "node.hpp"
#include <string>
#include <thread>
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
        virtual void apply(std::map<std::string, std::string>* log) {};
      };
      class PutMessage : public ActionMessage{
        std::string Key;
        std::string Value;
      public:
        PutMessage() : PutMessage("",""){};
        PutMessage(std::string k, std::string v) : Key(k), Value(v){};
        void apply(std::map<std::string, std::string>* log) {(*log)[Key] = Value;};
        template<class Archive>
        void serialize(Archive & archive) {
          archive(CEREAL_NVP(Key), CEREAL_NVP(Value));
        };
      };
       
      class StellarKV {
        LocalNode * node;
        std::map<std::string, std::string> log;
        std::thread * t;
        std::mutex mtx;
        unsigned int slot;
        void Tick();

      public:
        StellarKV(RPCLayer * rpc);
        std::pair<std::string,bool> Get(std::string k);
        void Put(std::string k, std::string v);

      };
    }

  }
}
CEREAL_REGISTER_TYPE_WITH_NAME(DISTPROJ::Application::StellarKV::PutMessage, "PutMessage");
#endif


