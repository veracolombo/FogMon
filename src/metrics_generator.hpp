#ifndef METRICS_GENERATOR_HPP_
#define METRICS_GENERATOR_HPP_

#include "common.hpp"
#include "sleeper.hpp"

#include <vector>
#include <map>
#include <thread>

using namespace std;

class MetricsGenerator {

public:
    enum Trend {trSTABLE, trUNSTABLE, trCONSTANT, trINCREASING, trDECREASING};

    MetricsGenerator();
    ~MetricsGenerator();

    struct series_data {
        vector<float> free_cpu;

        vector<int64_t> total_memory;
        vector<float> free_memory;

        vector<int64_t> total_disk;
        vector<float> free_disk;

        vector<float> battery; 
    };

    struct currentVal_data {
        float free_cpu;

        int64_t total_memory;
        float free_memory;

        int64_t total_disk;
        float free_disk;

        float battery;
    };

    void start();
    void stop();

    void initialize();

    static map<Metric, Trend> trends;
    static currentVal_data currentVal;
    static map<Trend, series_data> series;

private:
    bool running;

    thread cpuThread;
    thread memoryThread;
    thread diskThread;
    thread batteryThread;

    void cpu();
    void memory();
    void disk();
    void battery();

    Sleeper sleeper;
};

#endif