#include "condition.hpp"
#include "adaptive_storage.hpp"
#include <iostream>

Condition::Condition() {}

Condition::~Condition() {}

void Condition::MetricInStateFor(Environment *env, UDFContext *udfc, UDFValue *out) {

    // get arguments
    UDFValue m; UDFValue s;
    UDFNthArgument(udfc, 1, STRING_BIT, &m);
    UDFNthArgument(udfc, 2, STRING_BIT, &s);

    const char* metric = m.lexemeValue->contents;
    const char* state = s.lexemeValue->contents;

    // get data
    IAdaptiveStorage* storage = new AdaptiveStorage();
    storage->open("adaptive_storage.db");
    vector<tuple<string, int, int>> data = storage->getStates();

    int count = 0;
    bool found = false;
    string lasttime;

    try{
        lasttime = get<0>(data.at(0));
    }catch (const std::out_of_range& e) { }

    
    for(int i=0; i<data.size(); i++){

        string currtime = get<0>(data[i]);
        
        if(lasttime != currtime){
            lasttime = currtime;
            if(found==false){
                break;
            }else{
                found = false;
            }
        }else{
            if(found == true){
                continue;
            }
        }

        if(get<1>(data[i]) == lMetrics.at(metric) &&
           get<2>(data[i]) == lStates.at(state)){
                count += 1;
                found = true;
        }     
    }

    // cout << "Metric " << metric << " in state " << state << " for " << count << " samples" << endl;
    out->integerValue = CreateInteger(env,count);
 }