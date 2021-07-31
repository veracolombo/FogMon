#ifndef ADAPTIVE_FOLLOWER_HPP_
#define ADAPTIVE_FOLLOWER_HPP_

#include "follower.hpp"                // parent
#include "iadaptivestorage.hpp"
#include "adaptive_factory.hpp"  
#include "adaptive_controller.hpp"
#include "common.hpp"
#include "adaptive_follower_connections.hpp"
#include "iadaptivefollower.hpp"

class AdaptiveFollower : virtual public IAdaptiveFollower, virtual public Follower {

public:
    // metrics
    static map<Metric, bool> metrics;

    AdaptiveFollower();
    AdaptiveFollower(Message::node node, int nThreads);
    ~AdaptiveFollower();

    
    virtual void initialize(AdaptiveFactory* factory = NULL);

    virtual void start(vector<Message::node> mNodes);
    virtual void stop();

    IAdaptiveFollowerConnections* getConnections();

    
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

    AdaptiveFactory tFactory;
    AdaptiveFactory * factory;

    AdaptiveFollowerConnections *connections;

    //void getCpu();
    //void getMemory();
    //void getDisk();

    // void changeServer();
    
    bool sendReport();

    void getHardware() override;
    void timer() override;
    void TestTimer() override;
    
};

#endif