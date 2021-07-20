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
};

#endif