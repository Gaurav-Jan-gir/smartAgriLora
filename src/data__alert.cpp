#include "Arduino.h"

// Alert bit definitions for 16-bit alert code
#define ALERT_NONE                0x0000
#define ALERT_LOW_TEMP            0x0001
#define ALERT_HIGH_TEMP           0x0002
#define ALERT_LOW_HUMIDITY        0x0004
#define ALERT_HIGH_HUMIDITY       0x0008
#define ALERT_LOW_SOIL_MOISTURE   0x0010
#define ALERT_HIGH_SOIL_MOISTURE  0x0020
#define ALERT_LOW_BATTERY         0x0040
#define ALERT_SENSOR_FAILURE      0x0080
#define ALERT_COMM_FAILURE        0x0100
#define ALERT_CONFIG_ERROR        0x0200
#define ALERT_LOW_SIGNAL          0x0400
#define ALERT_MULTIPLE            0x0800
// Reserved bits: 0x1000, 0x2000, 0x4000, 0x8000 for future use

class AlertColor {
private:
  static const uint8_t NUM_BASE_ALERTS = 13;
  static const uint8_t NUM_COMBO_ALERTS = 15;
  
  // Base alert colors for individual alert types
  static const char baseColors[NUM_BASE_ALERTS][8] PROGMEM;
  static const char baseDescriptions[NUM_BASE_ALERTS][20] PROGMEM;
  
  // Common agricultural alert combinations with specific colors
  static const uint16_t comboAlerts[NUM_COMBO_ALERTS] PROGMEM;
  static const char comboColors[NUM_COMBO_ALERTS][8] PROGMEM;
  static const char comboDescriptions[NUM_COMBO_ALERTS][50] PROGMEM;
  
  // Priority order for single alerts (higher priority alerts override lower ones)
  static const uint16_t alertPriority[NUM_BASE_ALERTS] PROGMEM;

public:
  // Get hex color from 16-bit alert code
  static void getColorFromCode(uint16_t alertCode, char* buffer, size_t len) {
    if (alertCode == ALERT_NONE) {
      strncpy(buffer, "#000000", len);  // Black for no alert
      buffer[len - 1] = '\0';
      return;
    }
    
    // First check for predefined agricultural combinations
    for (uint8_t i = 0; i < NUM_COMBO_ALERTS; i++) {
      uint16_t combo = pgm_read_word(&comboAlerts[i]);
      if ((alertCode & combo) == combo) {  // Exact match for the combination
        strncpy_P(buffer, comboColors[i], len);
        buffer[len - 1] = '\0';
        return;
      }
    }
    
    // If no combination matches, use priority-based single alert color
    for (uint8_t i = 0; i < NUM_BASE_ALERTS; i++) {
      uint16_t priority = pgm_read_word(&alertPriority[i]);
      if (alertCode & priority) {
        strncpy_P(buffer, baseColors[i], len);
        buffer[len - 1] = '\0';
        return;
      }
    }
    
    // Fallback for unknown alert
    strncpy(buffer, "#FFFFFF", len);
    buffer[len - 1] = '\0';
  }

  // Get description from 16-bit alert code
  static void getDescriptionFromCode(uint16_t alertCode, char* buffer, size_t len) {
    if (alertCode == ALERT_NONE) {
      strncpy(buffer, "No Alert", len);
      buffer[len - 1] = '\0';
      return;
    }
    
    // First check for predefined agricultural combinations
    for (uint8_t i = 0; i < NUM_COMBO_ALERTS; i++) {
      uint16_t combo = pgm_read_word(&comboAlerts[i]);
      if ((alertCode & combo) == combo) {  // Exact match for the combination
        strncpy_P(buffer, comboDescriptions[i], len);
        buffer[len - 1] = '\0';
        return;
      }
    }
    
    // If no combination matches, build description from individual alerts
    buffer[0] = '\0';
    uint8_t alertCount = 0;
    
    for (uint8_t i = 0; i < NUM_BASE_ALERTS; i++) {
      uint16_t alertBit = pgm_read_word(&alertPriority[NUM_BASE_ALERTS - 1 - i]); // Reverse order for description
      if (alertCode & alertBit) {
        if (alertCount > 0) {
          strncat(buffer, " + ", len - strlen(buffer) - 1);
        }
        char desc[20];
        strncpy_P(desc, baseDescriptions[NUM_BASE_ALERTS - 1 - i], sizeof(desc));
        desc[sizeof(desc) - 1] = '\0';
        strncat(buffer, desc, len - strlen(buffer) - 1);
        alertCount++;
      }
    }
    
    buffer[len - 1] = '\0';
  }

