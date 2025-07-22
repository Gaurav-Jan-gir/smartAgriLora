#include "lora_params.h"
#include "thresholds.h"

class ConfigManager{
    private:
        LoraParams param;
        Thresholds thresholds;
        
        // Private helper functions
        LoraParams getDefaultParams();
        Thresholds getDefaultThresholds();
        bool validateParams(const LoraParams& params);
        bool validateThresholds(const Thresholds& thresholds);
        
    public:
        ConfigManager(){
            param.tp = 17;
            param.sf = 7;
            param.cr = 5;
            param.pl = 8;
            param.sw = 0x12;
            param.fr = 865E6;  // 865 MHz for India
            param.bw = 125E3;
        }
        
        // Existing functions - optimized with references
        LoraParams getOptimalParamsForRange(float range);
        void setParams(const LoraParams& p);
        const LoraParams& getParams() const;  // Return by const reference
        void setThresholds(const Thresholds& th);
        const Thresholds& getThresholds() const;  // Return by const reference
        
        // Additional function declarations
        void resetToDefaults();
        float calculateRange(const LoraParams& params);
};