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

    vector<string> query = {"CREATE TABLE IF NOT EXISTS States (timestamp TIMESTAMP, metric INTEGER, state INTEGER)"};
    
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
