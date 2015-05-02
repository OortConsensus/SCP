
#ifndef TRANSPORTDUMMY_H
#define TRANSPORTDUMMY_H

#include <cstdlib>
#include <vector>
#include <map>

#include "common.hpp"
#include "queue.hpp"
#include "transport.hpp"

namespace DISTPROJ {

  class Node;

  class Message;
  class MessageClient;

  class FakeRPCLayer : public RPCLayer {
    std::map<NodeID, Queue<Message*>*> messageQueues;

    friend MessageClient;

  public:
    FakeRPCLayer();
    MessageClient* GetClient(NodeID id);
    void AddNode(NodeID node);

    void Send(Message* msg, NodeID id, NodeID peerID);
    bool Receive(Message* msg, NodeID id);    
    void Broadcast(Message* msg, NodeID id);
  };

  class MessageClient{
    NodeID id;
    RPCLayer* rpc;
  public:
    MessageClient(NodeID id, RPCLayer* r);
    void Send(Message* msg, NodeID peerID);
    bool Receive(Message* msg);    
    void Broadcast(Message* msg);
  };

}

#endif
