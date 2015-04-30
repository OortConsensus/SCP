
#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "message.h"

namespace DISTPROJ {

class RPCLayer {

  public:
    virtual void Send(Message msg, uint64_t peerID);
    virtual bool Recieve(Message* msg);    
    virtual void Broadcast(Message msg);

    virtual void AddNode(NodeID node);

};

}

#endif
