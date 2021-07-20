#include "adaptive_controller.hpp"
#include "adaptive_follower.hpp"
#include <iostream>

AdaptiveController::AdaptiveController(AdaptiveFollower* node){
    this->node = node;
    this->rule = new Rule();
}

AdaptiveController::~AdaptiveController() {}

void AdaptiveController::initialize() {
    this->rule->initialize("clips/facts.clp", "clips/rules.clp");
}

void AdaptiveController::start() {
    this->statesThread = thread(&AdaptiveController::statesTimer, this);
}

void AdaptiveController::stop() {
    if(this->statesThread.joinable()){
        this->statesThread.join();
    }
}

map<Metric, vector<State>> AdaptiveController::getStates(){
    return this->states;
}

void AdaptiveController::addState(Metric metric, State state){
    this->states[metric].push_back(state);
}

void AdaptiveController::saveStates(){
    
    for(auto &s : this->states){
        this->node->getAdaptiveStorage()->saveStates(s.second, s.first);
    }
}


void AdaptiveController::statesTimer(){

    while(true){
    
        auto t_start = std::chrono::high_resolution_clock::now();

        this->series.clear();
        this->states.clear();

        for(auto const &m : this->node->getMetrics()){
        
            vector<float> res = this->node->getStorage()->getLastValues(m, this->history);

            if(!res.empty()){
                this->series[m] = res;
            }
        }

        this->stable();
        this->increasing();
        this->decreasing();
        this->alarms();

        this->saveStates();
        this->rule->run();      // trigger

        this->toStringSeries();
        this->toStringStates();

        auto t_end = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(t_end-t_start).count();
        int sleeptime = this->node->node->timeReport - elapsed_time + 1.5;
        
        if (sleeptime > 0)
            sleeper.sleepFor(chrono::seconds(sleeptime));
    }
}



void AdaptiveController::stable(float delta_max, float tol){
    
    for(auto s : this->series){
        
        int k = s.second.size()-1;

        if(k == 0)     // only one sample
            continue;

        State first_sample = State::NONE;
        if(s.second[0] - s.second[1] < delta_max){
            first_sample = State::STABLE;
        }

        int stables = 0;

        for(int i=0; i<k; i++){
            float current_sample = s.second[i];
            float prev_sample = s.second[i+1];

            if(abs(current_sample - prev_sample) < delta_max){
                stables += 1;
            }
        }

        if(first_sample == State::STABLE && stables >= ceil(tol*k)){
            this->states[s.first].push_back(State::STABLE);
        }else{
            this->states[s.first].push_back(State::UNSTABLE);
        }
    }
}

void AdaptiveController::increasing(float tol) {
    for(auto s : this->series){

        int k = s.second.size() - 1;

        if(k == 0)     // only one sample
            continue;

        //State first_sample = State::NONE;
        //if(s.second[0] > s.second[1]){
        //    first_sample = State::INCREASING;
        //}
        

        int increasing = 0;
        for (int i=0; i<k; i++){
            float current_sample = s.second[i];
            float prev_sample = s.second[i+1];

            if(current_sample > prev_sample){
                increasing += 1;
            }
        }

        if(/*first_sample == State::INCREASING &&*/ increasing >= ceil(tol*k) && s.second[0]>s.second[k]){
            this->states[s.first].push_back(State::INCREASING);
        }
    }
}

void AdaptiveController::decreasing(float tol){
    for(auto s : this->series){
        
        int k = s.second.size() - 1;

        if(k == 0)     // only one sample
            continue;

        
        //State first_sample = State::NONE;
        //if(s.second[k] < s.second[k-1]){
        //    first_sample = State::DECREASING;
        //}
        

        int decreasing = 0;
        for (int i=0; i<k; i++){
            float current_sample = s.second[i];
            float prev_sample = s.second[i+1];

            if(current_sample < prev_sample){
                decreasing += 1;
            }
        }

        if(/*first_sample == State::DECREASING && */decreasing >= ceil(tol*k) && s.second[0]<s.second[k]){
            this->states[s.first].push_back(State::DECREASING);
        }
    }
}




