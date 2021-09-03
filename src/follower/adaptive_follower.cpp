#include "adaptive_follower.hpp"
#include "adaptive_follower_connections.hpp"
#include <iostream>
#include <sigar.h>

map<Metric, bool> AdaptiveFollower::metrics = {
    {Metric::FREE_CPU, true},
    {Metric::FREE_MEMORY, false},
    {Metric::FREE_DISK, false},
    {Metric::LATENCY, true},
    {Metric::BANDWIDTH, true},
    {Metric::CONNECTED_IOTS, true},
    {Metric::BATTERY, true}
};

bool AdaptiveFollower::leaderAdequacy = true;

AdaptiveFollower::AdaptiveFollower() { }

AdaptiveFollower::AdaptiveFollower(Message::node node, int nThreads) : Follower(node, nThreads) {
    this->connections = NULL;
    this->storage = NULL;
    this->metricsGenerator = NULL;
    this->adaptive_controller = NULL;
}

AdaptiveFollower::~AdaptiveFollower() {
    this->stop();

    try{
        delete this->adaptive_controller;
        this->adaptive_controller = NULL;
    }catch(...) {}

    try{
        delete this->metricsGenerator;
        this->metricsGenerator = NULL;
    }catch(...) {}
}

void AdaptiveFollower::initialize(AdaptiveFactory* fact) {

    if(fact == NULL) {
        this->factory = &this->tFactory;
    }else {
        this->factory = fact;
    }

    this->metricsGenerator = new MetricsGenerator();
    this->metricsGenerator->initialize(this);

    if(this->adaptive_controller == NULL){
        this->adaptive_controller = new AdaptiveController();
        this->adaptive_controller->initialize(this);
    }

    if(this->connections == NULL){
        this->connections = this->factory->newConnections(this->nThreads);
        this->connections->initialize(this);
        Follower::connections = this->connections;
    }

    if(this->storage == NULL){
        this->storage = this->factory->newStorage("monitoring.db");
        Follower::storage = this->storage;
    }

    Follower::initialize(this->factory);
}

void AdaptiveFollower::start(vector<Message::node> mNodes){
    Follower::start(mNodes);

    this->metricsGenerator->start();
    this->adaptive_controller->start();
}

void AdaptiveFollower::stop(){
    if(this->metricsGenerator){
        this->metricsGenerator->stop();
    }

    if(this->adaptive_controller){
        this->adaptive_controller->stop();
    }

    Follower::stop();
}


