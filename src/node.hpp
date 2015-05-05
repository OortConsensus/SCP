#ifndef NODE_H
#define NODE_H

#include <set>
#include <map>
#include <cstdlib>
#include <cstdint>
#include <thread>
#include <mutex>

#include "common.hpp"
#include "quorum.hpp"

namespace DISTPROJ {

  class RPCLayer;
  class Message;
  class Slot;
  class MessageClient;

  class Node {

  public:
    Node(NodeID _id, RPCLayer& _rpc);
    Node(NodeID _id, RPCLayer& _rpc, Quorum _quorumSet);

    NodeID GetNodeID();
    Quorum GetQuorumSet();
    void PrintQuorumSet();

  protected:
    NodeID id;
    RPCLayer& rpc;
    Quorum quorumSet;
    std::thread * t;
    friend Slot;

  };

  class LocalNode : public Node {

  public:
    LocalNode(NodeID _id, RPCLayer& _rpc);
    LocalNode(NodeID _id, RPCLayer& _rpc, Quorum _quorumSet) ;

    void AddKnownNode(NodeID v);
    void AddNodeToQuorum(NodeID v);
    void RemoveNodeFromQuorum(NodeID v);
    void UpdateQurorum(Quorum quorumSet);

    int QuorumSize();
    void SetThreshold(int t);
    int GetThreshold();

    void Start();
    SlotNum Propose(std::string value);
    void SendMessage(std::shared_ptr<Message> msg);
    void SendMessageTo(std::shared_ptr<Message> msg, NodeID i);
    bool ReceiveMessage(std::shared_ptr<Message>* msg);
    void ProcessMessage(std::shared_ptr<Message> msg);
    void DumpLog();
    std::pair<std::string, bool> View(SlotNum s);;

  private:
    SlotNum maxSlot;
    std::mutex mtx;
    SlotNum NewSlot(); // only one of these can run at a time
    void Tick();
    std::map<SlotNum, std::shared_ptr<Slot>> log;
    std::set<NodeID> knownNodes;
    MessageClient * mc;

  };

}

#endif
