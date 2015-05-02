#ifndef QUORUM_H
#define QUORUM_H

#include <set>

#include "common.hpp"

namespace DISTPROJ {

  struct Quorum {
    int threshold;
    std::set<NodeID> members;
  };

}

#endif

