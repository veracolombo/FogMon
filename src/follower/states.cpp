#include "states.hpp"
#include "iagent.hpp"
#include "storage.hpp"
#include <vector>
#include <iostream>

using namespace std;

States::States(){
}

States::States(IAgent* node, int history, float tol){
    this->node = node;
    this->history = history;
    this->tol = tol;
}

States::~States(){
}

void States::setNode(IAgent* node){
    this->node = node;
}

void States::addState(States::Metric metric, States::State state){
    this->nodeState[metric].push_back(state);
}

void States::changeTimeReport(){

    // se c'è almeno una metrica nello stato unstable
    bool unstable = false;
    for(auto el : this->nodeState){
        for (auto state : el.second){
            if(state == States::State::UNSTABLE)
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

void States::setMetrics(vector<States::Metric> metrics){
    if(!metrics.empty()){
        for(int i=0; i<metrics.size(); i++){
            this->metrics.push_back(metrics[i]);
        }
    }
}

void States::addMetric(States::Metric metric){
    for(auto m : this->metrics){
        if(m == metric){
            return;
        }
    }
    this->metrics.push_back(metric);
}

void States::getNodeState(){
    //clear map
    this->nodeState.clear();

    // calculate new states
    for(auto metric : this->metrics){
        
        if(metric == States::Metric::FREE_CPU){
            vector<float> series = this->node->getStorage()->getFreeCpu(this->history);

            this->stable<float>(metric, series, 0.1, this->tol);
            this->increasing<float>(metric, series, this->tol);
            this->decreasing<float>(metric, series, this->tol);
            this->alarms<float>(metric, series, this->tol, 1.0, 0.1, 1.0, 0.9);

        }else if(metric == States::Metric::FREE_MEMORY){
            vector<float> series = this->node->getStorage()->getFreeMemory(this->history);

            /*
            cout << "Free memory series: ";

            for (auto el : series){
                cout << el << " ";
            }

            cout << endl;
            */

            this->stable<float>(metric, series, 0.1, this->tol);
            this->increasing<float>(metric, series, this->tol);
            this->decreasing<float>(metric, series, this->tol);
            this->alarms<float>(metric, series, this->tol, 1.0, 0.1, 1.0, 0.5);

        }else if(metric == States::Metric::FREE_DISK){
            vector<float> series = this->node->getStorage()->getFreeDisk(this->history);

            /*
            cout << "Free disk series: ";

            for (auto el : series){
                cout << el << " ";
            }

            cout << endl;
            */

            this->stable<float>(metric, series, 0.1, this->tol);
            this->increasing<float>(metric, series, this->tol);
            this->decreasing<float>(metric, series, this->tol);
            this->alarms<float>(metric, series, this->tol, 1.0, 0.1, 1.0, 0.5);
        }
    }

    this->changeTimeReport();
}

void States::toString(){
    cout << "********* Actual state *********" << endl;

    for (auto pair : this->nodeState){
        switch(pair.first){
            case States::Metric::FREE_CPU:          cout << "Free CPU:"; break;
            case States::Metric::FREE_MEMORY:       cout << "Free Memory:"; break;
            case States::Metric::FREE_DISK:         cout << "Free Disk:"; break;
        }

        for (int i=0; i<pair.second.size(); i++){
            switch(pair.second[i]){
                case States::State::STABLE:         cout << " Stable"; break;
                case States::State::UNSTABLE:       cout << " Unstable"; break;
                case States::State::INCREASING:     cout << " Increasing"; break;
                case States::State::DECREASING:     cout << " Decreasing"; break;
                case States::State::TOO_HIGH:       cout << " Too_High"; break;
                case States::State::TOO_LOW:        cout << " Too_Low"; break;
                case States::State::ALARMING_HIGH:  cout << " Alarming_High"; break;
                case States::State::ALARMING_LOW:   cout << " Alarming_Low"; break;
                case States::State::OK:             cout << " Ok"; break;
            }
            if(i!=pair.second.size()-1){
                cout << ",";
            }
        }
        cout << endl;
    }
    cout << "********************************" << endl; 
}