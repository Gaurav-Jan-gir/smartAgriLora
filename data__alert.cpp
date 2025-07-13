#include "Arduino.h"

class AlertColor {
private:
  static const uint8_t NUM_ALERTS = 27;

  // Indexed array of color hex codes with comments
  const String colorList[NUM_ALERTS] = {
    "#000000", // 0  - Black         - No Alert
    "#00BFFF", // 1  - DeepSkyBlue   - Low Temperature
    "#FF4500", // 2  - OrangeRed     - High Temperature
    "#ADD8E6", // 3  - LightBlue     - Low Humidity
    "#006400", // 4  - DarkGreen     - High Humidity
    "#A0522D", // 5  - Sienna        - Low Soil Moisture
    "#228B22", // 6  - ForestGreen   - High Soil Moisture
    "#B22222", // 7  - Firebrick     - Low Battery
    "#FFD700", // 8  - Gold          - Sensor Failure
    "#C0C0C0", // 9  - Silver        - Communication Failure
    "#FF1493", // 10 - DeepPink      - Configuration Error

    // Dual Alerts
    "#5F9EA0", // 11 - CadetBlue     - Low Temp + Low Humidity
    "#4682B4", // 12 - SteelBlue     - Low Temp + High Humidity
    "#FF8C00", // 13 - DarkOrange    - High Temp + Low Humidity
    "#8B0000", // 14 - DarkRed       - High Temp + High Humidity
    "#6A5ACD", // 15 - SlateBlue     - Low Temp + Low Moisture
    "#20B2AA", // 16 - LightSeaGreen - Low Temp + High Moisture
    "#FF6347", // 17 - Tomato        - High Temp + Low Moisture
    "#B22222", // 18 - Firebrick     - High Temp + High Moisture
    "#8B4513", // 19 - SaddleBrown   - Low Humidity + Low Moisture
    "#2E8B57", // 20 - SeaGreen      - High Humidity + High Moisture

    // Triple Alerts
    "#8B008B", // 21 - DarkMagenta   - High Temp + High Humidity + Low Battery
    "#708090", // 22 - SlateGray     - Low Temp + Low Humidity + Low Battery
    "#FF0000", // 23 - Red           - High Temp + Low Humidity + Low Moisture
    "#1E90FF", // 24 - DodgerBlue    - Low Temp + High Humidity + High Moisture

    // Special / Extended Alerts
    "#9932CC", // 25 - DarkOrchid    - Low Signal Strength
    "#800080"  // 26 - Purple        - Multiple Alerts
  };

public:
  // Get hex color from alert code (0–26)
  String getColorFromCode(uint8_t code) {
    if (code < NUM_ALERTS) return colorList[code];
    return "#FFFFFF"; // fallback for unknown code
  }

  // Optional: Get description of alert
  String getDescriptionFromCode(uint8_t code) {
    switch (code) {
      case 0: return "No Alert";
      case 1: return "Low Temperature";
      case 2: return "High Temperature";
      case 3: return "Low Humidity";
      case 4: return "High Humidity";
      case 5: return "Low Soil Moisture";
      case 6: return "High Soil Moisture";
      case 7: return "Low Battery";
      case 8: return "Sensor Failure";
      case 9: return "Communication Failure";
      case 10: return "Configuration Error";

      case 11: return "Low Temp + Low Humidity";
      case 12: return "Low Temp + High Humidity";
      case 13: return "High Temp + Low Humidity";
      case 14: return "High Temp + High Humidity";
      case 15: return "Low Temp + Low Moisture";
      case 16: return "Low Temp + High Moisture";
      case 17: return "High Temp + Low Moisture";
      case 18: return "High Temp + High Moisture";
      case 19: return "Low Humidity + Low Moisture";
      case 20: return "High Humidity + High Moisture";

      case 21: return "High Temp + High Humidity + Low Battery";
      case 22: return "Low Temp + Low Humidity + Low Battery";
      case 23: return "High Temp + Low Humidity + Low Moisture";
      case 24: return "Low Temp + High Humidity + High Moisture";

      case 25: return "Low Signal Strength";
      case 26: return "Multiple Alerts";

      default: return "Unknown Alert";
    }
  }
};
