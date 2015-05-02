#include <iostream>

#include "transport.hpp"
#include "transportDummyImpl.hpp"
#include "message.hpp"
#include "quorum.hpp"
#include "node.hpp"
#include <array>

using namespace DISTPROJ;
const int N = 6;
std::array<LocalNode*, N> nodes;

int main(int argc, char **argv) {

  // Create transport layer.
  FakeRPCLayer rpc = FakeRPCLayer();

  // Create nodes.
  for (auto i =0; i < N-1; i++)
    nodes[i] = new LocalNode(i, rpc);

  // Print IDs.
  for (auto i=0; i < N-1; i++)
    std::cout << nodes[i]->GetNodeID() << "\n";

  // Create quorum sets.
  Quorum qs;
  qs.threshold = 3;
  qs.members = std::set<NodeID> {0, 1, 2, 3, 4};

  // Add quorum sets to nodes.
  for (auto i=0; i < N-1; i++)
    nodes[i]->UpdateQurorum(qs);

  // Print a nodes quorum set threshold.
  nodes[1]->PrintQuorumSet();

  // Update the quorum set.
  nodes[5]  = new LocalNode(5, rpc);
  nodes[1]->AddNodeToQuorum(nodes[5]->GetNodeID());
  nodes[1]->PrintQuorumSet();

  // Make a sample message.
  //PrepareMessage samplePrepareMsg = PrepareMessage(n1.GetNodeID(),0, );






  
  std::cout << "Implementation coming soon\n";

  return 0;
}
