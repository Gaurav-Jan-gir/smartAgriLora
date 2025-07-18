#include "config_manager.h"
#include <cmath>

ConfigManager::ConfigManager() {
    param = getDefaultParams();
    thresholds = getDefaultThresholds();
}

LoraParams ConfigManager::getDefaultParams() {
    LoraParams params;
    params.fr = 865E6;           // 865 MHz for India (corrected from 868 MHz)
    params.sf = 7;               // SF7 for good range/speed balance
    params.bw = 125E3;           // 125 kHz standard bandwidth
    params.cr = 5;               // 4/5 coding rate
    params.tp = 17;              // 17 dBm transmission power
    params.sw = 0x34;            // Private network sync word
    params.pl = 8;               // Standard preamble length
    params.crc = true;           // Enable CRC for error detection
    return params;
}

Thresholds ConfigManager::getDefaultThresholds() {
    Thresholds thresholds;
    thresholds.lowTemperature = 5.0f;    // 5°C minimum
    thresholds.highTemperature = 35.0f;  // 35°C maximum
    thresholds.lowHumidity = 30.0f;      // 30% minimum humidity
    thresholds.highHumidity = 80.0f;     // 80% maximum humidity
    thresholds.lowSoilMoisture = 200.0f; // Minimum soil moisture (raw ADC: 0-1023)
    thresholds.highSoilMoisture = 800.0f; // Maximum soil moisture (raw ADC: 0-1023)
    return thresholds;
}

void ConfigManager::setParams(const LoraParams& params) {
    if (validateParams(params)) {
        param = params;
    }
}

const LoraParams& ConfigManager::getParams() const {
    return param;
}

LoraParams ConfigManager::getOptimalParamsForRange(float range) {
    LoraParams params = param;
    
    // Adjust parameters based on required range
    if (range < 1000) {
        // Short range: optimize for speed
        params.sf = 7;
        params.bw = 250E3;
        params.tp = 14;
    } else if (range < 5000) {
        // Medium range: balance speed and range
        params.sf = 9;
        params.bw = 125E3;
        params.tp = 17;
    } else {
        // Long range: optimize for maximum range
        params.sf = 12;
        params.bw = 62.5E3;
        params.tp = 20;
    }
    
    return params;
}

void ConfigManager::setThresholds(const Thresholds& thresholds) {
    if (validateThresholds(thresholds)) {
        this->thresholds = thresholds;
    }
}

const Thresholds& ConfigManager::getThresholds() const {
    return thresholds;
}

bool ConfigManager::validateParams(const LoraParams& params) {
    // Validate frequency (common ISM bands)
    if (params.fr != 433E6 && params.fr != 865E6 && params.fr != 866E6 && 
        params.fr != 867E6 && params.fr != 868E6 && params.fr != 915E6) {
        return false;
    }
    
    // Validate spreading factor
    if (params.sf < 6 || params.sf > 12) {
        return false;
    }
    
    // Validate bandwidth
    if (params.bw != 7.8E3 && params.bw != 10.4E3 && 
        params.bw != 15.6E3 && params.bw != 20.8E3 && 
        params.bw != 31.25E3 && params.bw != 41.7E3 && 
        params.bw != 62.5E3 && params.bw != 125E3 && 
        params.bw != 250E3) {
        return false;
    }
    
    // Validate coding rate
    if (params.cr < 5 || params.cr > 8) {
        return false;
    }
    
    // Validate TX power
    if (params.tp < 2 || params.tp > 20) {
        return false;
    }
    
    // Validate preamble length
    if (params.pl < 6 || params.pl > 65535) {
        return false;
    }
    
    return true;
}

bool ConfigManager::validateThresholds(const Thresholds& thresholds) {
    // Validate temperature thresholds
    if (thresholds.lowTemperature >= thresholds.highTemperature) {
        return false;
    }
    
    // Validate humidity thresholds
    if (thresholds.lowHumidity >= thresholds.highHumidity || 
        thresholds.lowHumidity < 0 || thresholds.highHumidity > 100) {
        return false;
    }
    
    // Validate soil moisture thresholds
    if (thresholds.lowSoilMoisture >= thresholds.highSoilMoisture ||
        thresholds.lowSoilMoisture < 0 || thresholds.highSoilMoisture > 1023) {
        return false;
    }
    
    return true;
}

void ConfigManager::resetToDefaults() {
    param = getDefaultParams();
    thresholds = getDefaultThresholds();
}

float ConfigManager::calculateRange(const LoraParams& params) {
    // Simplified range calculation based on LoRa parameters
    // Real-world range depends on many factors (obstacles, antenna, etc.)
    float baseRange = 1000.0f; // Base range in meters
    
    // Spreading factor increases range exponentially
    float sfMultiplier = pow(2, (params.sf - 7) * 0.5f);
    
    // Bandwidth affects sensitivity (lower BW = better sensitivity)
    float bwMultiplier = sqrt(125E3 / params.bw);
    
    // TX power affects range
    float powerMultiplier = pow(10, (params.tp - 14) / 20.0f);
    
    return baseRange * sfMultiplier * bwMultiplier * powerMultiplier;
}

