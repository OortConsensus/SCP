
#ifndef TRANSPORTDUMMY_H
#define TRANSPORTDUMMY_H

#include <cstdlib>
#include <vector>
#include <map>
#include "queue.h"
#include "transport.h"

namespace DISTPROJ {

  class Node;

  class Message;
  class FakeRPCLayer : public RPCLayer {
    std::map<uint64_t, Queue<Message&>> messageQueues;

    class MessageClient {

      uint64_t id;

    public:
      MessageClient(uint64_t id);

      void Send(Message& msg, uint64_t peerID);
      bool Recieve(Message& msg);    
      void Broadcast(Message& msg);

      void AddNode(Node& node);
    };

  public:
    FakeRPCLayer();
    MessageClient GetClient(uint64_t id);

  };

}

#endif
