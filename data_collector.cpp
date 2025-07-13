#include <Arduino.h>
#include <DHT.h>

// ----- Sensor Configuration -----
#define DHTPIN 2              // Pin connected to DHT sensor
#define DHTTYPE DHT11         // Change to DHT22 if using that sensor
#define SOIL_PIN A0           // Analog pin for soil moisture

DHT dht(DHTPIN, DHTTYPE); // DHT sensor object

// Struct to hold sensor data
struct SensorData {
    float temperature;   // in Celsius
    float humidity;      // in Percent
    float soilMoisture;  // Inverted raw 10-bit value (for compression use)
};

// Call this function in your main Arduino sketch
SensorData get_data() {
    SensorData data;

    data.temperature = dht.readTemperature();     // °C
    data.humidity = dht.readHumidity();           // %
    int rawSoil = analogRead(SOIL_PIN);           // 10-bit value: 0–1023
    data.soilMoisture = 1023.0f - rawSoil;        // Inverted value

    return data;
}