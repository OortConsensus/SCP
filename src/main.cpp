#include <iostream>

#include "transport.hpp"
#include "transportDummyImpl.hpp"
#include "message.hpp"
#include "quorum.hpp"
#include "node.hpp"

using namespace DISTPROJ;

int main(int argc, char **argv) {

  // Create transport layer.
  FakeRPCLayer rpc = FakeRPCLayer();

  // Create nodes.
  LocalNode n1 = LocalNode(1, rpc);
  LocalNode n2 = LocalNode(2, rpc);
  LocalNode n3 = LocalNode(3, rpc);
  LocalNode n4 = LocalNode(4, rpc);
  LocalNode n5 = LocalNode(5, rpc);

  // Print IDs.
  std::cout << n1.GetNodeID() << "\n";
  std::cout << n2.GetNodeID() << "\n";
  std::cout << n3.GetNodeID() << "\n";
  std::cout << n4.GetNodeID() << "\n";
  std::cout << n5.GetNodeID() << "\n";

  // Create quorum sets.
  Quorum qs;
  qs.threshold = 3;
  qs.members = std::set<NodeID> {1, 2, 3, 4, 5};

  // Add quorum sets to nodes.
  n1.UpdateQurorum(qs);
  n2.UpdateQurorum(qs);
  n3.UpdateQurorum(qs);
  n4.UpdateQurorum(qs);
  n5.UpdateQurorum(qs);

  // Print a nodes quorum set threshold.
  n1.PrintQuorumSet();

  // Update the quorum set.
  LocalNode n6 = LocalNode(6, rpc);
  n1.AddNodeToQuorum(n6.GetNodeID());
  n1.PrintQuorumSet();

  // Make a sample message.
  //PrepareMessage samplePrepareMsg = PrepareMessage(n1.GetNodeID(),0, );






  
  std::cout << "Implementation coming soon\n";

  return 0;
}
