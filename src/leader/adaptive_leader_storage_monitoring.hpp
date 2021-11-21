#ifndef ADAPTIVE_LEADER_STORAGE_MONITORING_HPP_
#define ADAPTIVE_LEADER_STORAGE_MONITORING_HPP_

#include "iadaptiveleader_storage_monitoring.hpp"
#include "leader_storage.hpp"
#include "adaptive_storage_monitoring.hpp"

class AdaptiveLeaderStorageMonitoring : public LeaderStorage, public AdaptiveStorageMonitoring, virtual public IAdaptiveLeaderStorageMonitoring {
protected:
    void createTables() override;

public:
    AdaptiveLeaderStorageMonitoring(Message::node node);
    ~AdaptiveLeaderStorageMonitoring();

    std::vector<AdaptiveReport::adaptive_report_result> getAdaptiveReport(bool complete);
    AdaptiveReport::adaptive_report_result getAdaptiveReport(Message::node node, bool complete);

    //std::string addNode(Message::node node, AdaptiveReport::hardware_result hardware, AdaptiveReport::battery_result battery, Message::node *monitored = NULL);

    void addReport(AdaptiveReport::adaptive_report_result result, Message::node *monitored = NULL);
    void addReport(std::vector<AdaptiveReport::adaptive_report_result> results, Message::node ip);

    void addReportStates(Message::node node, std::map<Metric, std::vector<State>> states);

    virtual AdaptiveReport::battery_result getBattery(Message::node node);

    virtual std::vector<std::tuple<std::string, Metric, State>> getFollowerStates();
    virtual Message::node getMNode(std::string id);
};

#endif