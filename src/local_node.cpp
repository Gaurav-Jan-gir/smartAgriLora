#include "local_node.h"
#include "lora_receiver.h"
#include "data_collector.h"  // For get_sensor_data() function
#include <exception>
#include "LoRa.h"

const byte LocalNode::getDestinationAddress(){
    static uint8_t i = 0;
    return destination_addresses[i++ % size_da];
}

bool LocalNode::sendMessage(){
    try{
        get_sensor_data(sensorData);
        destination_address = getDestinationAddress();
        sender.sendData(sensorData, localAddress, destination_address, lora);
        return true;
    }
    catch(const std::exception& e){
        return false;
    }
}

bool LocalNode::receiveMessage() {
    PayloadData message = receiver.receiveMessage(localAddress, lora);
    if (!message.data) return false;

    LoraReceiver::MessageType messageType = receiver.getMessageType();
    if (messageType == LoraReceiver::NONE) return false;  // Fixed: added return statement

    switch (messageType) {
        case LoraReceiver::DATA:
            // Ignored by local node
            break;

        case LoraReceiver::CONFIG: {
            LoraParams params;
            receiver.decodeParams(message, params);
            configManager.setParams(params);
            break;
        }

        case LoraReceiver::THRESHOLDS: {
            Thresholds th;
            receiver.decodeThresholds(message, th);
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

    delete[] message.data; // only if dynamically allocated
    return true;
}
