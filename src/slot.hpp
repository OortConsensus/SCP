#ifndef SLOT_H
#define SLOT_H

#include <string>
#include <map>

#include "ballot.hpp"
#include "quorum.hpp"
#include "common.hpp"

namespace DISTPROJ {

  class Message;

  enum Phase { PREPARE, FINISH, EXTERNALIZE };

  struct SlotState {
    Ballot b;
    Ballot p;
    Ballot p_0;
    Ballot c;
    unsigned int slotNum;
  };


  class Slot {

    public:
      Slot(int id);

      void handle(Message& msg);

      // Dump state / received message inforamtion.
      void Dump();

    private:
      void handlePrepare(NodeID v, Quorum& d, SlotState vState);
      void handleFinish(NodeID v, Quorum& d, SlotState vState);

      SlotState state;
      Phase phi;
      std::map<NodeID, Message&> M;

  };

}

#endif
