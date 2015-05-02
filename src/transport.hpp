
#ifndef TRANSPORT_H
#define TRANSPORT_H
#include "types.hpp"

namespace DISTPROJ {
  class Message;
  class Node;

class RPCLayer {

  public:
  virtual void Send(Message* msg, uint64_t id,  uint64_t peerID) = 0;
  virtual bool Recieve(Message* msg, uint64_t id)=0;    
  virtual void Broadcast(Message* msg, uint64_t id)=0;

    virtual void AddNode(NodeID node)=0;

};

}

#endif
