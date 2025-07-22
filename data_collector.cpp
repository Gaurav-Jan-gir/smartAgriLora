#include "data_collector.h"

// Define the DHT sensor object here (not in header)
DHT dht(DHTPIN, DHTTYPE);

void get_sensor_data(SensorData& data) {
    data.temperature = dht.readTemperature();     // °C
    data.humidity = dht.readHumidity();           // %
    int rawSoil = analogRead(SOIL_PIN);           // 10-bit value: 0–1023
    data.soilMoisture = 1023.0f - rawSoil;        // Inverted raw ADC value (0-1023)
    // Note: Soil moisture kept as raw ADC value for compression efficiency
    // Will be converted to percentage at central node for user display
}