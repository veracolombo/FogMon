#include "adaptive_storage_monitoring.hpp"
#include "string.h"
#include <vector>
#include <sstream>

using namespace std;

AdaptiveStorageMonitoring::AdaptiveStorageMonitoring() : Storage() { }
AdaptiveStorageMonitoring::~AdaptiveStorageMonitoring() { }

void AdaptiveStorageMonitoring::createTables(){
    Storage::createTables();

    char *zErrMsg = 0;
    
    vector<string> query = {"CREATE TABLE IF NOT EXISTS Other_metrics (time TIMESTAMP PRIMARY KEY, battery REAL)"};
    
    for(string str : query) {
        int err = sqlite3_exec(this->db, str.c_str(), 0, 0, &zErrMsg);
        isError(err, zErrMsg, "createTablesAdaptive");
    }
}

AdaptiveReport::battery_result AdaptiveStorageMonitoring::getBattery() {
    char *zErrMsg = 0;
    char buf[1024];
    std::sprintf(buf, "SELECT avg(battery) AS mean_battery, variance(battery) AS var_battery, strftime('%%s','now') as lasttime FROM Other_metrics");

    AdaptiveReport::battery_result r;
    memset(&r,0,sizeof(AdaptiveReport::battery_result));

    int err = sqlite3_exec(this->db, buf, IAdaptiveStorageMonitoring::getBatteryCallback, &r, &zErrMsg);
    isError(err, zErrMsg, "getBattery");

    return r;
}

vector<float> AdaptiveStorageMonitoring::getLastValues(Metric metric, int limit){

    char *zErrMsg = 0;
    char buf[1024];
    stringstream query;

    if(metric == Metric::FREE_CPU){
        query << "SELECT free_cpu FROM Hardware ORDER BY time DESC LIMIT " << limit;

    }else if(metric == Metric::FREE_MEMORY){
        query << "SELECT CASE WHEN memory = 0 THEN 0 ELSE free_memory/memory END FROM Hardware ORDER BY time DESC LIMIT " << limit;

    }else if(metric == Metric::FREE_DISK){
        query << "SELECT CASE WHEN disk = 0 THEN 0 ELSE free_disk/disk END FROM Hardware ORDER BY time DESC LIMIT " << limit; 

    }else if(metric == Metric::BATTERY){
        query << "SELECT battery FROM Other_metrics ORDER BY time DESC LIMIT " << limit;
    }

    std::sprintf(buf, query.str().c_str());

    vector<float> r;
    int err = sqlite3_exec(this->db, buf, IStorage::VectorFloatCallback, &r, &zErrMsg);
    isError(err, zErrMsg, "getLastValues");

    return r;
}

void AdaptiveStorageMonitoring::saveBattery(AdaptiveReport::battery_result battery, int window) {
    char *zErrMsg = 0;
    char buf[1024];
    std::sprintf(buf,"INSERT INTO Other_metrics (time, battery) VALUES (DATETIME('now'), %f)", battery.mean_battery);

    int err = sqlite3_exec(this->db, buf, 0, 0, &zErrMsg);
    isError(err, zErrMsg, "saveBattery1");

    std::sprintf(buf,"DELETE FROM Other_metrics WHERE time <= (SELECT time FROM Other_metrics ORDER BY time DESC LIMIT 1 OFFSET %d)", window);

    err = sqlite3_exec(this->db, buf, 0, 0, &zErrMsg);
    isError(err, zErrMsg, "saveBattery2");
}