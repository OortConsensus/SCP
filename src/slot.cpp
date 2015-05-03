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

PrepareMessage* Slot::Prepare() {
	auto p = new PrepareMessage(node->GetNodeID(), state.slotNum, state.b, state.p, state.p_, state.c, node->GetQuorumSet()); 
	return p;
}

FinishMessage* Slot::Finish() {
  auto p = new FinishMessage(node_>GetNodeID(), state.slotNum, state.b, node->GetQuorumSet());
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
        if ((std::static_pointer_cast<FinishMessage>(m))->c == state.c){
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

    if (b_commit > node.quorumSet.threshold) {
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
