#include "metrics_generator.hpp"
#include "adaptive_follower.hpp"
#include <iostream>


map<Metric, MetricsGenerator::Trend> MetricsGenerator::trends = {

    {FREE_CPU, MetricsGenerator::Trend::trSTABLE},

    {TOTAL_MEMORY, MetricsGenerator::Trend::trSTABLE},
    {FREE_MEMORY, MetricsGenerator::Trend::trSTABLE},

    {TOTAL_DISK, MetricsGenerator::trSTABLE},
    {FREE_DISK, MetricsGenerator::Trend::trSTABLE},

    {BATTERY, MetricsGenerator::Trend::trSTABLE}
};

map<Metric, map<MetricsGenerator::Trend, vector<float>>> MetricsGenerator::series = {
    { FREE_CPU, 
        { 
            { MetricsGenerator::Trend::trSTABLE,
                { 0.80, 0.83, 0.81, 0.82, 0.83, 0.81 } 
            },
            { MetricsGenerator::Trend::trUNSTABLE,
                { 0.80, 0.85, 0.60, 0.65, 0.50, 0.70 }
            },
            { MetricsGenerator::Trend::trTOO_LOW,
                { 0.2 }
            },
            { MetricsGenerator::Trend::trOK,
                { 0.8 }
            },
            { MetricsGenerator::Trend::trOK_TOO_LOW_OK,
                { 0.2, 0.8}
            },
            { MetricsGenerator::Trend::trSPIKE,
                { 0.90, 0.901, 0.899, 0.90, 0.901, 0.90, 0.899, 0.90, 0.901, 0.90, 0.911, 0.898, 0.90, 0.911, 0.90, 0.898, 0.60, 0.59, 0.899, 0.90, 0.901, 0.899}
            }
        }
    },

    { TOTAL_MEMORY,
        {
            { MetricsGenerator::Trend::trSTABLE,
                { 16000.0 }
            }
        }
    },

    { FREE_MEMORY,
        {
            { MetricsGenerator::Trend::trSTABLE,
                { 15000.0, 15050.0 }
            },
            { MetricsGenerator::Trend::trUNSTABLE,
                { 15000.0, 10000.0 }
            },
            { MetricsGenerator::Trend::trTOO_LOW,
                { 1.0 }
            },
            { MetricsGenerator::Trend::trOK,
                { 15000.0 }
            },
            { MetricsGenerator::Trend::trOK_TOO_LOW_OK,
                { 15000.0, 1.0 }
            }
        }
    },

    { TOTAL_DISK,
        {
            { MetricsGenerator::Trend::trSTABLE,
                { 500000.0 }
            }
        }
    },

    { FREE_DISK,
        {
            { MetricsGenerator::Trend::trSTABLE,
                { 400000.0, 400050.0 }
            },
            { MetricsGenerator::Trend::trUNSTABLE,
                { 400000.0, 300000.0 }
            },
            { MetricsGenerator::Trend::trTOO_LOW,
                { 100000.0 }
            },
            { MetricsGenerator::Trend::trOK,
                { 400000.0 }
            },
            { MetricsGenerator::Trend::trOK_TOO_LOW_OK,
                { 400000.0, 100000.0 }
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
            },
            { MetricsGenerator::Trend::trOK,
                { 0.8 }
            },
            { MetricsGenerator::Trend::trOK_TOO_LOW_OK,
                { 0.8, 0.2, 0.2, 0.2, 0.8, 0.8, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.2 }
            }
        }
    }
};


map<Metric, float> MetricsGenerator::currentVal = {

    {FREE_CPU, 0.0},
    {TOTAL_MEMORY, 0.0},
    {FREE_MEMORY, 0.0},
    {TOTAL_DISK, 0.0},
    {FREE_DISK, 0.0},
    {BATTERY, 0.0}
    
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
            else if(op == "bok")
                this->trends[BATTERY] = trOK;
            else if(op == "boktl")
                this->trends[BATTERY] = trOK_TOO_LOW_OK;
            else if(op == "cu")
                this->trends[FREE_CPU] = trUNSTABLE;
            else if(op == "cspike")
                this->trends[FREE_CPU] = trSPIKE;
        }
        /*
        for(auto &t : this->trends){
            if(t.first != BATTERY){
                
                if(t.first == TOTAL_MEMORY || t.first == TOTAL_DISK)
                    continue;

                Trend trend = static_cast<Trend>(rand() % last);
                t.second = trend;
            }
        }
        */
    }
    /*
    else{
        for(auto &t : this->trends){
            if(t.first == TOTAL_MEMORY || t.first == TOTAL_DISK)
                    continue;

            Trend trend = static_cast<Trend>(rand() % last);
            t.second = trend;
        }
    }
    */

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
    //cout << "metricsRoutine()" << endl;
    int idx = 0;

    while(this->running){
        //cout << "Metric: " << Metric2String.at(metric) << endl;
        //cout << "Trend: " << trend << endl;

        auto t_start = std::chrono::high_resolution_clock::now();

        //cout << "here" << endl;

        auto now = std::chrono::system_clock::now();
        auto UTC = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
        
        currentVal.at(metric) = series.at(metric).at(trend)[idx];

        if(metric==FREE_CPU){
            this->f.open("monitoring_logs/CPU_real.csv", ios_base::out | ios_base::app);
            
            if(this->f.is_open()){
                this->f << currentVal.at(metric) << " " << std::to_string(UTC) << "\n"; 
                this->f.close();
            } else{
                cout << "Unable to open file." << endl;
            }
        }

        idx++;

        //cout << "here2" << endl;

        if(idx == series.at(metric).at(trend).size())
            idx = 0;

        //cout << "here3" << endl;

        auto t_end = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(t_end-t_start).count();
        int sleeptime = 1-elapsed_time;

        if(sleeptime > 0)
            sleeper.sleepFor(chrono::seconds(sleeptime));
    }
}

