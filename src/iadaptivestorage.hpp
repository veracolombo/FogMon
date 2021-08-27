#ifndef IADAPTIVESTORAGE_HPP_
#define IADAPTIVESTORAGE_HPP_

#include <sqlite3.h>
#include <string>
#include <map>
#include "common.hpp"
#include "message.hpp"
#include "report.hpp"
#include "ithing.hpp"
#include "queue.hpp"
#include <thread>
#include <atomic>
#include <functional>
#include <utility>
#include <vector>

using namespace std;

/**
 * An sqlite3 abstract class for the database
*/
class IAdaptiveStorage {

public:
    virtual ~IAdaptiveStorage();

    /**
     * open the database
     * @param path the location of the database
    */
    void open(std::string path);
    /**
     * close the database
    */
    void close();

    /**
     * Start the listener thread
    */
    void start();

    /**
     * Stop the thread
    */
    void stop();

    virtual vector<tuple<string, int, int>> getStates() = 0;

    virtual void saveStates(vector<State> states, Metric metric) = 0;

protected:

    /**
     * create the tables for this database
    */
    virtual void createTables() = 0;

    static int _getStatesCallback(void *data, int argc, char **argv, char **azColName);
    static int _getIntCallback(void *i, int argc, char **argv, char **azColName);
    static int _VectorFloatCallback(void *vec, int argc, char **argv, char **azColName);

    /**
     * 
    */
    virtual void worker();

    /**
     * the sqlite3 connection
    */
    sqlite3 *db;

    Queue<std::function<void(void)>> queue;
    std::atomic_bool running = false;
    /**
     * thread worker for the database
    */
    std::thread workerThread;

};

#endif