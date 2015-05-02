#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "ballot.hpp"
#include "quorum.hpp"
#include "common.hpp"

namespace DISTPROJ {

  enum MessageType {FinishMessage_t, PrepareMessage_t};
  
  class Message {
  private:
    MessageType t;

  public:
    Message(MessageType t) : t(t){};
    MessageType type() { return t;};
    virtual std::string toJSON()=0;
    // Message f(std::string s);
    virtual unsigned int getSlot() = 0;

  };
  
  class PrepareMessage : public Message {

  public:
    PrepareMessage(NodeID _v, unsigned int _slotID, Ballot _b, Ballot _p,
                   Ballot _p_0, Ballot _c, Quorum _d)
      :  Message(PrepareMessage_t),v(_v), slotID(_slotID), b(_b), p(_p), p_0(_p_0), c(_c), d(_d) {};

    unsigned int getSlot() { return slotID; };
    std::string toJSON();

  private:
    NodeID v;
    unsigned int slotID;
    Ballot b, p, p_0, c;
    Quorum d;

    
  };
  
  class FinishMessage : public Message {
    
  public:
    FinishMessage(NodeID _v, unsigned int _slotID, Ballot _b, Quorum _d)
      : Message(FinishMessage_t), v(_v), slotID(_slotID), b(_b), d(_d)  {};

    unsigned int getSlot() { return slotID; };
    std::string toJSON();

  private:
    NodeID v;
    unsigned int slotID;
    Ballot b;
    Quorum d;

  };

}

#endif
