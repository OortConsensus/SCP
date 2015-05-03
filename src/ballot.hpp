#ifndef BALLOT_H
#define BALLOT_H

#include <cstdio>
#include <iostream>
#include <sstream>  


 
namespace DISTPROJ {

  struct Ballot {
    uint32_t num;
    std::string value;
	template<class Archive>
	void serialize(Archive & archive) {
	  archive(num, value ); // serialize things by passing them to the archive
	};
  };

  inline bool operator==(const Ballot& lhs, const Ballot& rhs) {
    return lhs.num == rhs.num && lhs.value == rhs.value;
  }
  inline bool operator!=(const Ballot& lhs, const Ballot& rhs) {
    return !(lhs == rhs);
  }
  inline bool compatible (const Ballot& lhs, const Ballot& rhs) {
    return lhs.value == rhs.value;
  }

  inline bool incompatible (const Ballot& lhs, const Ballot& rhs) {
    return lhs.value != rhs.value;
  }
  inline bool operator< (const Ballot& lhs, const Ballot& rhs) {
    return incompatible(lhs, rhs) && lhs.num < rhs.num;
    
  }

  inline bool operator> (const Ballot& lhs, const Ballot& rhs) {
    return incompatible(lhs, rhs) && lhs.num > rhs.num;
    
  }
}


#endif
