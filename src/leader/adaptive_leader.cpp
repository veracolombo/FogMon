#include "adaptive_leader.hpp"
#include <iostream>
 
AdaptiveLeader::AdaptiveLeader() {}

AdaptiveLeader::AdaptiveLeader(Message::node node, int nThreads) : Leader(node, nThreads), AdaptiveFollower(node, nThreads), Follower(node, nThreads) {}

AdaptiveLeader::~AdaptiveLeader() {}

void AdaptiveLeader::start(std::vector<Message::node> mNodes){
    Leader::start(mNodes);
    this->adaptive_controller->start();
}

void AdaptiveLeader::stop(){
    AdaptiveFollower::stop();
    if(this->timerFunThread.joinable())
        this->timerFunThread.join();
}

void AdaptiveLeader::initialize(LeaderFactory* factory, AdaptiveFactory* adFact){
    Leader::initialize(factory);

    if(adFact == NULL) {
        this->adaptiveFactory = &this->adaptiveTFactory;
    }else {
        this->adaptiveFactory = adFact;
    }
    
    this->adaptiveStorage = this->adaptiveFactory->newAdaptiveStorage("adaptive_storage.db");

    this->adaptive_controller = new AdaptiveController(this);
    this->adaptive_controller->initialize();
}