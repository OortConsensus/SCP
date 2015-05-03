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
    PrepareMessage(NodeID _v, unsigned int _slotID, Ballot _b, Ballot _p,
                   Ballot _p_, Ballot _c, Quorum _d)
      :  Message(PrepareMessage_t),v(_v), slotID(_slotID), b(_b), p(_p), p_(_p_), c(_c), d(_d) {};

    unsigned int getSlot() { return slotID; };
    std::string toJSON();
    NodeID from() {return v;};
    bool follows( Message * x) {
      auto m = (PrepareMessage *) x;
      switch (x->type()){
      case FinishMessage_t:
        return false;
      case PrepareMessage_t:
        return b.num > m->b.num || ( b.num == m->b.num && (p.num >
        m->p.num || (p.num == m->p.num && (p_.num > m->p_.num || (p_.num
        == m->p_.num && c.num > m->c.num))))); // See SCP pg 29
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
