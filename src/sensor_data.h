struct SensorData {
    float temperature;  // Temperature in degrees Celsius
    float humidity;     // Humidity in percentage
    float soilMoisture; // Soil moisture as raw ADC value (0-1023)
    // Note: soilMoisture kept as raw value for compression efficiency
    // Will be converted to percentage at central node for display
};