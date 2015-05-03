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

Slot::Slot(int id, LocalNode* m) : node(m), phi(PREPARE){
  state.slotNum = id;
}

PrepareMessage* Slot::Prepare() {
  auto p = new PrepareMessage(node->GetNodeID(), state.slotNum, state.b, state.p, state.p_, state.c, Quorum{}); 
  return p;
}

void Slot::handle(Message* msg){
  // Add the message to be the last message seen
  // Handle the response
  switch (msg->type()) {
  case PrepareMessage_t:
    handle((PrepareMessage*) msg);
    break;
  case FinishMessage_t:
    handle((FinishMessage*) msg);
    break;
  default:
    printf("GARBAGE");
    break;
  }
}
void Slot::handle(PrepareMessage* msg) {
  printf("PREPARE");
  bool returnNow = false;
  Message* last;
  try {
    last = M.at(msg->from());
  }catch(std::out_of_range){
    M[msg->from()] = msg;
    last = msg;
  }
  if (phi == PREPARE ) {
    // if( true /* && a message allows v to accept that new ballots are prepared by either of accepts 2 criteria */) {
    // if prepared ballot then set p
    if (msg->p > state.b){
      state.p = msg->p;
      returnNow = true;
    }
    // if prepared ballot then set p_

    if (msg->p_ > state.b){
      state.p_ = msg->p_;
      returnNow = true;
    }
    if (state.c.num != 0 && (state.p > state.c || state.p_ > state.c)) {
      state.c.num = 0;
      returnNow = true;
    }
    if (returnNow) {
      node->SendMessage(Prepare());
      return;

    }

    // }
    if ( state.b != state.c && state.b == state.p /* V confirms b is prepared */ ) {

      auto b_prepared = true;
      for( auto kp : M){
        auto m = kp.second;
        switch (m->type()){
        case FinishMessage_t:
          b_prepared &= ((FinishMessage *) m)->b == state.p;
        case PrepareMessage_t:
          b_prepared &= ((PrepareMessage *) m)->b == state.p;

        }

      }
      if (b_prepared){
        state.c = state.b;
      }

    }

  }
}
void Slot::handle(FinishMessage* msg) {
  printf("Finish");
  if (phi == PREPARE && state.b == state.p && state.b == state.c && msg->b == state.b) { // RULE 3
    phi = FINISH;

  }
  if (phi == FINISH && state.b == state.p && state.b == state.c && msg->b == state.b){ // RULE 4
    phi = EXTERNALIZE;
  }

}
// Dump state / received message inforamtion.
void Slot::Dump(){
  // TODO
}

void handlePrepare(NodeID v, Quorum& d, SlotState vState){
  // TODO
}

void handleFinish(NodeID v, Quorum& d, SlotState vState){
  // TODO
}
