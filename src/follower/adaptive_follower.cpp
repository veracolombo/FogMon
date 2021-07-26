#include "adaptive_follower.hpp"
#include <iostream>

map<Metric, bool> AdaptiveFollower::metrics = {
    {Metric::FREE_CPU, true},
    {Metric::FREE_MEMORY, true},
    {Metric::FREE_DISK, true},
    {Metric::LATENCY, false},
    {Metric::BANDWIDTH, false},
    {Metric::CONNECTED_IOTS, false}
};

AdaptiveFollower::AdaptiveFollower() {}

AdaptiveFollower::AdaptiveFollower(Message::node node, int nThreads) : Follower(node, nThreads) { }

AdaptiveFollower::~AdaptiveFollower() {
    try{
        delete this->adaptiveStorage;
        this->adaptiveStorage = NULL;
    }catch(...) {}
}

void AdaptiveFollower::initialize(Factory* fact, AdaptiveFactory* adFact) {
    Follower::initialize(fact);

    if(adFact == NULL) {
        this->adaptiveFactory = &this->adaptiveTFactory;
    }else {
        this->adaptiveFactory = adFact;
    }
    
    if(this->adaptiveStorage == NULL){
        this->adaptiveStorage = this->adaptiveFactory->newAdaptiveStorage("adaptive_storage.db");
    }

    this->adaptive_controller = new AdaptiveController(this);
    this->adaptive_controller->initialize();
}

void AdaptiveFollower::start(vector<Message::node> mNodes){
    Follower::start(mNodes);
    this->adaptive_controller->start();
}

void AdaptiveFollower::stop(){
    this->adaptive_controller->stop();

    Follower::stop();

    if(this->adaptiveStorage)
        this->adaptiveStorage->close();
}


vector<Metric> AdaptiveFollower::getMetrics(){
    vector<Metric> res;
    
    for(auto const& m : this->metrics){
        res.push_back(m.first);
    }

    return res;
}


void AdaptiveFollower::setMetrics(vector<Metric> metrics){
    if(!this->metrics.empty()){    
        this->metrics.clear();
    }
    
    for(auto const& m : metrics){
        this->metrics[m] = true;
    }
}


/*
void AdaptiveFollower::addMetric(Metric metric){
    for(auto m : this->metrics){
        if(m == metric){
            return;
        }
    }
    this->metrics.push_back(metric);
}

void AdaptiveFollower::removeMetric(Metric metric){
    this->metrics.erase(std::remove(this->metrics.begin(), this->metrics.end(), metric), this->metrics.end());
}
*/


IAdaptiveStorage* AdaptiveFollower::getAdaptiveStorage() {
    return this->adaptiveStorage;
}


bool AdaptiveFollower::sendReport(){
    bool ret = true;

    std::optional<std::pair<int64_t,Message::node>> ris = this->connections->sendUpdate(this->nodeS, this->update);

    if(ris == nullopt) {
        cout << "update retry..." << endl;
        ris = this->connections->sendUpdate(this->nodeS,this->update);
        if(ris == nullopt) {
            //change server
            cout << "Changing server..." << endl;
            if(!selectServer(this->node->getMNodes())) {
                cout << "Failed to find a server!!!!!!!!" << endl;
            }
            ret = false;
        }
    }

    if(ris != nullopt) {
        this->update.first= (*ris).first;
        this->update.second= (*ris).second;
    }

    Follower::nUpdate += 1;

    cout << "Number of updates sent until now: " << Follower::nUpdate << endl;

    return ret;
}

