#include "lora_receiver.h"
#include "lora_sender.h"
#include "Arduino.h"
#include "config_manager.h"
#include "data_collector.h"
#include "sensor_data.h"

class LocalNode{
    private:
        LoraReceiver receiver;
        LoraSender sender;
        ConfigManager configManager;
        SensorData sensorData;
        int range;
        const byte localAddress = 0x03;
    public:
        LocalNode() : receiver(), sender(), configManager() , range(0) {
            sensorData.temperature = 0.0f;
            sensorData.humidity = 0.0f;
            sensorData.soilMoisture = 0.0f;
        }
        void sendMessage();
        void receiveMessage();
        void getSensorData();
};