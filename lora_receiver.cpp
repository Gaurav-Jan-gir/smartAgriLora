#include "LoRa.h"
#include "SPI.h"
#include <stdint.h>
#include "lora_receiver.h"

#ifndef MIN_PACKET
#define MIN_PACKET 4  // Minimum: [Type, To, From]
#endif

uint16_t* LoraReceiver::receiveMessage(byte &local_address, LoRaClass &lora) {
    int packetSize = lora.parsePacket();
    if (packetSize < MIN_PACKET) {
        messageType = MessageType::NONE;
        return nullptr;
    }

    // Read message type
    uint8_t typeByte = lora.read();
    if (typeByte < 1 || typeByte > 4) {
        messageType = MessageType::NONE;
        return nullptr;
    }
    messageType = static_cast<MessageType>(typeByte);

    // Read addresses
    byte received_address = lora.read();
    byte sender_address = lora.read();

    if (received_address != local_address && received_address != BROADCAST_ADDRESS) {
        messageType = MessageType::NONE;
        return nullptr;
    }

    // Calculate remaining payload
    int payloadBytes = packetSize - 3;
    int payloadWords = payloadBytes / 2;

    // Allocate payload buffer
    uint16_t* payload = new uint16_t[payloadWords];
    for (int i = 0; i < payloadWords; i++) {
        payload[i] = (uint16_t)lora.read() | ((uint16_t)lora.read() << 8);
    }

    return payload;
}


