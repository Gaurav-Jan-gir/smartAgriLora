#include "lora_receiver.h"

class LoraSender{
    public:
        bool sendData(const SensorData& data, const byte &sender_address, const byte &receiver_address, LoRaClass &lora);
        bool sendConfig(const LoraParams& params, const byte &sender_address, const byte &receiver_address, LoRaClass &lora);
        bool sendThresholds(const Thresholds& thresholds, const byte &sender_address, const byte &receiver_address, LoRaClass &lora);
        bool sendFail(const byte &sender_address, const byte &receiver_address, LoRaClass &lora);
};