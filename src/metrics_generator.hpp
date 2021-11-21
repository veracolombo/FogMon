#ifndef METRICS_GENERATOR_HPP_
#define METRICS_GENERATOR_HPP_

#include "common.hpp"
#include "sleeper.hpp"

#include <vector>
#include <map>
#include <thread>

#include <fstream>

#include "iadaptivefollower.hpp"

using namespace std;

class MetricsGenerator {

public:
    enum Trend {trSTABLE, trUNSTABLE, trTOO_LOW, last};

    MetricsGenerator();
    ~MetricsGenerator();

    void start();
    void stop();

    void initialize(IAdaptiveFollower *node);

    /*
    Trend desiderato per una determinata metrica
    */
    static map<Metric, Trend> trends;

    /*
    Valore che assume la metrica all'istante corrente
    */
    static map<Metric, float> currentVal;

    /*
    Serie di dati da generare per una coppia (Metrica, Trend)
    */
    static map<Metric, map<Trend, vector<float>>> series;

    ofstream f;

private:
    IAdaptiveFollower* node;

    bool running;

    /*
    Un thread per ciascuna metrica
    */
    thread *metricsThreads;

    void metricsRoutine(Metric metric, Trend trend);

    Sleeper sleeper;
};

#endif