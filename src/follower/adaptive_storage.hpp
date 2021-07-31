#ifndef ADAPTIVE_STORAGE_HPP_
#define ADAPTIVE_STORAGE_HPP_

#include "iadaptivestorage.hpp"
#include <vector>
#include <string>
#include <tuple>
#include <utility>
#include "common.hpp"

using namespace std;

class AdaptiveStorage : virtual public IAdaptiveStorage {
protected:
    virtual void createTables();
    void isError(int err, char *zErrMsg, std::string mess);

public:
    AdaptiveStorage();
    virtual ~AdaptiveStorage();

    vector<tuple<string, int, int>> getStates();
    void saveStates(vector<State> states, Metric metric);

    vector<float> getFreeCpu(int history);
    vector<float> getFreeMemory(int history);
    vector<float> getFreeDisk(int history);

    void saveCpu(int cores, float free_cpu, int window);
    void saveMemory(int64_t memory, float free_memory, int window);
    void saveDisk(int64_t disk, float free_disk, int window);

    vector<float> getLastValues(Metric metric, int history);
};

#endif