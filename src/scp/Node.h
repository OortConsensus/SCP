#ifndef NODE_H
#define NODE_H

namespace DISTPROJ {

  class Node {
    
    public:
      Node(uint64_t const& ID, SCP* SCP);


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

    protected:
      const uint64_t ME; // My node ID.
      SCP* mySCP;
      // TODO: Need a cache to map sethashes --> quorumSets

  }

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

  }

}

#endif
