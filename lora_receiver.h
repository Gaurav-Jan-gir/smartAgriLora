#ifndef LORA_RECEIVER_H
#define LORA_RECEIVER_H

#include <stdint.h>
#include "sensor_data.h"
#include "thresholds.h"
#include "lora_params.h"
#include "payload_data.h"
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
    PayloadData receiveMessage(const byte &localAddress, LoRaClass &lora);
    MessageType getMessageType() const { return messageType; }
    SensorData decodeData(const PayloadData& payload);
    Thresholds decodeThresholds(const PayloadData& payload);
    LoraParams decodeParams(const PayloadData& payload);
    bool decodeFail(const PayloadData& payload);
};

#endif
