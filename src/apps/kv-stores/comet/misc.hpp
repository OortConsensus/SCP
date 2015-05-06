#ifndef STELLAR_KV_MISC_H
#define STELLAR_KV_MISC_H

#include "common.hpp"
#include "cereal/cereal.hpp"
#include "cereal/types/string.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"

#include <string>
#include <sstream>
#include <iostream>

namespace DISTPROJ {

  namespace Application {
    
    namespace KVStellar {

      // TODO : Change to enum.
      typedef std::string Err;

      //enum OpType { GET, PUT };
      typedef std::string OpType;

      struct Operation {
        OpType opType;
        OpID id;
        std::string key;
        std::string value;

        template<class Archive>
        void serialize(Archive & archive) {
          archive(CEREAL_NVP(opType), CEREAL_NVP(id), CEREAL_NVP(key), CEREAL_NVP(value));
        }
      };

      struct GetArgs {
        OpID id;
        std::string key;
      };

      struct GetReply {
        Err err;
        std::string value;
      };

      struct PutArgs {
        OpID id;
        std::string key;
        std::string value;
      };

      struct PutReply {
        Err err;
      };

    }
  }
}

#endif
