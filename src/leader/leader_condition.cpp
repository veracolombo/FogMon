#include "leader_condition.hpp"

void LeaderCondition::IsBatteryTooLow(Environment *env, UDFContext *udfc, UDFValue *out) {
    UDFValue _ip;
    UDFFirstArgument(udfc, STRING_BIT, &_ip);

    const char* ip = _ip.lexemeValue->contents;

    bool ret = true; // !! ritorna sempre true !! //

    out->lexemeValue = CreateBoolean(env, ret);
}

/*
vector<tuple<string, float>> getBattery() {

}
*/