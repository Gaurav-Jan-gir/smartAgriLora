#include "headers/data_collector.h"

SensorData get_data() {
    SensorData data;
    data.temperature = dht.readTemperature();     // °C
    data.humidity = dht.readHumidity();           // %
    int rawSoil = analogRead(SOIL_PIN);           // 10-bit value: 0–1023
    data.soilMoisture = 1023.0f - rawSoil;        // Inverted value
    return data;
}