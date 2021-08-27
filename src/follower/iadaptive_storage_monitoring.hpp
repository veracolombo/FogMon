#ifndef IADAPTIVE_STORAGE_MONITORING_HPP_
#define IADAPTIVE_STORAGE_MONITORING_HPP_

#include "storage.hpp"
#include "adaptive_report.hpp"

class IAdaptiveStorageMonitoring : virtual public IStorage {
public:
    /**
     * get the state of the hardware saved
     * @return 
    */
    virtual AdaptiveReport::battery_result getBattery() = 0;

    /**
     * get recent values of a metric
     * @param meric kind of metric
     * @param limit history
    */
    virtual std::vector<float> getLastValues(Metric metric, int limit) = 0;

    /**
     * save a battery test
     * @param battery the result of the test
    */
    virtual void saveBattery(AdaptiveReport::battery_result battery, int window) = 0;

protected:
    static int getBatteryCallback(void *R, int argc, char **argv, char **azColName);
};

#endif