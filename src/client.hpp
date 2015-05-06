#ifndef STELLAR_CLIENT_KV_H
#define STELLAR_CLIENT_KV_H

#include <string>
#include <random>

#include "node.hpp"
#include "common.hpp"
#include "misc.hpp"

namespace DISTPROJ {
  
  class LocalNode;
  class RPCLayer;

  namespace Application {
    
    namespace KVStellar {

      class ServerKV;
      
      class ClientKV {

        std::shared_ptr<ServerKV> server;
        std::string name;

        public:

        ClientKV(std::shared_ptr<ServerKV> svr, std::string nm);

        void Put(std::string key, std::string value);
        std::string Get(std::string key);

        // TODO : Move to a common header.
        std::string nrand() {
          // Generate node id.
          std::random_device rd;
          std::mt19937 gen(rd());
          std::uniform_int_distribution<NodeID> dist(0, ~0);

          std::ostringstream os;
          os << dist(gen);
          return os.str();
        }

      };
      
    }

  }
}

#endif
