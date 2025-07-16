#include "lora_receiver.h"

#ifndef MIN_PACKET
#define MIN_PACKET 4  // Minimum: [Type, To, From]
#endif

PayloadData LoraReceiver::receiveMessage(const byte &local_address, LoRaClass &lora) {
    int packetSize = lora.parsePacket();
    if (packetSize < MIN_PACKET) {
        messageType = MessageType::NONE;
        return {nullptr, 0};
    }

    // Read message type
    uint8_t typeByte = lora.read();
    if (typeByte < 1 || typeByte > 4) {
        messageType = MessageType::NONE;
        return {nullptr, 0};
    }
    messageType = static_cast<MessageType>(typeByte);

    // Read addresses
    byte received_address = lora.read();
    byte sender_address = lora.read();

    if (received_address != local_address && received_address != BROADCAST_ADDRESS) {
        messageType = MessageType::NONE;
        return {nullptr, 0};
    }

    // Calculate remaining payload
    int payloadBytes = packetSize - 3;
    int payloadWords = payloadBytes / 2;

    // Allocate payload buffer
    uint16_t* payload = new uint16_t[payloadWords];
    for (int i = 0; i < payloadWords; i++) {
        payload[i] = (uint16_t)lora.read() | ((uint16_t)lora.read() << 8);
    }

    return {payload, payloadWords};
}

SensorData LoraReceiver::decodeData(const PayloadData& payload) {
    SensorData data;
     if (payload.size < 2) return data; 
    uint32_t compressedMessage = ((uint32_t)payload.data[1] << 16) | payload.data[0];
    data.temperature = compressedMessage & 0x7FF;
    data.humidity = (compressedMessage >> 11) & 0x3FF;
    data.soilMoisture = (compressedMessage >> 21) & 0x3FF;
    data.temperature = (data.temperature - 400) / 10.0f; // Convert to Celsius
    data.humidity = data.humidity / 10.0f; // Convert to percentage
    data.soilMoisture = (data.soilMoisture * 100.0f) / 1023.0f; // Inverted value
    return data;
}

Thresholds LoraReceiver::decodeThresholds(const PayloadData& payload){
    Thresholds th;
    if (payload.size < 1) return th; // Not enough data
    for (int i = 0; i < payload.size; i++) {
        uint16_t message = payload.data[i];
        uint8_t type = message & 0x07; // Last 3 bits for type
        float value = (message >> 3) / 10.0f; // Next bits for value, divided by 10 for precision

        switch (type) {
            case 1: th.lowTemperature = value/10.0f - 40.0f; break;
            case 2: th.highTemperature = value/10.0f - 40.0f; break;
            case 3: th.lowHumidity = value/10.0f; break;
            case 4: th.highHumidity = value/10.0f; break;
            case 5: th.lowSoilMoisture = value/10.0f; break;
            case 6: th.highSoilMoisture = value/10.0f; break;
            default: break; // Ignore unknown types
        }
    }
    return th;  

}

LoraParams LoraReceiver::decodeParams(const PayloadData& payload) {
    LoraParams params;
    for(int i = 0; i < payload.size; i++) {
        uint16_t message = payload.data[i];
        uint8_t type = message & 0x07; // Last 3 bits for type
        message >>= 3; // Shift to get the actual value
        switch (type) {
            case 0: params.tp = message; break; // Transmission power
            case 1: params.sf = message; break; // Spreading factor
            case 2: params.cr = message; break; // Coding rate
            case 3: params.pl = message; break; // Preamble length
            case 4: params.sw = message; break; // Sync word
            case 5: params.fr = message; break; // Frequency
            case 6: params.bw = message; break; // Bandwidth
            default: break; // Ignore additional data
        }
    }
    return params;
}

