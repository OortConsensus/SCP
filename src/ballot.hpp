#ifndef BALLOT_H
#define BALLOT_H

namespace DISTPROJ {

  struct Ballot {
    int num;
    std::string value;

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
