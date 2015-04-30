  switch (_msg.type()) {
    case PrepareMessage_t:
      auto msg = (PrepareMessage) _msg;

      if (log.find(msg.getSlot()) == log.end()) {
        log[msg.getSlot()] = Slot(msg.getSlot());
      }

      log[msg.getSlot()].handle(msg)

      break;
    case FinishMessage_t:
      auto msg = (FinishMessage) _msg;
      break;
    default:
      std::cout << "Panic : unknown message type.\n";
  }
