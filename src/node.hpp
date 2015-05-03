#ifndef NODE_H
#define NODE_H

#include <set>
#include <map>
#include <cstdlib>
#include <cstdint>
#include <thread>

#include "common.hpp"
#include "quorum.hpp"

namespace DISTPROJ {

  class RPCLayer;
  class Message;
  class Slot;
  class MessageClient;

  class Node {

  public:
    Node(NodeID _id, RPCLayer& _rpc);
    Node(NodeID _id, RPCLayer& _rpc, Quorum _quorumSet);

    NodeID GetNodeID();
    Quorum GetQuorumSet();
    void PrintQuorumSet();

  protected:
    NodeID id;
    RPCLayer& rpc;
    Quorum quorumSet;
    std::thread * t;

  };

  class LocalNode : public Node {

  public:
    LocalNode(NodeID _id, RPCLayer& _rpc);
    LocalNode(NodeID _id, RPCLayer& _rpc, Quorum _quorumSet) ;

    void AddKnownNode(NodeID v);
    void AddNodeToQuorum(NodeID v);
    void UpdateQurorum(Quorum quorumSet);

    void Start();
    void SendMessage(Message* msg);
    bool ReceiveMessage(Message ** msg);
    void ProcessMessage(Message* msg);

    void DumpLog();

  private:
    void Tick();
    std::map<unsigned int, Slot*> log;
    std::set<NodeID> knownNodes;
    MessageClient * mc;

  };

}

#endif
