#include "message.hpp"
#include "slot.hpp"
#include "quorum.hpp"
#include <string>


using namespace DISTPROJ;


template<class Archive>
void PrepareMessage::serialize(Archive & archive) {
  archive(v, slotID, b, p, p_, c, d);
  // archive(v, slotID, b,p,p_,c,d);
}

template<class Archive>
void FinishMessage::serialize(Archive & archive) {
  archive(v,slotID, b,d); // serialize things by passing them to the archive
}


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
}


bool FinishMessage::follows( std::shared_ptr<Message> x) {
  auto m = std::static_pointer_cast<FinishMessage>( x);
  switch (x->type()){
  case FinishMessage_t:
	return b.num > m->b.num;
  case PrepareMessage_t:
	return true;
  default:
	return true; 
  }
}




bool PrepareMessage::follows( std::shared_ptr<Message> x) {
  auto m = std::static_pointer_cast<PrepareMessage>(x);
  auto first = b.num > m->b.num;
  auto first_continue = b.num == m->b.num;
  auto second = p.num > m->p.num;
  auto second_continue = p.num == m->p.num;
  auto third = p_.num > m->p_.num;
  auto third_continue =p_.num == m->p_.num;
  auto fourth = c.num > m->c.num;

  switch (x->type()){
  case FinishMessage_t:
	return false;
  case PrepareMessage_t:
	return first || (  first_continue && (second || (second_continue && (third || (third_continue && fourth))))); // See SCP pg 29
  default:
	return true; 
  }
}

