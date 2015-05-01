#ifndef NODE_H
#define NODE_H

#include <set>
#include <map>
#include <cstdlib>
#include <cstdint>

typedef uint64_t NodeID;

namespace DISTPROJ {

  class Quorum;
  class Slot;
  class RPCLayer;
  class Message;

  class Node {

    public:
      Node(NodeID _id, const RPCLayer& _rpc);
      Node(NodeID _id, const RPCLayer& _rpc, Quorum& _quorumSet);

      NodeID getNodeID() { 
        return id; 
      };

      Quorum& getQuorumSet() {
        return quorumSet;
      }

    protected:
      const NodeID id;
      const RPCLayer& rpc;
      Quorum& quorumSet;

  };

  class LocalNode : public Node {

    public:
      LocalNode(NodeID _id, const RPCLayer& _rpc) 
        : Node(_id, _rpc) {};
      LocalNode(NodeID _id, const RPCLayer& _rpc, Quorum& _quorumSet) 
        : Node(_id, _rpc, _quorumSet) {}; 

      void AddKnownNode(NodeID v);
      void UpdateQurorum(Quorum& quorumSet);

      void Tick();
      void SendMessage(Message& msg);
      void ProcessMessage(Message& msg);

      void DumpLog();

    private:
      std::map<unsigned int, Slot> log;
      std::set<NodeID> knownNodes;

  };

}

#endif