void AdaptiveFollower::timer(){
    int iter=0;
    while(this->running) {

        auto t_start = std::chrono::high_resolution_clock::now();

        if(metrics[FREE_CPU] &&
           metrics[FREE_MEMORY] &&
           metrics[FREE_DISK]) {
            //generate hardware report and send it
            cout << "Measuring Hardware and sending it..." << endl;
            this->getHardware();
            if(!this->sendReport()){
                iter=0;
            }
        }

        //every 10 iterations ask the nodes in case the server cant reach this network
        if(iter%10 == 0) {
            vector<Message::node> ips = this->connections->requestNodes(this->nodeS);   // chiede al leader gli i ip dei follower nel suo gruppo
            vector<Message::node> tmp = this->getStorage()->getNodes();
            vector<Message::node> rem;

            for(auto node : tmp) {
                bool found = false;
                int i=0;
                while(!found && i<ips.size()) {
                    if(node.id == ips[i].id)
                        found = true;
                    i++;
                }
                if(!found) {
                    rem.push_back(node);
                }
            }

            this->getStorage()->updateNodes(ips,rem);               // elimina dallo storage i nodi che non vengono restituiti dal leader
        }

        //every leaderCheck iterations update the MNodes
        if(iter% this->node->leaderCheck == this->node->leaderCheck-1) {
            vector<Message::node> res = this->connections->requestMNodes(this->nodeS);  // chiede al leader gli i ip dei follower nel suo gruppo
            if(!res.empty()) {
                for(int j=0; j<res.size(); j++)
                {
                    if(res[j].ip==std::string("::1")||res[j].ip==std::string("127.0.0.1"))
                        res[j].ip = this->nodeS.ip;         // sostituzione ip locale del leader con ip esterno
                }
                this->node->setMNodes(res);                 // aggiornamento dei nodi conosciuti
                cout << "Check server" << endl;
                bool change = this->checkServer(res);       // controlla se c'è un nodo con latenza minore
                if(change) {
                    cout << "Changing server" << endl;
                    if(!selectServer(res)) {
                        cout << "Failed to find a server!!!!!!!!" << endl;
                    }
                }
                cout << "no change" << endl;
            }
        }

        // check iperf still running
        // int res = this->pIperf->nowaitproc();
        // if(res!=-1) {
        //     string out = this->pIperf->readoutput();
        //     cout << "Iperf crashed!!!!!!!! "<< res << " " << out << endl;
        //     delete this->pIperf;
        //     this->pIperf = NULL;
        //     this->startIperf();
        // }

        if(iter % 3 == 0) {
            string out = this->pIperf->readoutput();
            cout << "Iperf restart" << endl;
            delete this->pIperf;
            this->pIperf = NULL;
            this->startIperf();
        }

        auto t_end = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::duration<float>>(t_end-t_start).count();
        int sleeptime = Node::timeReport-elapsed_time;
        
        
        cout << "Time report: " << Node::timeReport << endl;
        

        if (sleeptime > 0)
            sleeper.sleepFor(chrono::seconds(sleeptime));
        iter++;
    }
}


void AdaptiveFollower::TestTimer(){
    int iter=0;
    while(this->running) {
        //monitor IoT
        if(metrics[CONNECTED_IOTS] && iter%4 == 0){
            cout << "Measuring IoTs and sending it..." << endl;
            this->testIoT();
            if(!this->sendReport()){
                iter=0;
            }
        }


        vector<thread> LatencyThreads;
        if(metrics[LATENCY]){
            //get list ordered by time for the latency tests
            //test the least recent
            cout << "Measuring Latency..." << endl;
            vector<Message::node> ips = this->storage->getLRLatency(this->node->maxPerLatency, this->node->timeLatency);

            for(auto node : ips) {
                if(this->myNode.id == node.id)
                    continue;
                //start thread for latency tests
                thread LatencyThread = thread([this,node]{
                    int val = this->testPing(node.ip);
                    if(val >= 0) {
                        this->storage->saveLatencyTest(node, val, this->node->latencyWindow);
                    }
                });

                LatencyThreads.push_back(move(LatencyThread));
            }
        }

        thread BandwidthThread;
        
        if(metrics[BANDWIDTH]){
        //start thread for bandwidth tests
        BandwidthThread = thread([this]{
            //test bandwidth
            //get 10 nodes tested more than 300 seconds in the past
                cout << "Measuring Bandwidth..." << endl;
                vector<Message::node> ips = this->storage->getLRBandwidth(this->node->maxPerBandwidth + 5, this->node->timeBandwidth);
                cout << "List B: ";
                for(auto node : ips) {
                    //cout << node.ip << " ";
                }
                cout << endl;
                int i=0;
                int tested=0;
                while(i < ips.size() && tested < this->node->maxPerBandwidth) {
                    if(this->myNode.id == ips[i].id) {
                        i++;
                        continue;
                    }
                    Report::test_result last;
                    int state = this->storage->getTestBandwidthState(ips[i], last);
                    float val = this->testBandwidth(ips[i], last.mean, state);
                    if (state==3) {
                        if(val < 0)
                            val = this->testBandwidth(ips[i], last.mean, state); 
                    }
                    this->storage->saveBandwidthTest(ips[i], val, state, this->node->latencyWindow);
                    if(val >= 0) {
                        tested++;
                    }
                    i++;
                }
        });
        }
    
        if(metrics[LATENCY]){
            cout << "Sending Latency..." << endl;
            for(auto &LatencyThread : LatencyThreads) {
                LatencyThread.join();
            }
            if(!this->sendReport()){
                iter=0;
            }
        }

        if(metrics[BANDWIDTH]){       
            cout << "Sending Bandwidth..." << endl; 
            BandwidthThread.join();
            if(!this->sendReport()){
                iter=0;
            }
        }

        sleeper.sleepFor(chrono::seconds(this->node->timeTests));
        iter++;
    }
}

