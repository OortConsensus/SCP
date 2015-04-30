#ifndef NODE_H
#define NODE_H

#include <set>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include "quorum.h"

typedef uint64_t NodeID;

namespace DISTPROJ {

  class Slot;

  class Node {

    public:
      Node(uint64_t id, Quorum quorumSet);

      uint64_t getNodeID() { 
        return id; 
      };

      Quorum getQuorumSet() {
        return quorumSet;
      }

    protected:
      const uint64_t id;
      Quorum quorumSet;

  };

  template <class T>
  class LocalNode : public Node {

    public:
      LocalNode(uint64_t id, Quorum quorumSet) : Node(id, quorumSet) {}; 

      void AddKnownNode(Node node);
      void UpdateQurorum(Quorum quorumSet);

      void Tick();
      void SendMessage(T msg);
      void ProcessMessage(T msg);

      void DumpLog();

    private:
      std::vector<Slot> log;
      std::set<NodeID> knownNodes;

  };

}

#endif
