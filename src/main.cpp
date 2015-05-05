#include <iostream>
#include <array>

#include "RPC.hpp"
#include "fakeRPC.hpp"
#include "message.hpp"
#include "quorum.hpp"
#include "node.hpp"
#include "ballot.hpp"
#include <chrono>
#include <thread>
#include <fstream>
#include "stellarkv.hpp"

using namespace DISTPROJ;
using namespace DISTPROJ::Application::StellarKV;
using namespace std;
const int N = 6;

int main(int argc, char *argv[]) {
  std::array<shared_ptr<StellarKV>, N> nodes;
  // Create transport layer.
  shared_ptr<FakeRPCLayer> rpc = make_shared<FakeRPCLayer>();

  // Create nodes.
  for (auto i =0; i < N; i++)
    nodes[i] = make_shared<StellarKV>(rpc, 0.8);
  set<NodeID> s;
  for (auto i =0; i < N; i++)
    s.insert(nodes[i]->GetNodeID());

  for (auto i =0; i < N; i++) {
    nodes[i]->AddPeers(s);
  }
  printf("Need to get %i nodes to agree out of %i nodes\n",nodes[0]->GetThreshold(), N);
  nodes[0]->Put("test", "MESSAGE");
  for (;;this_thread::sleep_for(chrono::seconds(1))){
    auto count = N;
    for (auto n : nodes) {
      auto r = n->Get("test");
      if (r.second){
        printf("Key was set to (%s) on %llu\n",r.first.second.c_str(), n->GetNodeID());
        count--;
      }
    }
    if (count == 0) {
      break;
    }
  }

  //nodes[0]->Put("test2", "MESSAGE2");
  nodes[1]->Put("test2", "MESSAGE2");
  for (;;this_thread::sleep_for(chrono::seconds(1))){
    auto count = N;
    for (auto n : nodes) {
      auto r = n->Get("test2");
      if (r.second){
        printf("Key was set to (%s) on %llu\n",r.first.second.c_str(), n->GetNodeID());
        count--;
      }
    }
    if (count == 0) {
      break;
    }
  }


  
  nodes[0]->Put("test3", "MESSAGE3.1");
  nodes[1]->Put("test3", "MESSAGE3.2");
  for (;;this_thread::sleep_for(chrono::seconds(1))){
    auto count = N;
    for (auto n : nodes) {
      auto r = n->Get("test3");
      if (r.second){
        printf("Key was set to (%s) on %llu\n",r.first.second.c_str(), n->GetNodeID());
        count--;
      }
    }
    if (count == 0) {
      break;
    }
  }
  printf("COMPLETE\n");
  return 0;
}





// int main0(int argc, char *argv[]) {
//   std::array<shared_ptr<StellarKV>, N> nodes;
//   printf("%s %s \n", argv[1], argv[0]);
//   if (!argv[1] || !argv[2]) // Must provide a read/write fifo
//     exit(-1);
//   std::ofstream ofs (argv[2], std::ofstream::out);
//   ofs << "lorem ipsum\n";
//   ofs.close();
//   std::ifstream ifs (argv[1], std::ifstream::in);
//   std::cout << "GOT:"<<ifs.get() <<"\n";
//   ifs.close();
//   printf("passed fstreams\n");
//   // Create transport layer.
//   FakeRPCLayer rpc = FakeRPCLayer();

//   // Create nodes.
//   for (auto i =0; i < N-1; i++)
//     nodes[i] = new LocalNode(i, rpc);

//   // Print IDs.
//   for (auto i=0; i < N-1; i++)
//     std::cout << nodes[i]->GetNodeID() << "\n";

//   // Create quorum sets.
//   Quorum qs;
//   qs.threshold = 3;
//   qs.members = std::set<NodeID> {0, 1, 2, 3, 4};

//   // Add quorum sets to nodes.
//   for (auto i=0; i < N-1; i++)
//     nodes[i]->UpdateQurorum(qs);

//   // Print a nodes quorum set threshold.
//   nodes[1]->PrintQuorumSet();

//   // Update the quorum set.
//   nodes[5]  = new LocalNode(5, rpc);
//   nodes[1]->AddNodeToQuorum(nodes[5]->GetNodeID());
//   nodes[1]->PrintQuorumSet();

//   // Make a sample message.
//   Ballot dummyBallot;
//   dummyBallot.num = 1;
//   dummyBallot.value = "";

//   // auto samplePrepareMsg = std::make_shared<PrepareMessage>(
//   //     nodes[0]->GetNodeID(),
//   //     0,
//   //     dummyBallot,
//   //     dummyBallot,
//   //     dummyBallot,
//   //     dummyBallot,
//   //     nodes[0]->GetQuorumSet());

//   // nodes[0]->SendMessage(samplePrepareMsg);
//   nodes[0]->Propose("hello");
//   std::this_thread::yield();
//   std::this_thread::sleep_for(std::chrono::seconds(1));
//   // printf("sending duplicate message\n");
//   // nodes[0]->SendMessage(samplePrepareMsg);

//   // nodes[1]->ReceiveMessage();
  








//   for (auto i=0; i < N; i++)
//     nodes[i]->Start();





//   printf("Implementation coming soon\n");
//   while (true){


//   }

//   return 0;
// }