  // Check if specific alert type is active
  static bool isAlertActive(uint16_t alertCode, uint16_t alertType) {
    return (alertCode & alertType) != 0;
  }
  
  // Add alert to existing alert code
  static uint16_t addAlert(uint16_t alertCode, uint16_t alertType) {
    return alertCode | alertType;
  }
  
  // Remove alert from existing alert code
  static uint16_t removeAlert(uint16_t alertCode, uint16_t alertType) {
    return alertCode & ~alertType;
  }
  
  // Get count of active alerts
  static uint8_t getAlertCount(uint16_t alertCode) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < 16; i++) {
      if (alertCode & (1 << i)) count++;
    }
    return count;
  }
};

// === Data stored in program memory ===

// Colors ordered by priority (highest priority first) - for single alerts
const char AlertColor::baseColors[AlertColor::NUM_BASE_ALERTS][8] PROGMEM = {
  "#FF0000", // ALERT_SENSOR_FAILURE    - Red (highest priority)
  "#C0C0C0", // ALERT_COMM_FAILURE      - Silver
  "#FF1493", // ALERT_CONFIG_ERROR      - DeepPink
  "#B22222", // ALERT_LOW_BATTERY       - Firebrick
  "#FF4500", // ALERT_HIGH_TEMP         - OrangeRed
  "#00BFFF", // ALERT_LOW_TEMP          - DeepSkyBlue
  "#006400", // ALERT_HIGH_HUMIDITY     - DarkGreen
  "#ADD8E6", // ALERT_LOW_HUMIDITY      - LightBlue
  "#228B22", // ALERT_HIGH_SOIL_MOISTURE- ForestGreen
  "#A0522D", // ALERT_LOW_SOIL_MOISTURE - Sienna
  "#9932CC", // ALERT_LOW_SIGNAL        - DarkOrchid
  "#800080", // ALERT_MULTIPLE          - Purple
  "#000000"  // ALERT_NONE              - Black (lowest priority)
};

const char AlertColor::baseDescriptions[AlertColor::NUM_BASE_ALERTS][20] PROGMEM = {
  "Sensor Failure",
  "Comm Failure", 
  "Config Error",
  "Low Battery",
  "High Temp",
  "Low Temp",
  "High Humidity",
  "Low Humidity", 
  "High Soil Moisture",
  "Low Soil Moisture",
  "Low Signal",
  "Multiple Alerts",
  "No Alert"
};

const uint16_t AlertColor::alertPriority[AlertColor::NUM_BASE_ALERTS] PROGMEM = {
  ALERT_SENSOR_FAILURE,
  ALERT_COMM_FAILURE,
  ALERT_CONFIG_ERROR,
  ALERT_LOW_BATTERY,
  ALERT_HIGH_TEMP,
  ALERT_LOW_TEMP,
  ALERT_HIGH_HUMIDITY,
  ALERT_LOW_HUMIDITY,
  ALERT_HIGH_SOIL_MOISTURE,
  ALERT_LOW_SOIL_MOISTURE,
  ALERT_LOW_SIGNAL,
  ALERT_MULTIPLE,
  ALERT_NONE
};

