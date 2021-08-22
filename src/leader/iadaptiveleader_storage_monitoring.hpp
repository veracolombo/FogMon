#ifndef IADAPTIVELEADER_STORAGE_MONITORING_HPP_
#define IADAPTIVELEADER_STORAGE_MONITORING_HPP_

#include "leader_storage.hpp"
#include "adaptive_storage_monitoring.hpp"

class IAdaptiveLeaderStorageMonitoring : virtual public ILeaderStorage, virtual public IAdaptiveStorageMonitoring  {

};

#endif