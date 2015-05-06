#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <fstream>
#include <array>

#include "RPC.hpp"
#include "fakeRPC.hpp"
#include "node.hpp"
#include "server.hpp"
#include "client.hpp"

using namespace DISTPROJ;
using namespace DISTPROJ::Application::KVStellar;

const int Server_N = 6;
const int Client_N = 2;

int main(int argc, char *argv[]) {
  // Create transport layer.
  std::shared_ptr<FakeRPCLayer> rpc = std::make_shared<FakeRPCLayer>();

  // Create servers and quorum sets.
  std::array<std::shared_ptr<ServerKV>, Server_N> servers;
  for (auto i =0; i < Server_N; i++)
    servers[i] = std::make_shared<ServerKV>(rpc, 0.8);
  std::set<NodeID> s;
  for (auto i =0; i < Server_N; i++)
    s.insert(servers[i]->GetNodeID());
  for (auto i =0; i < Server_N; i++)
    servers[i]->AddPeers(s);

  // Create client group 0 -- talk to server 0.
  std::array<ClientKV*, Client_N> clients0;
  for (auto i=0; i < Client_N; i++)
    clients0[i] = new ClientKV(servers[0], "");

  // Create client group 1 -- talk to server 1.
  std::array<ClientKV*, Client_N> clients1;
  for (auto i=0; i < Client_N; i++)
    clients1[i] = new ClientKV(servers[1], "");

  // Basic tests.
  printf("Runnning basic tests on comet key-value store.\n");

  // Test put on one client and get on another (both talk to the same server.).
  clients0[0]->Put("1", "Test");
  if (clients0[1]->Get("1") == "Test") {
    printf("Test PASSED\n");
  } else {
    printf("Test FAILED\n");
  }

  // Test get on client talking to another server.
  if (clients1[0]->Get("1") == "Test") {
    printf("Test PASSED\n");
  } else {
    printf("Test FAILED\n");
  }

  return 0;
}

