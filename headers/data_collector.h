#include "Arduino.h"
#include "sensor_data.h"
#include "DHT.h"
// ----- Sensor Configuration -----
#define DHTPIN 2              // Pin connected to DHT sensor
#define DHTTYPE DHT11         // Change to DHT22 if using that sensor
#define SOIL_PIN A0           // Analog pin for soil moisture

DHT dht(DHTPIN, DHTTYPE); // DHT sensor object

SensorData getData();