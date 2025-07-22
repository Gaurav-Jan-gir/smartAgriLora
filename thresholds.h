struct Thresholds{
    float lowTemperature;   // Temperature in Celsius
    float highTemperature;  // Temperature in Celsius
    float lowHumidity;      // Humidity in percentage
    float highHumidity;     // Humidity in percentage
    float lowSoilMoisture;  // Soil moisture as raw ADC value (0-1023)
    float highSoilMoisture; // Soil moisture as raw ADC value (0-1023)
};