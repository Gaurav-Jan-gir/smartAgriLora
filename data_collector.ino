#include "DHT.h”

// ----- Sensor Configuration -----
#define DHTPIN 2              // Pin connected to DHT sensor
#define DHTTYPE DHT11         // Change to DHT22 if using that sensor
#define SOIL_PIN A0           // Analog pin for soil moisture

DHT dht(DHTPIN, DHTTYPE);

// Struct to hold sensor data
struct SensorData {
    float temperature;   // °C
    float humidity;      // %
    float soilMoisture;  // 10-bit raw value (inverted)
};

// Function to get sensor data using struct
SensorData get_data() {
    SensorData data;

    data.temperature = dht.readTemperature();     // °C
    data.humidity = dht.readHumidity();           // %
    int rawSoil = analogRead(SOIL_PIN);           // Raw 10-bit analog
    data.soilMoisture = 1023.0f - rawSoil;        // Inverted value

    return data;
}