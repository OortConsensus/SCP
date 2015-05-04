
#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "common.hpp"

namespace DISTPROJ {
  class Message;
  class Node;
  class MessageClient;

  class RPCLayer {

  public:
    virtual void Send(std::shared_ptr<Message> msg, uint64_t id,  uint64_t peerID)=0;
    virtual bool Receive(std::shared_ptr<Message>* msg, uint64_t id)=0;    
    virtual void Broadcast(std::shared_ptr<Message> msg, uint64_t id)=0;
    virtual MessageClient* GetClient(NodeID id)=0;
    virtual void AddNode(NodeID node)=0;

  };

}

#endif
