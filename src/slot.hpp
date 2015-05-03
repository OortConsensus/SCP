#ifndef SLOT_H
#define SLOT_H

#include <string>
#include <map>

#include "ballot.hpp"
#include "quorum.hpp"
#include "common.hpp"

namespace DISTPROJ {

  class Message;
  class PrepareMessage;
  class FinishMessage;
  class LocalNode;
  enum Phase { PREPARE, FINISH, EXTERNALIZE };

  struct SlotState {
    Ballot b;
    Ballot p;
    Ballot p_;
    Ballot c;
    unsigned int slotNum;
  };


  class Slot {

  public:
    Slot(int id, LocalNode* m);

    void handle(std::shared_ptr<Message> msg);

    // Dump state / received message inforamtion.
    void Dump();

  private:
    void handle(std::shared_ptr<PrepareMessage> msg);
    void handle(std::shared_ptr<FinishMessage> msg);
    // void handlePrepare(NodeID v, Quorum& d, SlotState vState);
    // void handleFinish(NodeID v, Quorum& d, SlotState vState);

    SlotState state;
    Phase phi;
    std::map<NodeID, std::shared_ptr<Message>> M;
    LocalNode * node;

    std::shared_ptr<PrepareMessage> Prepare();


  };
}

#endif
