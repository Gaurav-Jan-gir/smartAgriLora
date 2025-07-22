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
    
    // Validate payload: must be even number of bytes and > 0
    if (payloadBytes <= 0 || payloadBytes % 2 != 0) {
        messageType = MessageType::NONE;
        return {nullptr, 0};
    }
    
    int payloadWords = payloadBytes / 2;

    // Allocate payload buffer
    uint16_t* payload = new uint16_t[payloadWords];
    for (int i = 0; i < payloadWords; i++) {
        payload[i] = (uint16_t)lora.read() | ((uint16_t)lora.read() << 8);
    }

    return {payload, payloadWords};
}

void LoraReceiver::decodeData(const PayloadData& payload, SensorData& data) {
    if (payload.size < 2) return;
    uint32_t compressedMessage = ((uint32_t)payload.data[1] << 16) | payload.data[0];
    data.temperature = compressedMessage & 0x7FF;
    data.humidity = (compressedMessage >> 11) & 0x3FF;
    data.soilMoisture = (compressedMessage >> 21) & 0x3FF;
    data.temperature = (data.temperature - 400) / 10.0f; // Convert to Celsius
    data.humidity = data.humidity / 10.0f; // Convert to percentage
    // Keep soil moisture as raw ADC value (0-1023) for now
    // Will be converted to percentage at central node for user display
}

void LoraReceiver::decodeThresholds(const PayloadData& payload, Thresholds& thresholds) {
    if (payload.size < 6) return;
    thresholds.lowTemperature = (int16_t)(payload.data[0] & 0x7FF) / 10.0f - 40.0f; // Convert to Celsius
    thresholds.highTemperature = (int16_t)(payload.data[1] & 0x7FF) / 10.0f - 40.0f; // Convert to Celsius
    thresholds.lowHumidity = (uint16_t)(payload.data[2] & 0x3FF) / 10.0f; // Convert to percentage
    thresholds.highHumidity = (uint16_t)(payload.data[3] & 0x3FF) / 10.0f; // Convert to percentage
    thresholds.lowSoilMoisture = (uint16_t)(payload.data[4] & 0x3FF) / 10.0f; // Convert to raw ADC value
    thresholds.highSoilMoisture = (uint16_t)(payload.data[5] & 0x3FF) / 10.0f; // Convert to raw ADC value
}

void LoraReceiver::decodeParams(const PayloadData& payload, LoraParams& params) {
    if (payload.size < 5) return; // Minimum size for params
    params.tp = payload.data[0] & 0xFF; // Transmission power
    params.sf = payload.data[0] >> 8; // Spreading factor
    params.cr = payload.data[1] & 0xFF; // Coding rate
    params.sw = payload.data[1] >> 8; // Sync word
    params.pl = payload.data[2];
    params.fr = payload.data[3] * 1e6; // Convert MHz to Hz
    params.bw = payload.data[4] * 1e3; // Convert kHz to Hz
}

