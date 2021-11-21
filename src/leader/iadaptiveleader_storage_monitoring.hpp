#ifndef IADAPTIVELEADER_STORAGE_MONITORING_HPP_
#define IADAPTIVELEADER_STORAGE_MONITORING_HPP_

#include "leader_storage.hpp"
#include "adaptive_storage_monitoring.hpp"

class IAdaptiveLeaderStorageMonitoring : virtual public ILeaderStorage, virtual public IAdaptiveStorageMonitoring  {

public:
    
    virtual std::vector<AdaptiveReport::adaptive_report_result> getAdaptiveReport(bool complete = false) = 0;
    virtual AdaptiveReport::adaptive_report_result getAdaptiveReport(Message::node node, bool complete) = 0;

    //virtual std::string addNode(Message::node node, AdaptiveReport::hardware_result hardware, AdaptiveReport::battery_result battery, Message::node *monitored = NULL) = 0;

    virtual void addReport(AdaptiveReport::adaptive_report_result result, Message::node *monitored = NULL) = 0;
    virtual void addReport(std::vector<AdaptiveReport::adaptive_report_result> results, Message::node ip) = 0;

    virtual void addReportStates(Message::node node, std::map<Metric, std::vector<State>> states) = 0;

    virtual AdaptiveReport::battery_result getBattery(Message::node ip) = 0;

    // follower states
    virtual std::vector<std::tuple<std::string, Metric, State>> getFollowerStates() = 0;
    virtual Message::node getMNode(std::string id) = 0;

protected:
    static int getFollowerStatesCallback(void *vec, int argc, char **argv, char **azColName);
    static int getNodeCallback(void *R, int argc, char **argv, char **azColName);
};

#endif