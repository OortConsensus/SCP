#ifndef RPC_H
#define RPC_H

#include <set>

#include "common.hpp"

namespace DISTPROJ {
  class Message;
  class Node;
  class MessageClient;

  class RPCLayer {
  public:
    virtual void Send(std::shared_ptr<Message> msg, NodeID id,  NodeID peerID)=0;
    virtual bool Receive(std::shared_ptr<Message>* msg, NodeID id)=0;    
    virtual void Broadcast(std::shared_ptr<Message> msg, NodeID id, std::set<NodeID> peers)=0;
    virtual MessageClient* GetClient(NodeID id)=0;
    virtual void AddNode(NodeID node)=0;
  };
}

#endif
