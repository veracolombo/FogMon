#include "adaptive_leader_storage_monitoring.hpp"
#include <string.h>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

AdaptiveLeaderStorageMonitoring::AdaptiveLeaderStorageMonitoring(Message::node node) : LeaderStorage(node), AdaptiveStorageMonitoring(), Storage() { }
AdaptiveLeaderStorageMonitoring::~AdaptiveLeaderStorageMonitoring() { }


void AdaptiveLeaderStorageMonitoring::createTables(){
    AdaptiveStorageMonitoring::createTables();

    char *zErrMsg = 0;
    
    vector<string> query = {"CREATE TABLE IF NOT EXISTS MMNodes (id STRING PRIMARY KEY, ip STRING NOT NULL, port STRING NOT NULL, UNIQUE(ip, port))",
                            "CREATE TABLE IF NOT EXISTS MNodes (id STRING PRIMARY KEY, ip STRING NOT NULL, port STRING NOT NULL, cores INTEGER, mean_free_cpu REAL, var_free_cpu REAL, memory INTEGER, mean_free_memory FLOAT, var_free_memory FLOAT, disk INTEGER, mean_free_disk FLOAT, var_free_disk FLOAT, mean_battery FLOAT, var_battery FLOAT, lasttime TIMESTAMP, monitoredBy STRING REFERENCES MMNodes(id) NOT NULL, UNIQUE(ip, port))",
                            "CREATE TABLE IF NOT EXISTS MLinks (idA STRING REFERENCES MNodes(id) NOT NULL, idB STRING REFERENCES MNodes(id) NOT NULL, meanL FLOAT, varianceL FLOAT, lasttimeL TIMESTAMP, meanB FLOAT, varianceB FLOAT, lasttimeB TIMESTAMP, PRIMARY KEY(idA,idB))",
                            "CREATE TABLE IF NOT EXISTS MIots (id STRING PRIMARY KEY, desc STRING, ms INTEGER, idNode STRING REFERENCES MNodes(id) NOT NULL)",
                            "DELETE FROM MMNodes",
                            string("INSERT OR IGNORE INTO MMNodes (id, ip, port) VALUES (\"")+ this->nodeM.id+ string("\", \"::1\", \""+ this->nodeM.port +"\")")};
    
    for(string str : query) {
        int err = sqlite3_exec(this->db, str.c_str(), 0, 0, &zErrMsg);
        isError(err, zErrMsg, "createTablesAdaptiveLeader");
    }
}

vector<AdaptiveReport::adaptive_report_result> AdaptiveLeaderStorageMonitoring::getAdaptiveReport(bool complete) {
    //sqlite3_exec(db, "BEGIN TRANSACTION;", NULL, NULL, NULL);
    vector<Message::node> nodes = this->getAllNodes();
    vector<AdaptiveReport::adaptive_report_result> res;

    for(auto node : nodes) {
        res.push_back(this->getAdaptiveReport(node, complete));
    }
    //sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
    return res;
}

AdaptiveReport::adaptive_report_result AdaptiveLeaderStorageMonitoring::getAdaptiveReport(Message::node node, bool complete) {
    Report::report_result r = LeaderStorage::getReport(node, complete);

    AdaptiveReport::adaptive_report_result ar;
    ar.source = r.source;
    ar.hardware = r.hardware;
    ar.latency = r.latency;
    ar.bandwidth = r.bandwidth;
    ar.leader = r.leader;
    ar.battery = this->getBattery(ar.source);

    return ar;
}

/*
std::string AdaptiveLeaderStorageMonitoring::addNode(Message::node node, AdaptiveReport::hardware_result hardware, AdaptiveReport::battery_result battery, Message::node *monitored) {
    
    if(hardware.lasttime == 0 || battery.lasttime == 0) { //these are directly measured
        return "";
    }

    char *zErrMsg = 0;
    char buf[1024];
    vector<long long> res;
    std::sprintf(buf,"SELECT strftime('%%s',lasttime) FROM MNodes WHERE (strftime('%%s',lasttime)-%" PRId64" > 0) AND (id = \"%s\") ", hardware.lasttime, node.id.c_str());
    int err = sqlite3_exec(this->db, buf, IStorage::VectorIntCallback, &res, &zErrMsg);
    isError(err, zErrMsg, "addNodeAdaptiveLeader0");

    if(!res.size()) {
        stringstream query;
        query <<    "INSERT OR REPLACE INTO MNodes"
                    " (id, ip, port, cores, mean_free_cpu, var_free_cpu, memory, mean_free_memory, var_free_memory, disk, mean_free_disk, var_free_disk, mean_battery, var_battery, lasttime, monitoredBy)"
                    " VALUES (\""<< node.id <<"\", \""<< node.ip <<"\", \""<< node.port <<"\", "<<
                        hardware.cores <<", "<< hardware.mean_free_cpu <<", "<< hardware.var_free_cpu <<", "<<
                        hardware.memory <<", "<< hardware.mean_free_memory <<", "<< hardware.var_free_memory <<", "<<
                        hardware.disk <<", "<< hardware.mean_free_disk <<", "<< hardware.var_free_disk <<", "<< battery.mean_battery <<", "<< battery.var_battery <<", DATETIME("<< hardware.lasttime <<",\"unixepoch\"),";
        if(!monitored) {
            query << " \"" << this->nodeM.id <<"\")";
        }else {
            // block report about this leader from other nodes
            if (monitored->id == this->nodeM.id){
                return "";
            }
            query << " \""<< monitored->id <<"\")";
            //printf("report: %s --> %s lasttime: %ld\n",monitored->id.c_str(), node.ip.c_str(),hardware.lasttime);
        }
        int err = sqlite3_exec(this->db, query.str().c_str(), 0, 0, &zErrMsg);
        isError(err, zErrMsg, "addNodeAdaptiveLeader1");
        return node.id;
    }
    return "";
}
*/

AdaptiveReport::battery_result AdaptiveLeaderStorageMonitoring::getBattery(Message::node node) {
    char *zErrMsg = 0;
    char buf[1024];
    std::sprintf(buf,"SELECT *, (CASE WHEN \"%s\" = N.monitoredBy THEN strftime('%%s','now') ELSE strftime('%%s',lasttime) END) as lasttime FROM MNodes as N WHERE id = \"%s\" GROUP BY ip", this->nodeM.id.c_str(),node.id.c_str());

    AdaptiveReport::battery_result r(-1,0,0);

    int err = sqlite3_exec(this->db, buf, IAdaptiveStorageMonitoring::getBatteryCallback, &r, &zErrMsg);
    isError(err, zErrMsg, "getBatteryLeader");

    return r;
}