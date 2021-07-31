#include "iagent.hpp"
#include <string.h>
#include <sstream>
#include <iostream>
#include <inttypes.h>
#include "adaptive_storage.hpp"

using namespace std;

AdaptiveStorage::AdaptiveStorage() { }

AdaptiveStorage::~AdaptiveStorage() { }

void AdaptiveStorage::isError(int err, char *zErrMsg, std::string mess) {
    if( err!=SQLITE_OK )
    {
        cerr << "[" << mess << "] SQL error: " << zErrMsg << endl; 
        fflush(stderr);
        sqlite3_free(zErrMsg);
        exit(1);
    }
}

void AdaptiveStorage::createTables() {
    char *zErrMsg = 0;

    vector<string> query = {"CREATE TABLE IF NOT EXISTS States (timestamp TIMESTAMP, metric INTEGER, state INTEGER)",
                            "CREATE TABLE IF NOT EXISTS CPU (time TIMESTAMP, cores INTEGER, free_cpu REAL)",
                            "CREATE TABLE IF NOT EXISTS Memory (time TIMESTAMP, memory INTEGER, free_memory REAL)",
                            "CREATE TABLE IF NOT EXISTS Disk (time, TIMESTAMP, disk INTEGER, free_disk REAL)"};
    
    for(string str : query) {
        int err = sqlite3_exec(this->db, str.c_str(), 0, 0, &zErrMsg);
        isError(err, zErrMsg, "createTables");
    }
}

vector<tuple<string, int, int>> AdaptiveStorage::getStates(){
    char *zErrMsg = 0;
    char buf[1024];

    stringstream query;
    query << "SELECT * FROM States ORDER BY timestamp DESC";
    std::sprintf(buf, query.str().c_str());

    vector<tuple<string, int, int>> vect;

    int err = sqlite3_exec(this->db, buf, IAdaptiveStorage::_getStatesCallback, &vect, &zErrMsg);

    isError(err, zErrMsg, "getStates");

    return vect;
}


void AdaptiveStorage::saveStates(vector<State> states, Metric metric){
    char *zErrMsg = 0;
    char buf[1024];

    int err;
    stringstream query;

    for(int i=0; i<states.size(); i++){
        query.str("");
        query << "INSERT INTO States (timestamp, metric, state) VALUES (DATETIME('now'), " << metric << ", " << states[i] << ")";
        std::sprintf(buf, query.str().c_str());
        
        err = sqlite3_exec(this->db, buf, 0, 0, &zErrMsg);
        isError(err, zErrMsg, "saveStates1");
    }

    query.str("");
    query << "SELECT COUNT (DISTINCT timestamp) FROM States";
    std::sprintf(buf, query.str().c_str());

    int ntimes;
    err = sqlite3_exec(this->db, buf, _getIntCallback, &ntimes, &zErrMsg);
    isError(err, zErrMsg, "saveStates2");

    int window = 10;
    if(ntimes > window){
        query.str("");
        query << "DELETE FROM States WHERE timestamp = (SELECT MIN(timestamp) FROM States)";
        std::sprintf(buf, query.str().c_str());

        err = sqlite3_exec(this->db, buf, 0, 0, &zErrMsg);
        isError(err, zErrMsg, "saveStates3");
    }
}

vector<float> AdaptiveStorage::getFreeCpu(int history){
    char *zErrMsg = 0;
    char buf[1024];

    stringstream query;
    query << "SELECT free_cpu FROM CPU ORDER BY time DESC LIMIT " << history;
    std::sprintf(buf, query.str().c_str());

    vector<float> r;
    int err = sqlite3_exec(this->db, buf, IAdaptiveStorage::_VectorFloatCallback, &r, &zErrMsg);

    isError(err, zErrMsg, "getFreeCpu");

    return r;
}