// Common agricultural alert combinations with specific colors
const uint16_t AlertColor::comboAlerts[AlertColor::NUM_COMBO_ALERTS] PROGMEM = {
  // Critical system combinations (highest priority)
  ALERT_SENSOR_FAILURE | ALERT_COMM_FAILURE,
  ALERT_SENSOR_FAILURE | ALERT_LOW_BATTERY,
  ALERT_COMM_FAILURE | ALERT_LOW_BATTERY,
  
  // Common summer conditions
  ALERT_HIGH_TEMP | ALERT_LOW_HUMIDITY,                    // Hot & dry summer
  ALERT_HIGH_TEMP | ALERT_LOW_HUMIDITY | ALERT_LOW_SOIL_MOISTURE, // Drought conditions
  ALERT_HIGH_TEMP | ALERT_HIGH_HUMIDITY,                  // Hot & humid summer
  
  // Common winter conditions  
  ALERT_LOW_TEMP | ALERT_HIGH_HUMIDITY,                   // Cold & wet winter
  ALERT_LOW_TEMP | ALERT_LOW_HUMIDITY,                    // Cold & dry winter
  
  // Soil moisture combinations
  ALERT_LOW_SOIL_MOISTURE | ALERT_LOW_HUMIDITY,           // Dry soil & air
  ALERT_HIGH_SOIL_MOISTURE | ALERT_HIGH_HUMIDITY,         // Wet soil & air (risk of fungal issues)
  
  // Battery + environmental alerts
  ALERT_LOW_BATTERY | ALERT_HIGH_TEMP,                    // Hot weather affecting battery
  ALERT_LOW_BATTERY | ALERT_LOW_TEMP,                     // Cold weather affecting battery
  
  // Triple environmental combinations
  ALERT_HIGH_TEMP | ALERT_HIGH_HUMIDITY | ALERT_HIGH_SOIL_MOISTURE, // Optimal growth but disease risk
  ALERT_LOW_TEMP | ALERT_HIGH_HUMIDITY | ALERT_HIGH_SOIL_MOISTURE,  // Cool wet conditions
  ALERT_HIGH_TEMP | ALERT_LOW_HUMIDITY | ALERT_LOW_SOIL_MOISTURE    // Severe drought
};

const char AlertColor::comboColors[AlertColor::NUM_COMBO_ALERTS][8] PROGMEM = {
  // Critical system combinations
  "#8B0000", // Sensor failure + comm failure - Dark red
  "#DC143C", // Sensor failure + low battery - Crimson  
  "#696969", // Comm failure + low battery - Dim gray
  
  // Summer conditions
  "#FF8C00", // High temp + low humidity - Dark orange (summer heat)
  "#FF6347", // High temp + low humidity + low soil - Tomato (drought)
  "#8B4513", // High temp + high humidity - Saddle brown (humid heat)
  
  // Winter conditions
  "#4682B4", // Low temp + high humidity - Steel blue (winter wet)
  "#5F9EA0", // Low temp + low humidity - Cadet blue (winter dry)
  
  // Soil moisture combinations
  "#D2691E", // Low soil + low humidity - Chocolate (dry conditions)
  "#2E8B57", // High soil + high humidity - Sea green (wet conditions)
  
  // Battery + environmental
  "#CD853F", // Low battery + high temp - Peru (hot battery drain)
  "#708090", // Low battery + low temp - Slate gray (cold battery drain)
  
  // Triple combinations
  "#32CD32", // High temp + high humidity + high soil - Lime green (growth conditions)
  "#20B2AA", // Low temp + high humidity + high soil - Light sea green (cool wet)
  "#B22222"  // High temp + low humidity + low soil - Fire brick (severe drought)
};

const char AlertColor::comboDescriptions[AlertColor::NUM_COMBO_ALERTS][50] PROGMEM = {
  // Critical system combinations
  "System Failure",
  "Critical Low Power",
  "Communication Down",
  
  // Summer conditions
  "Hot Dry Summer",
  "Drought Conditions", 
  "Hot Humid Weather",
  
  // Winter conditions
  "Cold Wet Winter",
  "Cold Dry Winter",
  
  // Soil moisture combinations
  "Dry Soil & Air",
  "Wet Conditions - Disease Risk",
  
  // Battery + environmental
  "Hot Weather - Low Battery",
  "Cold Weather - Low Battery",
  
  // Triple combinations
  "Optimal Growth - Disease Risk",
  "Cool Wet Conditions",
  "Severe Drought"
};
