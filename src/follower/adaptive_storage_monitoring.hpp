#ifndef ADAPTIVE_STORAGE_MONITORING_HPP_
#define ADAPTIVE_STORAGE_MONITORING_HPP_

#include "storage.hpp"
#include "iadaptive_storage_monitoring.hpp"

class AdaptiveStorageMonitoring : virtual public Storage, virtual public IAdaptiveStorageMonitoring {
protected:
    virtual void createTables() override;
    virtual void call_superclass_create_tables();
public:
    AdaptiveStorageMonitoring();
    ~AdaptiveStorageMonitoring();
};

#endif