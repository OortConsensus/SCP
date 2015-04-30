#ifndef NODE_H
#define NODE_H

#include <set>
#include <cstdlib>

namespace DISTPROJ {

  class Node {

    std::set<uint64_t> knownNodes;

    

    public:
    Node(uint64_t id);


      /*
       * TODO:
       * Methods:
       * JHH -- I think we need these.
       *
       * checkForQuorum -- given a set of nodes determine if we have quorum.
       * checkForVBlocking -- given a set of nodes determine if they are a
       *                      v-blocking set for this node
       * getQuorumSet -- given a quorumsethash return the quorum set.
       * putQuorumSet -- store a quorum set.
       */
    uint64_t getNodeID() { return id; }; // Inline function

    protected:
      const uint64_t id; // My node ID.
      // TODO: Need a cache to map sethashes --> quorumSets

  };

  class LocalNode : public Node {

    public:
      LocalNode(); // TODO: Take a quorum set and a SCP ptr.

      /*
       * TODO
       * Methods:
       * JHH -- I think we need these.
       *
       * getQuorumSetHash -- return the hash of the quorum set.
       *
       */

    private:
      // TODO: Quorum set and its hash should be private.

  };

}

#endif
