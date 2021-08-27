#include "iadaptive_storage_monitoring.hpp"
#include "string.h"

using namespace std;

int IAdaptiveStorageMonitoring::getBatteryCallback(void *R, int argc, char **argv, char **azColName) {
    AdaptiveReport::battery_result *r = (AdaptiveReport::battery_result*)R;
    for(int i=0; i<argc; i++) {
        if(strcmp("mean_battery", azColName[i])==0) {
            r->mean_battery = stof(argv[i]);
        }else if(strcmp("var_battery", azColName[i])==0) {
            r->var_battery = stof(argv[i]);
        }else if(strcmp("lasttime", azColName[i])==0) {
            r->lasttime = stoll(argv[i]);
        }
    }
    return 0;
}