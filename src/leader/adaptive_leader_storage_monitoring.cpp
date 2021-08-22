#include "adaptive_leader_storage_monitoring.hpp"

AdaptiveLeaderStorageMonitoring::AdaptiveLeaderStorageMonitoring(Message::node node) : LeaderStorage(node), AdaptiveStorageMonitoring(), Storage() { }
AdaptiveLeaderStorageMonitoring::~AdaptiveLeaderStorageMonitoring() { }

void AdaptiveLeaderStorageMonitoring::createTables(){
    LeaderStorage::createTables();
    AdaptiveStorageMonitoring::createTables();
}

void AdaptiveLeaderStorageMonitoring::call_superclass_create_tables() { }