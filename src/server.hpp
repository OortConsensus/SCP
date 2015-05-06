#ifndef STELLAR_SERVER_KV_H
#define STELLAR_SERVER_KV_H

#include <string>
#include <set>
#include <mutex>
#include <memory>

#include "node.hpp"
#include "common.hpp"
#include "misc.hpp"
#include "cereal/cereal.hpp"
#include "cereal/types/string.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"

namespace DISTPROJ {
  
  class LocalNode;
  class RPCLayer;

  namespace Application {
    
    namespace KVStellar {
      
      class ServerKV {

        std::mutex mtx;
        LocalNode *node;
        float quorumThresholdRatio;

        std::map<std::string, std::string> db; // key --> value.
        std::map<OpID, std::string> seen; // txID --> result. Used for at most once semantics.
        SlotNum curSlot;

        public:

        ServerKV(std::shared_ptr<RPCLayer> rpc, float _quorumThresholdRatio);

        std::string ApplyOperation(Operation &op);
        void Put(PutArgs &args, PutReply &reply);
        void Get(GetArgs &args, GetReply &reply);

        NodeID GetNodeID() { return node->GetNodeID(); };
        int GetThreshold() {return node->GetThreshold(); };

        void AddPeer(NodeID peer);
        void AddPeers(std::set<NodeID> peers);
        void RemovePeer(NodeID peer);
        void RemovePeers(std::set<NodeID> peers);
        void ResizeThreshold() { 
          node->SetThreshold(node->QuorumSize() * quorumThresholdRatio); 
        };

      };
      
    }

  }
}

#endif

