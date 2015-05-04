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
#include <memory>
using namespace DISTPROJ;

Slot::Slot(int id, LocalNode* m) : phi(PREPARE), node(m){
  state.slotNum = id;
}

std::shared_ptr<PrepareMessage> Slot::Prepare() {
  auto p = std::make_shared<PrepareMessage>(node->GetNodeID(), state.slotNum, state.b, state.p, state.p_, state.c, node->GetQuorumSet()); 
  return p;
}

std::shared_ptr<FinishMessage> Slot::Finish() {
  auto p = std::make_shared<FinishMessage>(node->GetNodeID(), state.slotNum, state.b, node->GetQuorumSet());
  return p;
}

void Slot::handle(std::shared_ptr<Message> _msg){
  // Add the message to be the last message seen
  // Handle the response
  std::shared_ptr<Message> last;
  auto fmsg =std::static_pointer_cast<FinishMessage>( _msg);
  auto pmsg = std::static_pointer_cast<PrepareMessage>( _msg);
  switch (_msg->type()) {
  case PrepareMessage_t:
    try {
      last = M.at(pmsg->from());
    }catch(std::out_of_range){
      last = std::make_shared<PrepareMessage>(pmsg->from(), 0, Ballot{}, Ballot{}, Ballot{}, Ballot{}, Quorum{});
      M[pmsg->from()] = last;
    }
      
    if (pmsg->follows(last)) {
      handle(pmsg);
      M[pmsg->from()] = pmsg;
    }
    break;
  case FinishMessage_t:
    try {
      last = M.at(fmsg->from());
    }catch(std::out_of_range){
      last = std::make_shared<PrepareMessage>(fmsg->from(), 0, Ballot{}, Ballot{}, Ballot{}, Ballot{}, Quorum{});
      M[fmsg->from()] = last;
    }

    if (fmsg->follows(last)) {
      handle(fmsg);
      M[fmsg->from()] = fmsg;
    }
    break;
  default:
    printf("GARBAGE");
    break;
  }

}
void Slot::handle(std::shared_ptr<PrepareMessage> msg) {
  printf("PREPARE\n");
  bool returnNow = false;
  if (phi == PREPARE ) {

    // First case: We've never voted for anything. I.E. b = 0;
    // Vote for b but don't accept yet.
    if (state.b == Ballot {}) {
      state.b = msg->b;
      // Send out vote for b.
      node->SendMessage(Prepare());
      return;
    }

    // if( true /* && a message allows v to accept that new ballots are prepared by either of accepts 2 criteria */) {
    // if prepared ballot then set p
    
    // Definition -- Accept:
    //  node v accepts statement a (the value in b) iff it has never accepted
    //  a contradicting statment and
    //  1) there exists U s.t. node v is in U and everyone in U has voted for
    //    or accepted a. OR
    //  2) Each member of a v-blocking set claims to accept a.
    
    // Check that we haven't accepted a contradicting statement.
    
    // NOTE : the > operator does not accomplish the logic below.
    if (msg->b.num >= state.p.num && compatible(msg->b, state.p)){
      // Now check that one of our quorum slices has all voted for or 
      // accepted b.
      auto b_voted_or_accepted = 0;
      for (auto kp : M) {
        auto m = kp.second;
        switch (m->type()) {
        case FinishMessage_t:
          if ((std::static_pointer_cast<FinishMessage>(m))->b == msg->b){
            b_voted_or_accepted +=1;
          }
          break;
        case PrepareMessage_t:
          if ( (std::static_pointer_cast<PrepareMessage>(m))->b == msg->b ||
               (std::static_pointer_cast<PrepareMessage>(m))->p == msg->b){
            b_voted_or_accepted +=1;
          }
          break;
        }
      }

      if (b_voted_or_accepted > node->quorumSet.threshold) {
        state.p = state.b;
        returnNow = true;
      }
    } else {
      // Statement contradicted. Check for v-blocking.
      auto b_vblock_vote = 0;
      for (auto kp : M) {
        auto m = kp.second;
        switch (m->type()) {
        case FinishMessage_t:
          if ((std::static_pointer_cast<FinishMessage>(m))->b == msg->b){
            b_vblock_vote +=1;
          }
          break;
        case PrepareMessage_t:
          if ((std::static_pointer_cast<PrepareMessage>(m))->p == msg->b){
            b_vblock_vote +=1;
          }
          break;
        }
      }

      if (b_vblock_vote > node->quorumSet.threshold) {
        // v-blocking set found so accept the contradicting ballot.
        state.p_ = state.p;
        state.p = Ballot{};
        state.b = msg->b;
        returnNow = true;
      }
    }

    // If a c ballot exists but p >!~ c or p_ >!~ c, clear c.
    if (state.c.num != 0 && (state.p > state.c || state.p_ > state.c)) {
      state.c = Ballot{};
      returnNow = true;
    }

    if (returnNow) {
      node->SendMessage(Prepare());
    }

    if ( state.b != state.c && state.b == state.p /* V confirms b is prepared */ ) {
      auto b_prepared = 0;
      for(auto kp : M) {
        auto m = kp.second;
        switch (m->type()){
        case FinishMessage_t:
          if ((std::static_pointer_cast<FinishMessage>(m))->b == state.p){
            b_prepared +=1;
          }
          break;
        case PrepareMessage_t:
          if ( (std::static_pointer_cast<PrepareMessage>(m))->b == state.p){
            b_prepared +=1;
          }
          break;
        }
      }

      if (b_prepared > node->quorumSet.threshold){
        state.c = state.b;
        node->SendMessage(Finish());
      }
    }

  }
}
void Slot::handle(std::shared_ptr<FinishMessage> msg) {
  printf("Finish");
  if (phi == PREPARE && state.b == state.p && state.b == state.c && msg->b == state.b) { // RULE 3
    phi = FINISH;
    // TODO (JHH) : Figure what if anything needs to happen here.
  }
  if (phi == FINISH && state.b == state.p && state.b == state.c && msg->b == state.b){ // RULE 4
    // Check that this node ~confirms~ b.
    auto b_commit = 0;
    for (auto kp : M) {
      auto m = kp.second;
      switch (m->type()) {
      case FinishMessage_t:
        if ((std::static_pointer_cast<FinishMessage>(m))->b == state.c){ // Finish -> b == Prepare -> c
          b_commit +=1;
        }
          
        break;
      case PrepareMessage_t:
        if ( (std::static_pointer_cast<PrepareMessage>(m))->c == state.c){
          b_commit +=1;
        }
        break;
      }
    }

    if (b_commit > node->quorumSet.threshold) {
      phi = EXTERNALIZE;
    }
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
