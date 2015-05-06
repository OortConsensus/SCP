
#ifndef FAKE_RPC_H
#define FAKE_RPC_H

#include <cstdlib>
#include <vector>
#include <map>
#include <memory>

#include <set>

#include "common.hpp"
#include "queue.hpp"
#include "RPC.hpp"

namespace DISTPROJ {

  class Node;

  class Message;
  class MessageClient;

  class FakeRPCLayer : public RPCLayer {
    std::map<NodeID, Queue<std::string>*> messageQueues;

    friend MessageClient;

  public:
    FakeRPCLayer();
    MessageClient* GetClient(NodeID id);
    void AddNode(NodeID node);

    void Send(std::shared_ptr<Message> msg, NodeID id, NodeID peerID);
    bool Receive(std::shared_ptr<Message>* msg, NodeID id);    
    void Broadcast(std::shared_ptr<Message> msg, NodeID id, std::set<NodeID> peers);
  };

  class MessageClient{
    NodeID id;
    RPCLayer* rpc;
  public:
    MessageClient(NodeID id, RPCLayer* r);
    void Send(std::shared_ptr<Message> msg, NodeID peerID);
    bool Receive(std::shared_ptr<Message>* msg);    
    void Broadcast(std::shared_ptr<Message> msg, std::set<NodeID> peers);
    NodeID GetNodeID(){return id;};
  };

}

#endif
