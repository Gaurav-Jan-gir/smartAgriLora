#include "lora_params.h"
#include "thresholds.h"

class ConfigManager{
    private:
        LoraParams param;
        Thresholds thresholds;
    public:
        ConfigManager(){
            param.tp = 17;
            param.sf = 7;
            param.cr = 5;
            param.pl = 8;
            param.sw = 0x12;
            param.fr = 868E6;
            param.bw = 125E3;
        }
        void getOptimalParamsForRange(int range);
        void setParams(LoraParams p){param = p;}
        LoraParams getParams(){return param;}
        void setThresholds(Thresholds th){thresholds = th;}
        Thresholds getThresholds(){return thresholds;}

};