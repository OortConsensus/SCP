
#ifndef TRANSPORT_H
#define TRANSPORT_H


namespace DISTPROJ {
  class Message;
  class Node;

class RPCLayer {

  public:
    virtual void Send(Message& msg, uint64_t peerID);
    virtual bool Recieve(Message& msg);    
    virtual void Broadcast(Message& msg);

    virtual void AddNode(Node& node);

};

}

#endif
