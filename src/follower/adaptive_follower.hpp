#ifndef ADAPTIVE_FOLLOWER_HPP_
#define ADAPTIVE_FOLLOWER_HPP_

#include "follower.hpp"        // parent class
#include "iadaptivestorage.hpp"
#include "adaptive_factory.hpp"  
#include "adaptive_controller.hpp"
#include "common.hpp"

class AdaptiveFollower : public Follower {

public:
    // metrics
    static map<Metric, bool> metrics;

    AdaptiveFollower(Message::node node, int nThreads);
    ~AdaptiveFollower();

    void initialize(Factory* factory = NULL, AdaptiveFactory* adaptiveFactory = NULL);

    void start(vector<Message::node> mNodes);
    void stop();

    
    vector<Metric> getMetrics();
    void setMetrics(vector<Metric> metrics);

    /*
    void addMetric(Metric metric);
    void removeMetric(Metric metric);
    */

    IAdaptiveStorage* getAdaptiveStorage();

protected:
    AdaptiveController* adaptive_controller;
    IAdaptiveStorage* adaptiveStorage;

    AdaptiveFactory adaptiveTFactory;
    AdaptiveFactory * adaptiveFactory;

    void timer() override;
    void TestTimer() override;
};

#endif