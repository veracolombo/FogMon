/*
#include "StatesOld.hpp"
#include "iagent.hpp"
#include "storage.hpp"
#include <vector>
#include <iostream>

using namespace std;

StatesOld::StatesOld(){
}

StatesOld::StatesOld(IAgent* node, int history, float tol){
    this->node = node;
    this->history = history;
    this->tol = tol;
}

StatesOldOld::~StatesOld(){
}

void StatesOld::setNode(IAgent* node){
    this->node = node;
}

void StatesOld::addState(StatesOld::Metric metric, StatesOld::State state){
    this->nodeState[metric].push_back(state);
}

void StatesOld::changeTimeReport(){

    // se c'è almeno una metrica nello stato unstable
    bool unstable = false;
    for(auto el : this->nodeState){
        for (auto state : el.second){
            if(state == StatesOld::State::UNSTABLE)
                unstable = true;
                break;
        }
        if(unstable){
            break;
        }
    }

    if(unstable){
        this->node->node->timeReport = 10;
    }else{
        this->node->node->timeReport = 30;
    }
    
    std::cout << "change ReportTime : " << this->node->node->timeReport << " s" << std::endl;
}

void StatesOld::setMetrics(vector<StatesOld::Metric> metrics){
    if(!metrics.empty()){
        for(int i=0; i<metrics.size(); i++){
            this->metrics.push_back(metrics[i]);
        }
    }
}

void StatesOld::addMetric(StatesOld::Metric metric){
    for(auto m : this->metrics){
        if(m == metric){
            return;
        }
    }
    this->metrics.push_back(metric);
}

void StatesOld::getNodeState(){
    //clear map
    this->nodeState.clear();

    // calculate new StatesOld
    for(auto metric : this->metrics){
        
        if(metric == StatesOld::Metric::FREE_CPU){
            vector<float> series = this->node->getStorage()->getFreeCpu(this->history);

            this->stable<float>(metric, series, 0.1, this->tol);
            this->increasing<float>(metric, series, this->tol);
            this->decreasing<float>(metric, series, this->tol);
            this->alarms<float>(metric, series, this->tol, 1.0, 0.1, 1.0, 0.9);

        }else if(metric == StatesOld::Metric::FREE_MEMORY){
            vector<float> series = this->node->getStorage()->getFreeMemory(this->history);

            
            cout << "Free memory series: ";

            for (auto el : series){
                cout << el << " ";
            }

            cout << endl;
            

            this->stable<float>(metric, series, 0.1, this->tol);
            this->increasing<float>(metric, series, this->tol);
            this->decreasing<float>(metric, series, this->tol);
            this->alarms<float>(metric, series, this->tol, 1.0, 0.1, 1.0, 0.5);

        }else if(metric == StatesOld::Metric::FREE_DISK){
            vector<float> series = this->node->getStorage()->getFreeDisk(this->history);

            
            cout << "Free disk series: ";

            for (auto el : series){
                cout << el << " ";
            }

            cout << endl;
            

            this->stable<float>(metric, series, 0.1, this->tol);
            this->increasing<float>(metric, series, this->tol);
            this->decreasing<float>(metric, series, this->tol);
            this->alarms<float>(metric, series, this->tol, 1.0, 0.1, 1.0, 0.5);
        }
    }

    this->changeTimeReport();
}

void StatesOld::toString(){
    cout << "********* Actual state *********" << endl;

    for (auto pair : this->nodeState){
        switch(pair.first){
            case StatesOld::Metric::FREE_CPU:          cout << "Free CPU:"; break;
            case StatesOld::Metric::FREE_MEMORY:       cout << "Free Memory:"; break;
            case StatesOld::Metric::FREE_DISK:         cout << "Free Disk:"; break;
        }

        for (int i=0; i<pair.second.size(); i++){
            switch(pair.second[i]){
                case StatesOld::State::STABLE:         cout << " Stable"; break;
                case StatesOld::State::UNSTABLE:       cout << " Unstable"; break;
                case StatesOld::State::INCREASING:     cout << " Increasing"; break;
                case StatesOld::State::DECREASING:     cout << " Decreasing"; break;
                case StatesOld::State::TOO_HIGH:       cout << " Too_High"; break;
                case StatesOld::State::TOO_LOW:        cout << " Too_Low"; break;
                case StatesOld::State::ALARMING_HIGH:  cout << " Alarming_High"; break;
                case StatesOld::State::ALARMING_LOW:   cout << " Alarming_Low"; break;
                case StatesOld::State::OK:             cout << " Ok"; break;
            }
            if(i!=pair.second.size()-1){
                cout << ",";
            }
        }
        cout << endl;
    }
    cout << "********************************" << endl; 
}
*/