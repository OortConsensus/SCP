#ifndef NODE_H
#define NODE_H

namespace DISTPROJ {

  class Node {
    public:
    Node(uint64_t const& id, SCP* scp); // (CR: JR->JH, why are you passing ID by reference rather than by value?)


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
       * getNodeID -- return node's ME.
       */
    uint64_t getNodeID(){return id}; // Inline function

    protected:
      const uint64_t id; // My node ID.
      SCP* scp;
      // TODO: Need a cache to map sethashes --> quorumSets

  }

  class LocalNode : public Node { // (CR: JR->JH, what is a localnode for?)

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

  }

}

#endif
