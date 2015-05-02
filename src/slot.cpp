  // switch (_msg.type()) {
  //   case PrepareMessage_t:
  //     auto msg = (PrepareMessage) _msg;

  //     if (log.find(msg.getSlot()) == log.end()) {
  //       log[msg.getSlot()] = Slot(msg.getSlot());
  //     }

  //     log[msg.getSlot()].handle(msg)

  //     break;
  //   case FinishMessage_t:
  //     auto msg = (FinishMessage) _msg;
  //     break;
  //   default:
  //     std::cout << "Panic : unknown message type.\n";
  // }
#include "node.hpp"
#include "quorum.hpp"
#include "message.hpp"
#include "slot.hpp"
using namespace DISTPROJ;
Slot::Slot(int id){
  state.slotNum = id;
}

void Slot::handle(Message& msg){


}

      // Dump state / received message inforamtion.
void Slot::Dump(){

}

void Slot::handlePrepare(NodeID v, Quorum& d, SlotState vState){

}
void Slot::handleFinish(NodeID v, Quorum& d, SlotState vState){


}