void AdaptiveController::alarms(float tol, float too_high, float too_low, float alarming_high, float alarming_low){
    for(auto s : this->series){

        int k = s.second.size() - 1;

        if(k == 0)     // only one sample
            continue;

        State first_sample;
        if(s.second[0] > too_high){
            first_sample = State::TOO_HIGH;
        }else if(s.second[0] < too_low){
            first_sample = State::TOO_LOW;
        }else if(s.second[0] > alarming_high){
            first_sample = State::ALARMING_HIGH;
        }else if(s.second[0] < alarming_low){
            first_sample = State::ALARMING_LOW;
        }else{
            first_sample = State::OK;
        }

        int too_high_c = 0, too_low_c = 0, alarming_high_c = 0, alarming_low_c = 0, ok_c = 0;

        for(int i=0; i<k; i++){
            float sample = s.second[i];

            if(sample > too_high){
                too_high_c += 1;
            }else if(sample < too_low){
                too_low_c +=1;
            }else if(sample > alarming_high){
                alarming_high_c += 1;
            }else if(sample < alarming_low){
                alarming_low_c += 1;
            }else{
                ok_c += 1;
            }
        }

        if(first_sample == State::TOO_HIGH && too_high_c >= ceil(tol*k)){
            this->states[s.first].push_back(State::TOO_HIGH);
        }else if(first_sample == State::TOO_LOW && too_low_c >= ceil(tol*k)){
            this->states[s.first].push_back(State::TOO_LOW);
        }else if(first_sample == State::ALARMING_HIGH && alarming_high_c >= ceil(tol*k)){
            this->states[s.first].push_back(State::ALARMING_HIGH);
        }else if(first_sample == State::ALARMING_LOW && alarming_low_c >= ceil(tol*k)){
            this->states[s.first].push_back(State::ALARMING_LOW);
        }else{
            this->states[s.first].push_back(State::OK);
        }
    }
}




void AdaptiveController::toStringSeries(){
    cout << "********* Series *********" << endl;

    for(auto s : this->series){
        switch(s.first){
            case Metric::FREE_CPU:          cout << "Free CPU:"; break;
            case Metric::FREE_MEMORY:       cout << "Free Memory:"; break;
            case Metric::FREE_DISK:         cout << "Free Disk:"; break;
        }

        for(int i=0; i<s.second.size(); i++){
            cout << s.second[i];
            if(i!=s.second.size()-1){
                cout << ",";
            }
        }
        cout << endl;
    }
    cout << "********************************" << endl; 
}

void AdaptiveController::toStringStates(){
    cout << "********* Actual state *********" << endl;

    for(auto s : this->states){
        switch(s.first){
            case Metric::FREE_CPU:          cout << "Free CPU:"; break;
            case Metric::FREE_MEMORY:       cout << "Free Memory:"; break;
            case Metric::FREE_DISK:         cout << "Free Disk:"; break;
        }

        for(int i=0; i<s.second.size(); i++){
            switch(s.second[i]){
                case State::STABLE:         cout << " Stable"; break;
                case State::UNSTABLE:       cout << " Unstable"; break;
                case State::INCREASING:     cout << " Increasing"; break;
                case State::DECREASING:     cout << " Decreasing"; break;
                case State::TOO_HIGH:       cout << " Too_High"; break;
                case State::TOO_LOW:        cout << " Too_Low"; break;
                case State::ALARMING_HIGH:  cout << " Alarming_High"; break;
                case State::ALARMING_LOW:   cout << " Alarming_Low"; break;
                case State::OK:             cout << " Ok"; break;
            }
            if(i!=s.second.size()-1){
                cout << ",";
            }
        }
        cout << endl;
    }
    cout << "********************************" << endl; 
}
