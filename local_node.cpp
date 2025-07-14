#include "local_node.h"
#include "lora_receiver.h"

void LocalNode::sendMessage(){
    
}

void LocalNode::receiveMessage() {
    uint16_t* message = receiver.receiveMessage(localAddress);
    if (!message) return;

    LoraReceiver::MessageType messageType = receiver.getMessageType();
    if (messageType == LoraReceiver::NONE) return;

    switch (messageType) {
        case LoraReceiver::DATA:
            // Ignored by local node
            break;

        case LoraReceiver::CONFIG: {
            LoraParams params = receiver.decodeParams(message);
            configManager.setParams(params);
            break;
        }

        case LoraReceiver::THRESHOLDS: {
            Thresholds th = receiver.decodeThresholds(message);
            configManager.setThresholds(th);
            break;
        }

        case LoraReceiver::SENDFAIL: {
            range *= 1.1f;
            LoraParams newParams = configManager.getOptimalParamsForRange(range);
            configManager.setParams(newParams);
            break;
        }

        default:
            break;
    }

    // delete[] message; // only if dynamically allocated
}
