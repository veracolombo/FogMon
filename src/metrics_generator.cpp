#include "metrics_generator.hpp"
#include "adaptive_follower.hpp"
#include <iostream>


map<Metric, MetricsGenerator::Trend> MetricsGenerator::trends = {

    {FREE_CPU, MetricsGenerator::Trend::trSTABLE},

    //{TOTAL_MEMORY, MetricsGenerator::Trend::trCONSTANT},
    {FREE_MEMORY, MetricsGenerator::Trend::trSTABLE},

    //{TOTAL_DISK, MetricsGenerator::trCONSTANT},
    {FREE_DISK, MetricsGenerator::Trend::trSTABLE},

    {BATTERY, MetricsGenerator::Trend::trSTABLE}

};

map<MetricsGenerator::Trend, MetricsGenerator::series_data> MetricsGenerator::series = {

    {MetricsGenerator::Trend::trSTABLE, 
            { 
              { 0.80, 0.85 },
              //{ 4000000 },
              { 3500000.0, 3400000.0, 3600000.0, 3550000.0 },
              //{ 225000000 },
              { 100000000.0, 150000000.0, 125000000.0 },
              { 0.80, 0.85 } 
            }
    },

    {MetricsGenerator::Trend::trUNSTABLE,
        {
            { 0.60, 0.80, 0.60, 0.90 },
            //{ 4000000 },
            { 3500000.0, 2500000.0, 300000.0, 1500000.0},
            //{ 225000000 },
            { 100000000.0, 200000000.0, 90000000.0},
            { 0.70, 0.55, 0.80, 0.95}
        }
    }
    /*
    {MetricsGenerator::Trend::trCONSTANT, 
            { 
              { 0.80 },
              { 4000000 },
              { 3500000.0 },
              { 225000000 },
              { 100000000.0 },
              { 0.80 } 
            }
    }
    */
};

MetricsGenerator::currentVal_data MetricsGenerator::currentVal = {

    0.0,

    //0,
    0.0,

    //0,
    0.0,

    0.0
};

MetricsGenerator::MetricsGenerator(AdaptiveFollower* node) {
    this->running = false;
    this->node = node;
}
MetricsGenerator::~MetricsGenerator() { }

void MetricsGenerator::initialize(vector<Metric> metrics) {
    this->trends.clear();

    for(auto &m : metrics){
        Trend trend = static_cast<Trend>(rand() % last);
        this->trends[m] = trend;
    }
}

void MetricsGenerator::start() {
    this->running = true;

    map<Metric, bool> metrics = this->node->metrics;

    if(metrics[FREE_CPU]){
        this->cpuThread = thread(&MetricsGenerator::cpu, this);
    }

    if(metrics[FREE_MEMORY]){
        this->memoryThread = thread(&MetricsGenerator::memory, this);
    }

    if(metrics[FREE_DISK]){
        this->diskThread = thread(&MetricsGenerator::disk, this);
    }

    if(metrics[BATTERY]){
        this->batteryThread = thread(&MetricsGenerator::battery, this);
    }
}

void MetricsGenerator::stop() {
    this->running = false;
    if(this->cpuThread.joinable()){
        this->cpuThread.join();
    }
    if(this->memoryThread.joinable()){
        this->memoryThread.join();
    }
    if(this->diskThread.joinable()){
        this->diskThread.join();
    }
    if(this->batteryThread.joinable()){
        this->batteryThread.join();
    }
}

void MetricsGenerator::cpu(){
    Trend trend = trends.at(FREE_CPU);

    int idx = 0;

    while(this->running){

        auto t_start = std::chrono::high_resolution_clock::now();

        if(trend == trSTABLE){
            currentVal.free_cpu = series.at(trSTABLE).free_cpu[idx];
            
            idx++;

            if(idx == series.at(trSTABLE).free_cpu.size()) 
                idx = 0;
        } else if(trend == trUNSTABLE){
            currentVal.free_cpu = series.at(trUNSTABLE).free_cpu[idx];

            idx++;

            if(idx == series.at(trUNSTABLE).free_cpu.size())
                idx = 0;
        }/* else if(trend == trCONSTANT){
            currentVal.free_cpu = series.at(trUNSTABLE).free_cpu[idx];

            idx++;

            if(idx == series.at(trCONSTANT).free_cpu.size())
                idx = 0;
        }
        */

        auto t_end = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(t_end-t_start).count();
        int sleeptime = 1-elapsed_time;

        if (sleeptime > 0)
            sleeper.sleepFor(chrono::seconds(sleeptime));
    }
}

