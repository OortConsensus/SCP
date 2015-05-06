#include "client.hpp"
#include "server.hpp"
#include "misc.hpp"

using namespace DISTPROJ;
using namespace DISTPROJ::Application::KVStellar;
        
ClientKV::ClientKV(std::shared_ptr<ServerKV> svr, std::string nm) 
  : server(svr), name(nm) {}

void ClientKV::Put(std::string key, std::string value) {
  PutArgs args;
  args.id = nrand();
  args.key = key;
  args.value = value;

  PutReply reply;

  // TODO : Do over rpc and check reply for errors.
  server->Put(args, reply);
}

std::string ClientKV::Get(std::string key) {
  GetArgs args;
  args.id = nrand();
  args.key = key;

  GetReply reply;

  // TODO : Do over rpc and check reply for errors.
  server->Get(args, reply);

  return reply.value;
}
