
#ifndef TRANSPORT_H
#define TRANSPORT_H
#include "types.hpp"

namespace DISTPROJ {
  class Message;
  class Node;

class RPCLayer {

  public:
  RPCLayer();
  virtual void Send(Message& msg, uint64_t id,  uint64_t peerID);
  virtual bool Recieve(Message& msg, uint64_t id);    
  virtual void Broadcast(Message& msg, uint64_t id);

    virtual void AddNode(NodeID node);

};

}

#endif
