#include "lora_sender.h"

bool LoraSender::sendData(const SensorData& data, const byte &sender_address, const byte &receiver_address, LoRaClass &lora) {
    lora.beginPacket();
    lora.write((uint8_t)LoraReceiver::MessageType::DATA);
    lora.write(receiver_address);
    lora.write(sender_address);

    uint32_t compressedMessage = ((uint32_t)(data.soilMoisture * 1023.0f / 100.0f) << 21) |
                                    ((uint32_t)(data.humidity * 10.0f) << 11) |
                                    (uint32_t)(data.temperature * 10.0f + 400);
    lora.write((uint8_t)(compressedMessage & 0xFF));
    lora.write((uint8_t)((compressedMessage >> 8) & 0xFF));
    lora.write((uint8_t)((compressedMessage >> 16) & 0xFF));
    lora.write((uint8_t)((compressedMessage >> 24) & 0xFF));

    return lora.endPacket() > 0;
}

bool LoraSender::sendConfig(const LoraParams& params, const byte &sender_address, const byte &receiver_address, LoRaClass &lora) {
    lora.beginPacket();
    lora.write((uint8_t)LoraReceiver::MessageType::CONFIG);
    lora.write(receiver_address);
    lora.write(sender_address);
    uint16_t fr = (uint16_t)(params.fr / 1e6); // Convert frequency to MHz
    uint16_t bw = (uint16_t)(params.bw / 1e3); // Convert bandwidth to kHz

    lora.write((uint8_t)params.tp);
    lora.write((uint8_t)params.sf);
    lora.write((uint8_t)params.cr);
    lora.write((uint8_t)params.sw);
    lora.write((uint8_t)(params.pl & 0xFF));
    lora.write((uint8_t)((params.pl >> 8) & 0xFF));
    lora.write((uint8_t)(fr & 0xFF));
    lora.write((uint8_t)((fr >> 8) & 0xFF));
    lora.write((uint8_t)(bw & 0xFF));
    lora.write((uint8_t)((bw >> 8) & 0xFF));
    
    return lora.endPacket() > 0;
}

bool LoraSender::sendThresholds(const Thresholds& thresholds, const byte &sender_address, const byte &receiver_address, LoRaClass &lora) {
    lora.beginPacket();
    lora.write((uint8_t)LoraReceiver::MessageType::THRESHOLDS);
    lora.write(receiver_address);
    lora.write(sender_address);

    uint16_t lowTemp = (int16_t)(thresholds.lowTemperature * 10.0f + 400);
    uint16_t highTemp = (int16_t)(thresholds.highTemperature * 10.0f + 400);
    uint16_t lowHumidity = (uint16_t)(thresholds.lowHumidity * 10.0f);
    uint16_t highHumidity = (uint16_t)(thresholds.highHumidity * 10.0f);
    uint16_t lowSoilMoisture = (uint16_t)(thresholds.lowSoilMoisture * 10.0f);
    uint16_t highSoilMoisture = (uint16_t)(thresholds.highSoilMoisture * 10.0f);

    lora.write((uint8_t)(lowTemp & 0xFF));
    lora.write((uint8_t)((lowTemp >> 8) & 0xFF));
    lora.write((uint8_t)(highTemp & 0xFF));
    lora.write((uint8_t)((highTemp >> 8) & 0xFF));
    lora.write((uint8_t)(lowHumidity & 0xFF));  
    lora.write((uint8_t)((lowHumidity >> 8) & 0xFF));
    lora.write((uint8_t)(highHumidity & 0xFF));
    lora.write((uint8_t)((highHumidity >> 8) & 0xFF));
    lora.write((uint8_t)(lowSoilMoisture & 0xFF));
    lora.write((uint8_t)((lowSoilMoisture >> 8) & 0xFF));
    lora.write((uint8_t)(highSoilMoisture & 0xFF));
    lora.write((uint8_t)((highSoilMoisture >> 8) & 0xFF));

    return lora.endPacket() > 0;
}

bool LoraSender::sendFail(const byte &sender_address, const byte &receiver_address, LoRaClass &lora) {
    lora.beginPacket();
    lora.write((uint8_t)LoraReceiver::MessageType::SENDFAIL);
    lora.write(receiver_address);
    lora.write(sender_address);

    return lora.endPacket() > 0;
}
