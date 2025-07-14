#include "lora_receiver.h"
#include "lora_sender.h"
#include "Arduino.h"
#include "config_manager.h"
#include "data_collector.h"
#include "sensor_data.h"
#include "LoRa.h"

class LocalNode{
    private:
        LoRaClass lora;
        LoraReceiver receiver;
        LoraSender sender;
        ConfigManager configManager;
        SensorData sensorData;
        float range;
        const byte localAddress = 0x03;
    public:
        LocalNode() : receiver(), sender(), configManager() , range(30.f) {
            lora.begin(868E6); // Initialize LoRa on 868 MHz
            sensorData.temperature = 0.0f;
            sensorData.humidity = 0.0f;
            sensorData.soilMoisture = 0.0f;
        }
        void sendMessage();
        void receiveMessage();
};