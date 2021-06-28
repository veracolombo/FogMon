#ifndef STATES_HPP_
#define STATES_HPP_

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <math.h>
#include "iagent.hpp"

using namespace std;

class States {

public:
    enum State {STABLE, UNSTABLE, INCREASING, DECREASING, TOO_HIGH, TOO_LOW, ALARMING_HIGH, ALARMING_LOW, OK, NONE};
    enum Metric {FREE_CPU, FREE_MEMORY, FREE_DISK, LATENCY, BANDWIDTH};

    States();
    States(IAgent* node, int history=5, float tol=0.8);

    ~States();

    void setNode(IAgent* node);
    void setHistory(int history);
    void setTol(float tol);

    void setMetrics(vector<Metric> metrics);
    void addMetric(Metric metric);

    void getNodeState();

    void toString();

    template <typename T>
    void stable(Metric metric, vector<T> series, T delta_max, float tol) {

        int k = series.size() - 1;
        int stables = 0;

        for (int i=0; i<=k; i++){
            T current_sample = series[i];
            T prev_sample = series[i+1];

            if(abs(current_sample - prev_sample) < delta_max){
                stables += 1;
            }
        }

        if(stables >= ceil(tol*k)){
            addState(metric, State::STABLE);
        }else{
            addState(metric, State::UNSTABLE);
        }
    }

    
    template <typename T>
    void increasing(Metric metric, vector<T> series, float tol){

        int k = series.size() - 1;

        State first_sample = State::NONE;
        if(series[0] > series[1]){
            first_sample = State::INCREASING;
        }

        int increasing = 0;
        for (int i=1; i<=k; i++){
            T current_sample = series[i];
            T prev_sample = series[i+1];

            if(current_sample > prev_sample){
                increasing += 1;
            }
        }

        if(first_sample == State::INCREASING && increasing >= ceil(tol*k) && series[0]>series[k]){
            addState(metric, State::INCREASING);
        }
    }

    template <typename T>
    void decreasing(Metric metric, vector<T> series, float tol){

        int k = series.size() - 1;

        State first_sample = State::NONE;
        if(series[0] < series[1]){
            first_sample = State::DECREASING;
        }

        int decreasing = 0;
        for (int i=1; i<=k; i++){
            T current_sample = series[i];
            T prev_sample = series[i+1];

            if(current_sample < prev_sample){
                decreasing += 1;
            }
        }

        if(first_sample == State::DECREASING && decreasing >= ceil(tol*k) && series[0]<series[k]){
            addState(metric, State::DECREASING);
        }
    }

    template <typename T>
    void alarms(Metric metric, vector<T> series, float tol, T too_high, T too_low, T alarming_high, T alarming_low){

        int k = series.size() - 1;
        int too_high_c = 0, too_low_c = 0, alarming_high_c = 0, alarming_low_c = 0, ok_c = 0;

        State first_sample;
        if(series[0] > too_high){
            first_sample = State::TOO_HIGH;
        }else if(series[0] < too_low){
            first_sample = State::TOO_LOW;
        }else if(series[0] > alarming_high){
            first_sample = State::ALARMING_HIGH;
        }else if(series[0] < alarming_low){
            first_sample = State::ALARMING_LOW;
        }else{
            first_sample = State::OK;
        }

        for(int i=1; i<=k; i++){
            T sample = series[i];

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
            addState(metric, State::TOO_HIGH);
        }else if(first_sample == State::TOO_LOW && too_low_c >= ceil(tol*k)){
            addState(metric, State::TOO_LOW);
        }else if(first_sample == State::ALARMING_HIGH && alarming_high_c >= ceil(tol*k)){
            addState(metric, State::ALARMING_HIGH);
        }else if(first_sample == State::ALARMING_LOW && alarming_low_c >= ceil(tol*k)){
            addState(metric, State::ALARMING_LOW);
        }else{
            addState(metric, State::OK);
        }
    }

private:
    
    IAgent* node;
    int history;
    float tol;

    map<Metric, vector<State>> nodeState;
    vector<Metric> metrics;

    void addState(Metric metric, State state);
    void changeTimeReport();
};

#endif