#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "ballot.hpp"
#include "quorum.hpp"
#include "common.hpp"

namespace DISTPROJ {

  enum MessageType {FinishMessage_t=0, PrepareMessage_t=1};
  
  class Slot;
  class Message {
  private:
    MessageType t;

  public:
    Message(MessageType t) : t(t){};
    MessageType type() { return t;};
    virtual std::string toJSON()=0;
    // Message f(std::string s);
    virtual unsigned int getSlot() = 0;

    virtual bool follows(Message * x) = 0;

  };
  
  class PrepareMessage : public Message {

  public:
    PrepareMessage(NodeID _v, SlotNum _slotID, Ballot _b, Ballot _p,
                   Ballot _p_, Ballot _c, Quorum _d)
      :  Message(PrepareMessage_t),v(_v), slotID(_slotID), b(_b), p(_p), p_(_p_), c(_c), d(_d) {};

    unsigned int getSlot() { return slotID; };
    std::string toJSON();
    NodeID from() {return v;};
    bool follows( Message * x) {
      auto m = (PrepareMessage *) x;
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

  private:
    NodeID v;
    unsigned int slotID;
    Ballot b, p, p_, c;
    Quorum d;

    friend Slot;
    
  };
  
  class FinishMessage : public Message {
    
  public:
    FinishMessage(NodeID _v, unsigned int _slotID, Ballot _b, Quorum _d)
      : Message(FinishMessage_t), v(_v), slotID(_slotID), b(_b), d(_d)  {};

    unsigned int getSlot() { return slotID; };
    std::string toJSON();
    NodeID from() {return v;};
    bool follows( Message * x) {
      auto m = (FinishMessage *) x;
      switch (x->type()){
      case FinishMessage_t:
        return b.num > m->b.num;
      case PrepareMessage_t:
        return true;
      default:
        return true; 
      }
    }

  private:
    NodeID v;
    unsigned int slotID;
    Ballot b;
    Quorum d;

    friend Slot;
  };

}

#endif
