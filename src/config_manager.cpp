#include "config_manager.h"
#include <cmath>

ConfigManager::ConfigManager() {
    param = getDefaultParams();
    thresholds = getDefaultThresholds();
}

LoraParams ConfigManager::getDefaultParams() {
    LoraParams params;
    params.fr = 433E6;           // 433 MHz (within SX1278 range)
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
    
    // Optimize parameters based on specific range requirements
    if (range <= 50) {
        // Very short range: maximum speed, minimum power
        params.sf = 6;       // SF6 for fastest transmission
        params.bw = 250E3;   // 250 kHz for high data rate
        params.tp = 2;       // 2 dBm low power (corrected from -1 dBm)
        params.cr = 5;       // 4/5 coding rate for speed
        params.pl = 6;       // Minimum preamble length
    } else if (range <= 100) {
        // Short range: optimize for speed with slight range increase
        params.sf = 7;       // SF7 for good balance
        params.bw = 250E3;   // 250 kHz bandwidth
        params.tp = 2;       // 2 dBm power (minimum for reliability)
        params.cr = 5;       // 4/5 coding rate
        params.pl = 6;       // Minimum preamble for energy savings
    } else if (range <= 120) {
        // Ultra-efficient 100m config: optimized for agricultural sensors
        params.sf = 7;       // SF7 proven reliable at 100m
        params.bw = 125E3;   // 125 kHz for better sensitivity margin
        params.tp = 2;       // 2 dBm minimum power 
        params.cr = 5;       // 4/5 coding rate for speed
        params.pl = 6;       // Minimum preamble
    } else if (range <= 200) {
        // Medium-short range: balanced approach
        params.sf = 7;       // SF7 
        params.bw = 125E3;   // 125 kHz for better sensitivity
        params.tp = 10;      // 10 dBm power
        params.cr = 5;       // 4/5 coding rate
        params.pl = 8;       // Standard preamble
    } else if (range <= 400) {
        // Medium range: start prioritizing range over speed
        params.sf = 8;       // SF8 for better range
        params.bw = 125E3;   // 125 kHz standard
        params.tp = 14;      // 14 dBm power
        params.cr = 5;       // 4/5 coding rate
        params.pl = 8;       // Standard preamble
    } else if (range <= 600) {
        // Medium-long range: good balance
        params.sf = 9;       // SF9 for extended range
        params.bw = 125E3;   // 125 kHz
        params.tp = 17;      // 17 dBm power
        params.cr = 5;       // 4/5 coding rate
        params.pl = 8;       // Standard preamble
    } else if (range <= 1000) {
        // Long range: prioritize range with acceptable speed
        params.sf = 10;      // SF10 for long range
        params.bw = 125E3;   // 125 kHz
        params.tp = 17;      // 17 dBm power
        params.cr = 6;       // 4/6 coding rate for better error correction
        params.pl = 12;      // Longer preamble for better sync
    } else if (range <= 2000) {
        // Very long range: sacrifice speed for range
        params.sf = 11;      // SF11 for very long range
        params.bw = 62.5E3;  // 62.5 kHz for better sensitivity
        params.tp = 20;      // 20 dBm maximum power (SX1278 supports this)
        params.cr = 6;       // 4/6 coding rate
        params.pl = 16;      // Longer preamble
    } else {
        // Maximum range: optimize for maximum distance
        params.sf = 12;      // SF12 for maximum range
        params.bw = 62.5E3;  // 62.5 kHz for best sensitivity
        params.tp = 20;      // 20 dBm maximum power (SX1278 supports this)
        params.cr = 8;       // 4/8 coding rate for maximum error correction
        params.pl = 20;      // Longest preamble for best sync
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
    // Validate frequency (SX1278 supports 137-525 MHz)
    if (params.fr != 433E6 && params.fr != 434E6 && params.fr != 435E6) {
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
    
    // Validate TX power (SX1278: RFO pin +2 to +14dBm, PA_BOOST +2 to +20dBm)
    // Note: +20dBm requires PA_BOOST pin and has duty cycle limitations
    if (params.tp < 2 || params.tp > 20) {
        return false;
    }
    
    // Warn about high power limitations (for future implementation)
    // +20dBm should be used sparingly due to current consumption (125mA)
    
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
    // Enhanced range calculation based on SX1278 sensitivity specifications
    // Real-world range depends on many factors (obstacles, antenna, etc.)
    
    // Base sensitivity values from SX1278 datasheet
    float baseSensitivity = -137.0f; // dBm for SF12/125kHz
    
    // Adjust sensitivity based on SF and BW (from datasheet measurements)
    float sensitivity = baseSensitivity;
    if (params.sf == 6) sensitivity = -118.0f + 10 * log10(125E3 / params.bw);
    else if (params.sf == 7) sensitivity = -124.0f + 10 * log10(125E3 / params.bw);
    else if (params.sf == 8) sensitivity = -127.0f + 10 * log10(125E3 / params.bw);
    else if (params.sf == 9) sensitivity = -130.0f + 10 * log10(125E3 / params.bw);
    else if (params.sf == 10) sensitivity = -133.0f + 10 * log10(125E3 / params.bw);
    else if (params.sf == 11) sensitivity = -135.5f + 10 * log10(125E3 / params.bw);
    else if (params.sf == 12) sensitivity = -137.0f + 10 * log10(125E3 / params.bw);
    
    // Link budget calculation: Range ∝ 10^((TxPower - Sensitivity - LinkMargin)/20)
    float linkBudget = params.tp - sensitivity - 20.0f; // 20dB link margin
    float range = 1000.0f * pow(10, linkBudget / 40.0f); // Base 1km reference
    
    // Apply coding rate penalty (higher CR = longer packets = effective range reduction)
    float crPenalty = 1.0f - (params.cr - 5) * 0.05f;
    
    return range * crPenalty;
}

// Ground-level deployment with chip antenna optimization
LoraParams ConfigManager::getGroundLevelConfig(float range, float height_m) {
    LoraParams params = param;
    
    // Ground effect compensation - increase power for low heights
    float groundLoss_dB = 0;
    if (height_m < 0.5f) groundLoss_dB = 6.0f;      // 6dB loss below 0.5m
    else if (height_m < 1.0f) groundLoss_dB = 3.0f; // 3dB loss below 1.0m
    else groundLoss_dB = 0.0f;
    
    // Chip antenna compensation (-2dBi typical vs dipole)
    float chipAntennaLoss_dB = 4.0f; // 2dBi gain difference + 2dB efficiency loss
    
    // Total compensation needed
    float totalCompensation_dB = groundLoss_dB + chipAntennaLoss_dB;
    
    if (range <= 50) {
        // Very short range with ground compensation
        params.sf = 7;       // SF7 instead of SF6 for better sensitivity
        params.bw = 125E3;   // 125kHz for better penetration through vegetation
        params.tp = 5;       // 5dBm (+3dB compensation from normal 2dBm)
        params.cr = 5;       // 4/5 coding rate
        params.pl = 8;       // Longer preamble for better sync
    } else if (range <= 100) {
        // 100m range with chip antenna at ground level
        params.sf = 8;       // SF8 for better link margin
        params.bw = 125E3;   // 125kHz for vegetation penetration
        params.tp = 8;       // 8dBm (6dB higher than elevated config)
        params.cr = 5;       // 4/5 coding rate
        params.pl = 8;       // Standard preamble
    } else if (range <= 120) {
        // Extended ground-level range
        params.sf = 8;       // SF8 for margin
        params.bw = 125E3;   // 125kHz standard
        params.tp = 10;      // 10dBm for ground/chip antenna losses
        params.cr = 5;       // 4/5 coding rate
        params.pl = 12;      // Longer preamble for sync
    } else {
        // Use standard elevated configuration for longer ranges
        return getOptimalParamsForRange(range);
    }
    
    return params;
}

// Calculate range with ground effects and antenna type
float ConfigManager::calculateGroundRange(const LoraParams& params, float height_m, bool isChipAntenna) {
    float baseRange = calculateRange(params);
    
    // Ground effect loss
    float groundFactor = 1.0f;
    if (height_m < 0.5f) groundFactor = 0.25f;      // -6dB
    else if (height_m < 1.0f) groundFactor = 0.5f;  // -3dB
    else groundFactor = 1.0f;
    
    // Antenna type factor
    float antennaFactor = isChipAntenna ? 0.4f : 1.0f; // -4dB for chip antenna
    
    return baseRange * groundFactor * antennaFactor;
}

