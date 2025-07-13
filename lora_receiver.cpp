#include "LoRa.h"
#include "SPI.h"
#include <stdint.h>

byte local_address = 0x01;
byte broadcast_address = 0xFF;

void decode(uint32_t e_m, float* d_m){
    uint16_t temp = e_m & 0x7FF;
    uint16_t humidity = (e_m >> 11) & 0x3FF;
    uint16_t soil_moisture = (e_m >> 21) & 0x7FF;

    d_m[0] = (float(temp) - 400.0f) / 10.0f;
    d_m[1] = float(humidity) / 10.0f;
    d_m[2] = float(soil_moisture*100) / 1023.0f;
}

float* receive_message(int packetSize){
    if(packetSize < 6) return nullptr; 

    byte received_address = LoRa.read();
    byte sender_address = LoRa.read();

    if (received_address == local_address && sender_address == broadcast_address) {
        to_configure(packetSize - 2);
        return nullptr;
    }

    if (received_address != local_address && received_address != broadcast_address) {
        return nullptr;
    }

    uint32_t message = 0;
    for (int i = 0; i < 4; i++) {
        message |= ((uint32_t)LoRa.read()) << (8 * i);
    }

    float* data_values = new float[3];
    decode(message, data_values);
    return data_values;
}

bool to_configure(int remainingSize){
    if (remainingSize <= 0) return false;

    String config = "";
    while (LoRa.available() && config.length() < remainingSize) {
        config += (char)LoRa.read();
    }
    // Process config
    return config.length() == remainingSize;
}
