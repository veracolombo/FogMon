#ifndef ADAPTIVE_CONTROLLER_HPP_
#define ADAPTIVE_CONTROLLER_HPP_

#include "sleeper.hpp"
#include "common.hpp"
#include "rule.hpp"

#include <map>
#include <vector>
#include <math.h>
#include <thread>


class AdaptiveFollower;

class AdaptiveController {

public:

    AdaptiveController(AdaptiveFollower* node);
    ~AdaptiveController();

    void initialize();

    void start();
    void stop();

    map<Metric, vector<State>> getStates();

    void toStringSeries();
    void toStringStates();

    Rule* rule;
    

private:
    AdaptiveFollower* node;

    int history = 5;

    map<Metric, vector<float>> series;
    map<Metric, vector<State>> states;

    //threads
    std::thread statesThread;
    
    Sleeper sleeper;

    bool running;

    void statesTimer();

    void addState(Metric metric, State state);
    void saveStates();

    void stable(float delta_max = 0.1, float tol = 0.8);
    void increasing(float tol = 0.8);
    void decreasing(float tol = 0.8);
    void alarms(float tol=0.8, float too_high=1, float too_low=0.4, float alarming_high=1, float alarming_low=0.3);

};
#endif