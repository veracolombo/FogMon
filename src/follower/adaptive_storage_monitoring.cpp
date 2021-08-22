#include "adaptive_storage_monitoring.hpp"
#include <vector>
#include "string.h"

using namespace std;

AdaptiveStorageMonitoring::AdaptiveStorageMonitoring() : Storage() { }
AdaptiveStorageMonitoring::~AdaptiveStorageMonitoring() { }

void AdaptiveStorageMonitoring::createTables(){
    this->call_superclass_create_tables();

    char *zErrMsg = 0;
    
    vector<string> query = {"CREATE TABLE IF NOT EXISTS Other_metrics (time TIMESTAMP PRIMARY KEY, battery REAL)"};
    
    for(string str : query) {
        int err = sqlite3_exec(this->db, str.c_str(), 0, 0, &zErrMsg);
        isError(err, zErrMsg, "createTablesAdaptive");
    }
}

void AdaptiveStorageMonitoring::call_superclass_create_tables(){
    Storage::createTables();
}