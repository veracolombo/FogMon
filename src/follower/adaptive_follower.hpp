#ifndef ADAPTIVE_FOLLOWER_HPP_
#define ADAPTIVE_FOLLOWER_HPP_

<<<<<<< HEAD
#include "follower.hpp"                // parent
=======
#include "follower.hpp"        // parent class
#include "iadaptivestorage.hpp"
>>>>>>> parent of 106a5a7 (added adaptive leader)
#include "adaptive_factory.hpp"  
#include "adaptive_controller.hpp"
#include "common.hpp"
#include "adaptive_follower_connections.hpp"
#include "iadaptivefollower.hpp"

<<<<<<< HEAD
class AdaptiveFollower : virtual public IAdaptiveFollower, virtual public Follower {
=======
class AdaptiveFollower : public Follower {
>>>>>>> parent of 106a5a7 (added adaptive leader)

public:
    // metrics
    static map<Metric, bool> metrics;

    AdaptiveFollower(Message::node node, int nThreads);
    ~AdaptiveFollower();

<<<<<<< HEAD
    
    virtual void initialize(AdaptiveFactory* factory = NULL);
=======
    void initialize(Factory* factory = NULL, AdaptiveFactory* adaptiveFactory = NULL);
>>>>>>> parent of 106a5a7 (added adaptive leader)

    void start(vector<Message::node> mNodes);
    void stop();

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

<<<<<<< HEAD
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

=======
>>>>>>> parent of 106a5a7 (added adaptive leader)
    void timer() override;
    void TestTimer() override;
};

#endif