void MetricsGenerator::memory(){
    //Trend trend_tm = trends.at(TOTAL_MEMORY);
    Trend trend_fm = trends.at(FREE_MEMORY);

    int idx_tm = 0; int idx_fm = 0;

    while(this->running){

        auto t_start = std::chrono::high_resolution_clock::now();
        /*
        if(trend_tm == trCONSTANT){
            currentVal.total_memory = series.at(trCONSTANT).total_memory[idx_tm];

            idx_tm++;

            if(idx_tm == series.at(trCONSTANT).total_memory.size())
                idx_tm = 0;
        }
        */

        if(trend_fm == trSTABLE){
            currentVal.free_memory = series.at(trSTABLE).free_memory[idx_fm];
            
            idx_fm++;

            if(idx_fm == series.at(trSTABLE).free_memory.size()) 
                idx_fm = 0;

        } else if(trend_fm == trUNSTABLE){
            currentVal.free_memory = series.at(trUNSTABLE).free_memory[idx_fm];

            idx_fm++;

            if(idx_fm == series.at(trUNSTABLE).free_memory.size())
                idx_fm = 0;

        }/* else if(trend_fm == trCONSTANT){
            currentVal.free_memory = series.at(trCONSTANT).free_memory[idx_fm];

            idx_fm++;

            if(idx_fm == series.at(trCONSTANT).free_memory.size())
                idx_fm = 0;
        }
        */

        auto t_end = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(t_end-t_start).count();
        int sleeptime = 1-elapsed_time;

        if (sleeptime > 0)
            sleeper.sleepFor(chrono::seconds(sleeptime));
    }
}


void MetricsGenerator::disk(){
    //Trend trend_td = trends.at(TOTAL_DISK);
    Trend trend_fd = trends.at(FREE_DISK);

    int idx_td = 0; int idx_fd = 0;

    while(this->running){

        auto t_start = std::chrono::high_resolution_clock::now();

        /*
        if(trend_td == trCONSTANT){
            currentVal.total_disk = series.at(trCONSTANT).total_disk[idx_td];

            idx_td++;

            if(idx_td == series.at(trCONSTANT).total_disk.size())
                idx_td = 0;
        }
        */

        if(trend_fd == trSTABLE){
            currentVal.free_disk = series.at(trSTABLE).free_disk[idx_fd];
            
            idx_fd++;

            if(idx_fd == series.at(trSTABLE).free_disk.size()) 
                idx_fd = 0;

        } else if(trend_fd == trUNSTABLE){
            currentVal.free_disk = series.at(trUNSTABLE).free_disk[idx_fd];

            idx_fd++;

            if(idx_fd == series.at(trUNSTABLE).free_disk.size())
                idx_fd = 0;

        } /*else if(trend_fd == trCONSTANT){
            currentVal.free_disk = series.at(trCONSTANT).free_disk[idx_fd];

            idx_fd++;

            if(idx_fd == series.at(trCONSTANT).free_disk.size())
                idx_fd = 0;
        }
        */

        auto t_end = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(t_end-t_start).count();
        int sleeptime = 1-elapsed_time;

        if (sleeptime > 0)
            sleeper.sleepFor(chrono::seconds(sleeptime));
    }
}

void MetricsGenerator::battery(){
    Trend trend = trends.at(BATTERY);
    int idx = 0;

    while(this->running){
        auto t_start = std::chrono::high_resolution_clock::now();

        if(trend == trSTABLE){
            currentVal.battery = series.at(trSTABLE).battery[idx];

            idx++;

            if(idx == series.at(trSTABLE).battery.size())
                idx = 0;

        } else if (trend == trUNSTABLE){
            currentVal.battery = series.at(trUNSTABLE).battery[idx];

            idx++;

            if(idx == series.at(trUNSTABLE).battery.size())
                idx = 0;

        } /*else if (trend == trCONSTANT){
            currentVal.battery = series.at(trCONSTANT).battery[idx];

            idx++;

            if(idx == series.at(trCONSTANT).battery.size())
                idx = 0;
        }
        */

        auto t_end = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(t_end-t_start).count();
        int sleeptime = 1-elapsed_time;

        if(sleeptime > 0)
            sleeper.sleepFor(chrono::seconds(sleeptime));
    }
}

