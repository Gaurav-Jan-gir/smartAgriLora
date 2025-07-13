#include "headers/local_node.h"

LocalNode::getSensorData(){
    sensorData = get_data();
}

LocalNode::sendMessage(){
    getSensorData();
    LoraParams params = configManager.getOptimalParamsForRange(range);
    configManager.setParams(params);
    sender.sendData(sensorData, params);
}

LocalNode::receiveMessage(){
    int receiveType = receiver.getReceiveType();
    if (receiveType == LoraReceiver::DATA) {
        SensorData data = receiver.receiveData();
        // Local Nodes Don't receive data, ignore it
    } else if (receiveType == LoraReceiver::CONFIG) {
        LoraParams params = receiver.receiveConfig();
        configManager.setParams(params);
    } else if (receiveType == LoraReceiver::THRESHOLDS) {
        Thresholds th = receiver.receiveData();
        configManager.setThresholds(th);
    } else if (receiveType == LoraReceiver::SENDFAIL) {
        range = range * 1.1;
        sendMessage();
    }
}
