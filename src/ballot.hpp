#ifndef BALLOT_H
#define BALLOT_H

#include <cstdio>
#include <iostream>
#include <sstream>  
#include "hash.hpp"
#include "common.hpp"


#define DIFFICULTY 3
 
namespace DISTPROJ {

  struct Ballot {
    uint32_t num;
    std::string value;
	template<class Archive>
	void serialize(Archive & archive) {
      archive(num, value); // serialize things by passing them to the archive
	};
  };

  inline bool checkNonce(Ballot * b, SlotNum s , Nonce nonce){
    return sha256(b->value + std::to_string(s)+std::to_string(nonce)).substr(0, DIFFICULTY).find_first_not_of('0') == std::string::npos;
  }
  inline Nonce generateNonce(Ballot * b, SlotNum s){
    Nonce n=0;
    while(!checkNonce(b, s, n)){
      n++;
    }
    return n;
  }

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
