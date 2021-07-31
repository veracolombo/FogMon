#include "adaptive_leader.hpp"

#include "uiconnection.hpp"

#include <iostream>

bool AdaptiveLeader::leaderAdequacy = false;
 
AdaptiveLeader::AdaptiveLeader() {}

AdaptiveLeader::AdaptiveLeader(Message::node node, int nThreads) : Leader(node, nThreads), AdaptiveFollower(node, nThreads), Follower(node, nThreads) {}

AdaptiveLeader::~AdaptiveLeader() {}


void AdaptiveLeader::start(std::vector<Message::node> mNodes){
    Leader::start(mNodes);
    this->adaptive_controller->start();
}


void AdaptiveLeader::stop(){
    Leader::stop();
    /*
    AdaptiveFollower::stop();
    if(this->timerFunThread.joinable())
        this->timerFunThread.join();
    */
}


void AdaptiveLeader::initialize(AdaptiveLeaderFactory* fact){
    if(fact == NULL) {
        this->factory = &this->tFactory;
    }else {
        this->factory = fact;
    }

    this->connections = this->factory->newConnections(this->nThreads);
    this->connections->initialize(this);
    Leader::connections = this->connections;
    AdaptiveFollower::connections = this->connections;
    Follower::connections = this->connections;

    Leader::initialize(this->factory);

    this->adaptiveStorage = this->factory->newAdaptiveStorage("adaptive_storage.db");
    AdaptiveFollower::adaptiveStorage = this->adaptiveStorage;

    this->adaptive_controller = new AdaptiveController(this);
    this->adaptive_controller->initialize();
    AdaptiveFollower::adaptive_controller = this->adaptive_controller;

    AdaptiveFollower::initialize(this->factory);
}

IAdaptiveLeaderConnections* AdaptiveLeader::getConnections() {
    return this->connections;
}

/*
void AdaptiveLeader::timerFun(){
    this->iter = 1;
    this->lastQuality = -random()%10-20;
    while(this->running) {
        //routine for Nodes a
        auto t_start = std::chrono::high_resolution_clock::now();
        
        //check database for reports
        vector<Message::node> ips = this->getStorage()->getMLRHardware(100, this->node->timesilent);
        vector<Message::node> rem;
        for(auto&& node : ips) {
            bool res = this->connections->sendRequestReport(node);
            if(!res) {
                printf("Removing node from this group: %s\n",node.ip.c_str());
                rem.push_back(node);
            }
        }
        {
            auto t_end2 = std::chrono::high_resolution_clock::now();
            auto elapsed_time2 = std::chrono::duration_cast<std::chrono::duration<float>>(t_end2-t_start).count();
            cout << "timerFun1 " << elapsed_time2 << endl;
        }
        //remove the nodes that failed to respond
        cout << "sending RemoveNodes..." << endl;
        this->connections->sendRemoveNodes(rem);
        cout << "removeNodes sent." << endl;
        vector<Message::node> tmp;
        this->getStorage()->updateNodes(tmp,rem);   
        {
            auto t_end2 = std::chrono::high_resolution_clock::now();
            auto elapsed_time2 = std::chrono::duration_cast<std::chrono::duration<float>>(t_end2-t_start).count();
            cout << "timerFun2 " << elapsed_time2 << endl;
        }
        //routine for LeaderNodes
        ips = this->getStorage()->getMNodes();

        int num = ips.size();
        bool force = true;
        if (num<1)
            num = 1;
        int time = (int)(this->node->timePropagation*( log2(num)*5+3 ));
        if (iter < 600/this->node->timePropagation) {
            force = false;
            time += this->node->timePropagation*10;
        }
        printf("Check old nodes %d\n",time);
        
        int num_leaders = 0;
        rem = this->getStorage()->removeOldLNodes(time, num_leaders, force); // remove old leaders that do not update in a logarithmic time
        tmp = this->getStorage()->removeOldNodes(this->node->timesilent); // remove followers that do not update in heartbeat time
        //inform other nodes of the removals
        rem.insert(rem.end(),tmp.begin(),tmp.end());
        if (rem.size() > 0) {
            this->connections->sendRemoveNodes(rem);
            tmp.clear();
            this->getStorage()->updateNodes(tmp,rem);
        }
        {
            auto t_end2 = std::chrono::high_resolution_clock::now();
            auto elapsed_time2 = std::chrono::duration_cast<std::chrono::duration<float>>(t_end2-t_start).count();
            cout << "timerFun3 " << elapsed_time2 << endl;
        }
        int i=0;
        int sent=0;
        while(i < ips.size() && sent < 2) {
            if(ips[i].id == this->nodeS.id) {
                i++;
                continue;
            }
            
            vector<Report::report_result> report = this->getStorage()->getReport();

            if(this->connections->sendMReport(ips[i], report)) {
                sent++;
                printf("Sent to Leader: %s\n",ips[i].ip.c_str());
            }
            i++;
        }
        {
            auto t_end2 = std::chrono::high_resolution_clock::now();
            auto elapsed_time2 = std::chrono::duration_cast<std::chrono::duration<float>>(t_end2-t_start).count();
            cout << "timerFun4 " << elapsed_time2 << endl;
        }
        if(iter % 4 == 0) {
            this->getStorage()->complete();
            {
                vector<Report::report_result> report = this->getStorage()->getReport(true);
                UIConnection conn(this->getMyNode(),this->node->interfaceIp, this->node->session);
                conn.sendTopology(report);
            }
            if((iter % (4*2)) == 0) {
                this->lastQuality +=1;
                bool param = this->lastQuality >= 2;
                if (param)
                    this->lastQuality = 0;
                this->selector.checkSelection(param);
            }
        }
        {
            auto t_end2 = std::chrono::high_resolution_clock::now();
            auto elapsed_time2 = std::chrono::duration_cast<std::chrono::duration<float>>(t_end2-t_start).count();
            cout << "timerFun5 " << elapsed_time2 << endl;
        }
        */

        /// *** LEADER ADEQUACY CHECK *** ///
        /*
        if (!leaderAdequacy){
            // manda messaggio di notifica ai follower
            this->connections->sendChangeServer();
        }
        */

       /*
        auto t_end = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(t_end-t_start).count();
        //std::cout << "timerFun1: "<< elapsed_time << " s"<< endl;
        int sleeptime = this->node->timePropagation-elapsed_time;
        if (sleeptime > 0)
            sleeper.sleepFor(chrono::seconds(sleeptime));
        iter++;
        
        //t_end = std::chrono::high_resolution_clock::now();
        //elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(t_end-t_start).count();
        //std::cout << "timerFun2: "<< elapsed_time << " s"<< endl;
    }
}
*/