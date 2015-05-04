#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

#include "ballot.hpp"
#include "quorum.hpp"
#include "common.hpp"
#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/polymorphic.hpp"

namespace DISTPROJ {

  enum MessageType {FinishMessage_t=0, PrepareMessage_t=1};
  
  class Slot;
  class FinishMessage;
  class PrepareMessage;
  class Message {
  private:
    MessageType t;

  public:
    Message(MessageType t) : t(t){};
    MessageType type() { return t;};
    // Message f(std::string s);
    virtual unsigned int getSlot() = 0;

    virtual bool follows(std::shared_ptr<Message> x) = 0;
	
  };
  
  class PrepareMessage : public Message {

  public:
	PrepareMessage() : PrepareMessage(0,0,Ballot{}, Ballot{}, Ballot{}, Ballot{}, Quorum{}, 0){};
    PrepareMessage(NodeID _v, SlotNum _slotID, Ballot _b, Ballot _p,
                   Ballot _p_, Ballot _c, Quorum _d, Nonce n)
      :  Message(PrepareMessage_t),v(_v), slotID(_slotID), b(_b), p(_p), p_(_p_), c(_c), d(_d), nonce(n) {};

    template<class Archive>
    void serialize(Archive & archive) {
      archive(CEREAL_NVP(v), CEREAL_NVP(slotID), CEREAL_NVP(b),
              CEREAL_NVP(p), CEREAL_NVP(p_), CEREAL_NVP(c),
              CEREAL_NVP(d), CEREAL_NVP(nonce));
    };
    unsigned int getSlot() { return slotID; };
    NodeID from() {return v;};

    bool follows( std::shared_ptr<Message> x);
  private:
    NodeID v;
    unsigned int slotID;
    Ballot b, p, p_, c;
    Quorum d;
    Nonce nonce;

    friend Slot;
    
  };
  
  class FinishMessage : public Message {
    
  public:
	FinishMessage(): FinishMessage(0,0,Ballot{}, Quorum{}){};
    FinishMessage(NodeID _v, unsigned int _slotID, Ballot _b, Quorum _d)
      : Message(FinishMessage_t), v(_v), slotID(_slotID), b(_b), d(_d)  {};



    template<class Archive>
    void serialize(Archive & archive) {
      archive(CEREAL_NVP(v),CEREAL_NVP(slotID), CEREAL_NVP(b),CEREAL_NVP(d)); // serialize things by passing them to the archive
    };
    unsigned int getSlot() { return slotID; };
    NodeID from() {return v;};
    bool follows( std::shared_ptr<Message> x);
  private:
    NodeID v;
    unsigned int slotID;
    Ballot b;
    Quorum d;

    friend Slot;
  };

}
CEREAL_REGISTER_TYPE_WITH_NAME(DISTPROJ::FinishMessage, "Finish");
CEREAL_REGISTER_TYPE_WITH_NAME(DISTPROJ::PrepareMessage, "Prepare");

#endif
