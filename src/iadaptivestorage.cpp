#include "iadaptivestorage.hpp"
#include <string.h>
#include <tuple>

using namespace std;

IAdaptiveStorage::~IAdaptiveStorage() { }

void IAdaptiveStorage::start() {
    this->workerThread = thread(&IAdaptiveStorage::worker, this);
    this->running = true;
}

void IAdaptiveStorage::stop() {
    this->running = false;
    if(this->workerThread.joinable())
    {
        this->workerThread.join();
    }
}

void IAdaptiveStorage::worker() {
    while(this->running.load()) {
        function<void()> predicate;
        if(!this->queue.pop(&predicate))
            break;
        predicate();
    }
    this->running = false;
}

void IAdaptiveStorage::open(string path) {
    int err = sqlite3_open(path.c_str(), &(this->db));
    if( err ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(1);
    }

    char *zErrMsg = 0;

    err = sqlite3_exec(this->db, "PRAGMA journal_mode = WAL", 0, 0, &zErrMsg);
    if( err!=SQLITE_OK )
    {
        fprintf(stderr, "SQL error (pragme journal): %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        exit(1);
    } 

    err = sqlite3_exec(this->db, "PRAGMA synchronous = NORMAL", 0, 0, &zErrMsg);
    if( err!=SQLITE_OK )
    {
        fprintf(stderr, "SQL error (pragma sync): %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        exit(1);
    } 

    sqlite3_enable_load_extension(db, 1);
    

    err = sqlite3_exec(this->db, "SELECT load_extension('./libsqlitefunctions')", 0, 0, &zErrMsg);
    if( err!=SQLITE_OK )
    {
        fprintf(stderr, "SQL error (load extension): %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
        exit(1);
    }   
    

    this->createTables();
}

void IAdaptiveStorage::close() {
    if(this->db != nullptr)
        sqlite3_close(this->db);
    this->db = nullptr;
}

int IAdaptiveStorage::_getStatesCallback(void *vec, int argc, char **argv, char **azColName){
    vector<tuple<string, int, int>> *v = (vector<tuple<string, int, int>>*)vec;
    tuple<string, int, int> state;

    state = make_tuple(string(argv[0]), stoi(argv[1]), stoi(argv[2]));
  
    v->push_back(state);

   return 0;
}

int IAdaptiveStorage::_getIntCallback(void *i, int argc, char **argv, char **azColName) {
    int *val = (int*)i;
    *val = stoi(argv[0]);
    return 0;
}