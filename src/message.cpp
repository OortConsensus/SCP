#include "message.hpp"
#include "slot.hpp"
#include "quorum.hpp"
#include <string>


using namespace DISTPROJ;

// Message fromJSON(std::string s){

//   return FinishMessage(10,10,new Ballot{0,""}, new Quorum() );
// }

std::string PrepareMessage::toJSON(){
  return "";
}

std::string FinishMessage::toJSON(){
  return "";
}


template<class Archive>
void PrepareMessage::serialize(Archive & archive) {
  archive(v, slotID, b, p, p_, c, d);
  // archive(v, slotID, b,p,p_,c,d);
};


template<class Archive>
void Message::serialize(Archive & archive) {

  auto pm = (PrepareMessage*) this;
  auto fm = (FinishMessage*) this;
  switch (t) {
  case PrepareMessage_t:
	pm->serialize(archive);
	break;
  case FinishMessage_t:
	fm->serialize(archive);
	break;
  default:
	archive("grave error: Tried to serialize raw message");
	break;
  }
};
