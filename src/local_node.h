#include "lora_receiver.h"
#include "lora_sender.h"
#include "Arduino.h"
#include "config_manager.h"
#include "data_collector.h"
#include "sensor_data.h"
#include "LoRa.h"
#define BROADCAST_ADDRESS 0xFF
const byte destination_addresses[] = {0x01, 0x02, 0x03, 0x04, 0x05};
uint8_t size_da = sizeof(destination_addresses)/sizeof(destination_addresses[0]);


class LocalNode{
    private:
        LoRaClass lora;
        LoraReceiver receiver;
        LoraSender sender;
        ConfigManager configManager;
        SensorData sensorData;
        float range;
        const byte localAddress = 0x03;
        byte destination_address = 0x01;
    public:
        LocalNode() : receiver(), sender(), configManager() , range(30.f) {
            lora.begin(865E6); // Initialize LoRa on 865 MHz for India
            sensorData.temperature = -100.0f;  // Initialize with default values
            sensorData.humidity = -100.0f;
            sensorData.soilMoisture = -100.0f;
        }
        bool sendMessage();
        bool receiveMessage();
        const byte getDestinationAddress();
};

