#ifndef IADAPTIVELEADER_CONNECTIONS_HPP_
#define IADAPTIVELEADER_CONNECTIONS_HPP_

#include "leader_connections.hpp"
#include "adaptive_follower_connections.hpp"

class IAdaptiveLeader;

class IAdaptiveLeaderConnections : virtual public ILeaderConnections, virtual public IAdaptiveFollowerConnections  {
public:
    virtual void initialize(IAdaptiveLeader* parent) = 0;

    //virtual bool sendMReport(Message::node ip, vector<AdaptiveReport::adaptive_report_result> report) = 0;
    virtual bool sendChangeServer() = 0;
};

#endif