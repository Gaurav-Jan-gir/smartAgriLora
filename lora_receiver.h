#ifndef LORA_RECEIVER_H
#define LORA_RECEIVER_H

#include <stdint.h>
#include "sensor_data.h"
#include "thresholds.h"
#include "lora_params.h"
#include "LoRa.h"

#ifndef BROADCAST_ADDRESS
#define BROADCAST_ADDRESS 0xFF
#endif

class LoraReceiver {
public:
    // Scoped and type-safe enum
    enum MessageType : uint8_t {
        NONE = 0,
        DATA,
        CONFIG,
        THRESHOLDS,
        SENDFAIL
    };

private:
    MessageType messageType = NONE;

public:
    uint16_t* receiveMessage(byte &localAddress, LoRaClass &lora);
    MessageType getMessageType() const { return messageType; }
    SensorData decodeData(uint16_t* message);
    Thresholds decodeThresholds(uint16_t* message);
    LoraParams decodeParams(uint16_t* message);
    bool decodeFail(uint16_t* message);
};

#endif