vector<float> AdaptiveStorage::getFreeMemory(int history){
    char *zErrMsg = 0;
    char buf[1024];

    stringstream query;
    query << "SELECT free_memory/memory FROM Memory ORDER BY time DESC LIMIT " << history;
    std::sprintf(buf, query.str().c_str());

    vector<float> r;
    int err = sqlite3_exec(this->db, buf, IAdaptiveStorage::_VectorFloatCallback, &r, &zErrMsg);

    isError(err, zErrMsg, "getFreeMemory");
    
    return r;
}

vector<float> AdaptiveStorage::getFreeDisk(int history){
    char *zErrMsg = 0;
    char buf[1024];

    stringstream query;
    query << "SELECT free_disk/disk FROM Disk ORDER BY time DESC LIMIT " << history;
    std::sprintf(buf, query.str().c_str());

    vector<float> r;
    int err = sqlite3_exec(this->db, buf, IAdaptiveStorage::_VectorFloatCallback, &r, &zErrMsg);

    isError(err, zErrMsg, "getFreeDisk");
    
    return r;
}


void AdaptiveStorage::saveCpu(int cores, float free_cpu, int window){
    char *zErrMsg = 0;
    char buf[1024];

    stringstream query;
    query << "INSERT INTO CPU (time, cores, free_cpu) VALUES (DATETIME('now'), " << cores << ", " << free_cpu << ")";
    std::sprintf(buf, query.str().c_str());

    int err = sqlite3_exec(this->db, buf, 0, 0, &zErrMsg);
    isError(err, zErrMsg, "saveCpu1");

    query.str("");
    query << "DELETE FROM CPU WHERE time <= (SELECT time from CPU ORDER BY time DESC LIMIT 1 OFFSET " << window << ")";
    std::sprintf(buf, query.str().c_str());

    err = sqlite3_exec(this->db, buf, 0, 0, &zErrMsg);
    isError(err, zErrMsg, "saveCpu2");
}

void AdaptiveStorage::saveMemory(int64_t memory, float free_memory, int window){
    char *zErrMsg = 0;
    char buf[1024];

    stringstream query;
    query << "INSERT INTO Memory (time, memory, free_memory) VALUES (DATETIME('now'), " << memory << ", " << free_memory << ")";
    std::sprintf(buf, query.str().c_str());

    int err = sqlite3_exec(this->db, buf, 0, 0, &zErrMsg);
    isError(err, zErrMsg, "saveMemory1");

    query.str("");
    query << "DELETE FROM Memory WHERE time <= (SELECT time FROM Memory ORDER BY time DESC LIMIT 1 OFFSET " << window << ")";
    std::sprintf(buf, query.str().c_str());

    err = sqlite3_exec(this->db, buf, 0, 0, &zErrMsg);
    isError(err, zErrMsg, "saveMemory2");
}

void AdaptiveStorage::saveDisk(int64_t disk, float free_disk, int window){
    char *zErrMsg = 0;
    char buf[1024];

    stringstream query;
    query << "INSERT INTO Disk (time, disk, free_disk) VALUES (DATETIME('now'), " << disk << ", " << free_disk << ")";
    std::sprintf(buf, query.str().c_str());

    int err = sqlite3_exec(this->db, buf, 0, 0, &zErrMsg);
    isError(err, zErrMsg, "saveDisk1");

    query.str("");
    query << "DELETE FROM Disk WHERE time <= (SELECT time FROM Disk ORDER BY time DESC LIMIT 1 OFFSET " << window << ")";
    std::sprintf(buf, query.str().c_str());

    err = sqlite3_exec(this->db, buf, 0, 0, &zErrMsg);
    isError(err, zErrMsg, "saveDisk2");
}

vector<float> AdaptiveStorage::getLastValues(Metric metric, int history) {
    switch(metric){
        case(FREE_CPU):     {return this->getFreeCpu(history);}
        case(FREE_MEMORY):  {return this->getFreeMemory(history);}
        case(FREE_DISK):    {return this->getFreeDisk(history);}
    }
}