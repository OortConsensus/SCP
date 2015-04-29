
#ifndef TRANSPORTDUMMY_H
#define TRANSPORTDUMMY_H

#include <cstdlib>
#include <vector>
#include <map>
#include "queue.h"

namespace DISTPROJ {

class Node;

template <class T>
class FakeRPCLayer {

  std::map<uint64_t, Queue<T>> messageQueues;

  class MessageClient {

    uint64_t id;

    public:
      MessageClient(uint64_t id);

      void Send(T msg, uint64_t peerID);
      bool Recieve(T* msg);    
      void Broadcast(T msg);
  };

  public:
    FakeRPCLayer(std::vector<Node> nodes);
    MessageClient GetClient(uint64_t id);

};

}

#endif
