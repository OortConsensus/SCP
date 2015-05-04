#ifndef QUORUM_H
#define QUORUM_H

#include <set>

#include "common.hpp"
#include <cereal/types/set.hpp>

namespace DISTPROJ {

  struct Quorum {
    int threshold; // important to be int, not uint (we compare it to -1 in slot)
    std::set<NodeID> members;
	template<class Archive>
	void serialize(Archive & archive) {
	  archive(threshold, members ); // serialize things by passing them to the archive
	};
  };

}

#endif

