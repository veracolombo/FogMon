#ifndef ADAPTIVE_LEADER_STORAGE_MONITORING_HPP_
#define ADAPTIVE_LEADER_STORAGE_MONITORING_HPP_

#include "iadaptiveleader_storage_monitoring.hpp"
#include "leader_storage.hpp"
#include "adaptive_storage_monitoring.hpp"

class AdaptiveLeaderStorageMonitoring : public LeaderStorage, public AdaptiveStorageMonitoring, virtual public IAdaptiveLeaderStorageMonitoring {
protected:
    void createTables() override;
    void call_superclass_create_tables() override;

public:
    AdaptiveLeaderStorageMonitoring(Message::node node);
    ~AdaptiveLeaderStorageMonitoring();
};

#endif