vector<Metric> AdaptiveFollower::getMetrics(){
    vector<Metric> res;
    
    for(auto const& m : this->metrics){
        if(m.second == true){
            res.push_back(m.first);
        }
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

IAdaptiveFollowerConnections* AdaptiveFollower::getConnections() {
    return this->connections;
}

IAdaptiveStorageMonitoring* AdaptiveFollower::getStorage() {
    return this->storage;
}

void AdaptiveFollower::getHardware(){
    Report::hardware_result hardware;

    sigar_t *sigar;
    sigar_open(&sigar);

    if(metrics[FREE_CPU]){
        int status, i;
        sigar_cpu_t cpuT1;
        sigar_cpu_t cpuT2;
        sigar_cpu_list_t cpulist;               // number of cores

        status = sigar_cpu_list_get(sigar, &cpulist);
        if (status != SIGAR_OK) {
            printf("cpu_list error: %d (%s)\n",
                status, sigar_strerror(sigar, status));
            exit(1);
        }

        status = sigar_cpu_get(sigar, &cpuT1);
        if (status != SIGAR_OK) {
            printf("cpu error: %d (%s)\n",
                status, sigar_strerror(sigar, status));
            exit(1);
        }

        sleeper.sleepFor(chrono::seconds(1));

        status = sigar_cpu_get(sigar, &cpuT2);
        if (status != SIGAR_OK) {
            printf("cpu error: %d (%s)\n",
                status, sigar_strerror(sigar, status));
            exit(1);
        }

        unsigned long long diffIdle = cpuT2.idle - cpuT1.idle;
        unsigned long long totaldiff = cpuT2.total - cpuT1.total + cpuT2.user - cpuT1.user + cpuT2.sys - cpuT1.sys;

        hardware.cores = cpulist.number;

        hardware.mean_free_cpu = MetricsGenerator::currentVal[FREE_CPU];
        
        //hardware.mean_free_cpu = ((float)diffIdle)/(totaldiff);
        

        sigar_cpu_list_destroy(sigar, &cpulist);
    }

    if(metrics[FREE_MEMORY]){
        sigar_mem_t mem;
        sigar_mem_get(sigar,&mem);

        //hardware.memory = MetricsGenerator::currentVal.total_memory;
        hardware.memory = mem.total;

        hardware.mean_free_memory = MetricsGenerator::currentVal[FREE_MEMORY];

        //hardware.mean_free_memory = mem.actual_free;
    }

    if(metrics[FREE_DISK]){
        sigar_file_system_usage_t disk;
        sigar_file_system_usage_get(sigar,"/",&disk);

        //hardware.disk = MetricsGenerator::currentVal.total_disk;

        hardware.disk = disk.total;

        hardware.mean_free_disk = MetricsGenerator::currentVal[FREE_DISK];

        //hardware.mean_free_disk = disk.avail;
    }

    if(metrics[FREE_CPU] ||
       metrics[FREE_MEMORY] ||
       metrics[FREE_DISK])
        this->storage->saveHardware(hardware, this->node->hardwareWindow);          // save hardware data on DB

    sigar_close(sigar);
}

void AdaptiveFollower::getBattery(){
    
    AdaptiveReport::battery_result battery;

    if(metrics[BATTERY]){
        battery.mean_battery = MetricsGenerator::currentVal[BATTERY];
        this->storage->saveBattery(battery, this->node->hardwareWindow);
    }
}

void AdaptiveFollower::timer() {

    cout << this->node->isFollower() << endl;

    int iter=0;
    while(this->running) {

        auto t_start = std::chrono::high_resolution_clock::now();

        std::unique_lock<std::mutex> lck(this->adaptive_controller->mtx);

        this->getHardware();
        this->getBattery();

        std::optional<std::pair<int64_t,Message::node>> ris = this->connections->sendUpdate(this->nodeS, this->update); // manda update al nodo leader
        if(ris == nullopt) {           // il messaggio di update non ha ottenuto ack
            cout << "update retry..." << endl;
            ris = this->connections->sendUpdate(this->nodeS,this->update);  
            if(ris == nullopt) {
                //change server
                cout << "Changing server..." << endl;
                if(!selectServer(this->node->getMNodes())) {        // cerca un nuovo leader
                    cout << "Failed to find a server!!!!!!!!" << endl;
                }
                iter=0;
            }
        }

        if(ris != nullopt) {    // l'update è andato a buon fine
            nUpdate += 1;
            cout << "Number of updates sent until now: " << nUpdate << endl;

            // aggiorna variabile membro 'update'
            this->update.first= (*ris).first;
            this->update.second= (*ris).second;
        }

        AdaptiveController::ready = true;
        lck.unlock();
        this->adaptive_controller->cv.notify_one();

        //every 10 iterations ask the nodes in case the server cant reach this network
        if(iter%10 == 0) {
            vector<Message::node> ips = this->connections->requestNodes(this->nodeS);   // chiede al Leader gli ip dei Follower nel suo gruppo
            vector<Message::node> tmp = this->getStorage()->getNodes();                 // nodi già conosciuti dal Follower
            vector<Message::node> rem;                                                  // nodi da rimuovere dai nodi conosciuti dal Follower (tmp -ips)

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

            this->getStorage()->updateNodes(ips,rem);       // aggiunge nodi nuovi (ips)        
                                                            // elimina dallo storage i nodi che non vengono restituiti dal leader (rem)
        }

        //every leaderCheck iterations update the MNodes
        if(iter% this->node->leaderCheck == this->node->leaderCheck-1) {
            vector<Message::node> res = this->connections->requestMNodes(this->nodeS);  // chiede al Leader gli ip di tutti i Leader della rete
            if(!res.empty()) {
                for(int j=0; j<res.size(); j++)
                {
                    if(res[j].ip==std::string("::1")||res[j].ip==std::string("127.0.0.1"))
                        res[j].ip = this->nodeS.ip;         // sostituzione ip locale del leader con ip esterno
                }
                this->node->setMNodes(res);                 // aggiornamento dei nodi leader conosciuti
                cout << "Check server" << endl;
                bool change = this->checkServer(res);       // controlla se c'è un nodo con latenza minore e nel caso cambia Leader/gruppo
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
        if(metrics[CONNECTED_IOTS] &&
           iter%4 == 0){
            cout << "Measuring IoTs..." << endl;
            this->testIoT();
        }


        vector<thread> LatencyThreads;
        if(metrics[LATENCY]){
            //get list ordered by time for the latency tests
            //test the least recent
            cout << "Measuring Latency..." << endl;
            vector<Message::node> ips = this->storage->getLRLatency(this->node->maxPerLatency, this->node->timeLatency);

            for(auto node : ips) {
                if(this->getMyNode().id == node.id)
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
                    if(this->getMyNode().id == ips[i].id) {
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
    
        if(metrics[LATENCY])
            for(auto &LatencyThread : LatencyThreads)
                LatencyThread.join();

        if(metrics[BANDWIDTH])   
            BandwidthThread.join();

        sleeper.sleepFor(chrono::seconds(this->node->timeTests));
        iter++;
    }
}

bool AdaptiveFollower::changeServer(vector<Message::node> mNodes) {
    if(!this->node->isFollower()){
        if(!this->connections->sendHello(this->nodeS));
            return false;
        return true;
    }

    vector<Message::node> res = mNodes;
    if(!res.empty()) {
        for(int i=0; i<res.size(); i++) {
            if(res[i].ip==std::string("::1")||res[i].ip==std::string("127.0.0.1"))
                res[i].ip = this->nodeS.ip;
        }
        this->node->setMNodes(res);
    }

    while(!res.empty()) {
        int imin=0;
        unsigned int min = (unsigned int)this->testPing(res[imin].ip);

        for(int i=1; i<res.size(); i++) {
            unsigned int tmp = (unsigned int)this->testPing(res[i].ip);
            if(tmp < min) {
                imin = i;
                min = tmp;
            }
        }
        this->nodeS = res[imin];
        if(!this->connections->sendHello(this->nodeS)) {
            res.erase(res.begin()+imin);
        }else
            return true;
        
    }
    return false;
}