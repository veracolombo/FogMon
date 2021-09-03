#include "metrics_generator.hpp"
#include "adaptive_follower.hpp"
#include <iostream>


map<Metric, MetricsGenerator::Trend> MetricsGenerator::trends = {

    {FREE_CPU, MetricsGenerator::Trend::trSTABLE},

    //{TOTAL_MEMORY, MetricsGenerator::Trend::trCONSTANT},
    {FREE_MEMORY, MetricsGenerator::Trend::trSTABLE},

    //{TOTAL_DISK, MetricsGenerator::trCONSTANT},
    {FREE_DISK, MetricsGenerator::Trend::trSTABLE},

    {BATTERY, MetricsGenerator::Trend::trSTABLE},

    {LIGHT_INTENSITY, MetricsGenerator::Trend::trSTABLE}

};

map<Metric, map<MetricsGenerator::Trend, vector<float>>> MetricsGenerator::series = {
    { FREE_CPU, 
        { 
            { MetricsGenerator::Trend::trSTABLE,
                { 0.80, 0.85 } 
            },
            { MetricsGenerator::Trend::trUNSTABLE,
                { 0.60, 0.80, 0.60, 0.90 }
            },
            { MetricsGenerator::Trend::trTOO_LOW,
                { 0.2 }
            }
        }
    },

    { FREE_MEMORY,
        {
            { MetricsGenerator::Trend::trSTABLE,
                { 3500000.0, 3400000.0, 3600000.0, 3550000.0 }
            },
            { MetricsGenerator::Trend::trUNSTABLE,
                { 3500000.0, 2500000.0, 300000.0, 1500000.0 }
            },
            { MetricsGenerator::Trend::trTOO_LOW,
                { 0.2 }
            }
        }
    },

    { FREE_DISK,
        {
            { MetricsGenerator::Trend::trSTABLE,
                { 100000000.0, 150000000.0, 125000000.0 }
            },
            { MetricsGenerator::Trend::trUNSTABLE,
                { 100000000.0, 200000000.0, 90000000.0 }
            },
            { MetricsGenerator::Trend::trTOO_LOW,
                { 0.2 }
            }
        }
    },

    { BATTERY,
        {
            { MetricsGenerator::Trend::trSTABLE,
                { 0.80, 0.85 }
            },
            { MetricsGenerator::Trend::trUNSTABLE,
                { 0.70, 0.55, 0.80, 0.95 }
            },
            { MetricsGenerator::Trend::trTOO_LOW,
                { 0.2 }
            }
        }
    },

    { LIGHT_INTENSITY,
        {
            { MetricsGenerator::Trend::trSTABLE,
                { 0.50, 0.55 }
            },
            { MetricsGenerator::Trend::trUNSTABLE,
                { 0.50, 0.30, 0.70 }
            },
            { MetricsGenerator::Trend::trTOO_LOW,
                { 0.2 }
            }
        }
    }
};


map<Metric, float> MetricsGenerator::currentVal = {

    {FREE_CPU, 0.0},
    {FREE_MEMORY, 0.0},
    {FREE_DISK, 0.0},
    {BATTERY, 0.0},
    {LIGHT_INTENSITY, 0.0}
    
};

MetricsGenerator::MetricsGenerator() {
    this->running = false;
}
MetricsGenerator::~MetricsGenerator() {
    this->stop();
    delete [] this->metricsThreads;
}

void MetricsGenerator::initialize(IAdaptiveFollower* node) {
    this->node = node;

    this->metricsThreads = new thread[this->trends.size()];
}

void MetricsGenerator::start() {

    if(!this->node->node->mg_options.empty()){
        for(auto &op : this->node->node->mg_options){
            if(op == "btl")
                this->trends[BATTERY] = trTOO_LOW;
            else if(op == "bs")
                this->trends[BATTERY] = trSTABLE;
        }

        for(auto &t : this->trends){
            if(t.first != BATTERY){
                Trend trend = static_cast<Trend>(rand() % last);
                t.second = trend;
            }
        }
    }else{
        for(auto &t : this->trends){
            Trend trend = static_cast<Trend>(rand() % last);
            t.second = trend;
        }
    }

    //check if threads are already running
    for(int i=0; i<this->trends.size(); i++) {
        if(this->metricsThreads[i].joinable())
        {
            return;
        }
    }
    //start
    this->running = true;

    int i = 0;
    for(auto &t : this->trends) {
        this->metricsThreads[i] = thread(&MetricsGenerator::metricsRoutine, this, t.first, t.second);
        i++;
    }
}

void MetricsGenerator::stop() {
    this->running = false;

    for(int i=0; i<this->trends.size(); i++) {
        if(this->metricsThreads[i].joinable())
        {
            this->metricsThreads[i].join();
        }
    }
}

void MetricsGenerator::metricsRoutine(Metric metric, Trend trend) {
    int idx = 0;

    while(this->running){
        auto t_start = std::chrono::high_resolution_clock::now();

        currentVal.at(metric) = series.at(metric).at(trend)[idx];
        idx++;

        if(idx == series.at(metric).at(trend).size())
            idx = 0;

        auto t_end = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(t_end-t_start).count();
        int sleeptime = 1-elapsed_time;

        if(sleeptime > 0)
            sleeper.sleepFor(chrono::seconds(sleeptime));
    }
}

