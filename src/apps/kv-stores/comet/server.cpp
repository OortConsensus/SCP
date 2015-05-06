#include "server.hpp"
#include "misc.hpp"

#include <random>

using namespace DISTPROJ;
using namespace DISTPROJ::Application::KVStellar;

ServerKV::ServerKV(std::shared_ptr<RPCLayer> rpc, float _quorumThresholdRatio) 
  : quorumThresholdRatio(_quorumThresholdRatio) {
  // Generate node id.
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<NodeID> dist(0, ~0);

  node = new LocalNode(dist(gen), *rpc, Quorum{});
  curSlot = 0;

  node->Start();
}

void ServerKV::Put(PutArgs &args, PutReply &reply) {
  std::lock_guard<std::mutex> lock(mtx);

  Operation op;
  op.id = args.id;
  op.opType = PUT;
  op.key = args.key;
  op.value = args.value;

  ApplyOperation(op);
  reply.err = "OK";
}
  
void ServerKV::Get(GetArgs &args, GetReply &reply) {
  std::lock_guard<std::mutex> lock(mtx);

  Operation op;
  op.id = args.id;
  op.opType = GET;
  op.key = args.key;

  reply.value = ApplyOperation(op);
  reply.err = "OK";
}

std::string ServerKV::ApplyOperation(Operation &op) {
  while(1) {
    // Check if we've seen this operation.
    {
      std::map<OpID, std::string>::iterator it = seen.find(op.id);
      if (it != seen.end()) {
        return it->second;
      }
    }

    Operation decidedOp;
    auto newSlot = curSlot + 1;
    auto p = node->View(newSlot);
    if (p.second) {
      std::istringstream ss;
      ss.str(p.first);
      {
        cereal::JSONInputArchive archive(ss);
        archive(decidedOp);
      }
      printf("BAM key %s val %s\n", decidedOp.key.c_str(), decidedOp.value.c_str());
    } else {
      // Propose.
      std::ostringstream ss;
      {
        cereal::JSONOutputArchive archive(ss);
        archive(op);
      }
      node->Propose(ss.str(), newSlot);

      // Wait for a decision.
      printf("about to read decision.\n");
      while (!p.second) {
        p = node->View(newSlot);
      }
      std::istringstream iss;
      iss.str(p.first);
      {
        cereal::JSONInputArchive archive(iss);
        archive(decidedOp);
      }
      printf("BAM key %s val %s\n", decidedOp.key.c_str(), decidedOp.value.c_str());
    }

    // Apply operation.
    std::string result = "";
    switch (decidedOp.opType) {
    case GET:
      seen[decidedOp.id] = db[decidedOp.key];
      break;
    case PUT:
      seen[decidedOp.id] = "";
      db[decidedOp.key] = decidedOp.value;
      break;
    }
    curSlot++;

    // Check termination condition.
    if (decidedOp.id == op.id) {
      return seen[decidedOp.id];
    }
  }
}

void ServerKV::AddPeer(NodeID peer) {
  node->AddNodeToQuorum(peer);
  ResizeThreshold();
}

void ServerKV::AddPeers(std::set<NodeID> peers){
  for (auto n : peers) {
    AddPeer(n);
  }
}

void ServerKV::RemovePeer(NodeID peer) {
  node->RemoveNodeFromQuorum(peer);
  ResizeThreshold();
}

void ServerKV::RemovePeers(std::set<NodeID> peers) {
  for (auto n : peers) {
    RemovePeer(n);
  }
}
