#ifndef ADAPTIVE_FOLLOWER_HPP_
#define ADAPTIVE_FOLLOWER_HPP_

#include "follower.hpp"                // parent
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
    IAdaptiveStorageMonitoring* getStorage();
    AdaptiveController* getAdaptiveController();

    
    vector<Metric> getMetrics();
    void setMetrics(vector<Metric> metrics);
    

    /*
    void addMetric(Metric metric);
    void removeMetric(Metric metric);
    */

<<<<<<< HEAD
    virtual bool changeServer(vector<Message::node> mNodes);

    virtual void disableMetrics(vector<Metric> metrics);
    virtual void enableMetrics(vector<Metric> metrics);
=======
    IAdaptiveStorage* getAdaptiveStorage();
>>>>>>> parent of f5a88dd (added changeServer())

protected:

    AdaptiveController* adaptive_controller;

    AdaptiveFactory tFactory;
    AdaptiveFactory * factory;

    AdaptiveFollowerConnections *connections;
<<<<<<< HEAD
    IAdaptiveStorageMonitoring *storage;
=======

    //void getCpu();
    //void getMemory();
    //void getDisk();
>>>>>>> parent of 371572f (aggiunta estensione storage per altre metriche)

    MetricsGenerator *metricsGenerator;

    void getLightIntensity();
    void getBattery();
    void getHardware() override;

    void timer() override;
    void TestTimer() override;
    
};

#endif