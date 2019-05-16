#include "connections.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <errno.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/un.h>
#include <string>
#include <netdb.h>
#include <iostream>

using namespace std;

Connections::Connections(int nThread) : IConnections(nThread) {
}

Connections::~Connections() {
}

void Connections::initialize(INode *parent) {
    IConnections::initialize(parent);
    this->parent = parent;
}

void Connections::handler(int fd, Message &m) {
    string strIp = this->getSource(fd,m);

    if(m.getType() == Message::Type::REQUEST) {
        if(m.getArgument() == Message::Argument::IPERF) {
            if(m.getCommand() == Message::Command::START) {
                
                Message res;
                res.setType(Message::Type::RESPONSE);
                res.setCommand(Message::Command::START);
                int port = this->parent->getIperfPort();
                if(port > 0) {
                    res.setArgument(Message::Argument::POSITIVE);
                    res.setData(port);
                }else {
                    res.setArgument(Message::Argument::NEGATIVE);
                }
                //send response
                if(this->sendMessage(fd, res)) {
                    
                }
            }
        }else if(m.getArgument() == Message::Argument::ESTIMATE) {
            if(m.getCommand() == Message::Command::START) {
                
                Message res;
                res.setType(Message::Type::RESPONSE);
                res.setCommand(Message::Command::START);
                int port = this->parent->getEstimatePort();
                string a = string(" ");
                string port_ = to_string(port);
                Message::node val(a,strIp,port_);
                if(port > 0) {
                    res.setArgument(Message::Argument::POSITIVE);
                    res.setData(val);
                }else {
                    res.setArgument(Message::Argument::NEGATIVE);
                }
                //send response
                if(this->sendMessage(fd, res)) {
                    
                }
            }
        }else if(m.getArgument() == Message::Argument::NODES) {
            if(m.getCommand() == Message::Command::GET) {
                //build array of nodes
                vector<Message::node> nodes = this->parent->getStorage()->getNodes();

                //send nodes
                Message res;
                res.setType(Message::Type::RESPONSE);
                res.setCommand(Message::Command::GET);
                res.setArgument(Message::Argument::POSITIVE);

                res.setData(nodes);
            
                if(this->sendMessage(fd, res)) {
                    
                }
            }else if(m.getCommand() == Message::Command::SET) {
                Message res; //--------------------------------
                res.setType(Message::Type::RESPONSE);
                res.setCommand(Message::Command::SET);

                //refresh all the nodes with the array of nodes
                vector<Message::node> ips;
                if(!m.getData(ips)) {
                    res.setArgument(Message::Argument::NEGATIVE);
                }else {
                    res.setArgument(Message::Argument::POSITIVE);
                }
                //ips now contains the ip of the nodes
                this->parent->getStorage()->refreshNodes(ips);

                //send report
                if(this->sendMessage(fd, res)) {
                    
                }
            }
        }else if(m.getArgument() == Message::Argument::REPORT) {
            if(m.getCommand() == Message::Command::GET) {
                //build report
                Message res;
                res.setType(Message::Type::RESPONSE);
                res.setCommand(Message::Command::GET);
                res.setArgument(Message::Argument::POSITIVE);
                Report r;
                
                r.setHardware(this->parent->getStorage()->getHardware());
                r.setLatency(this->parent->getStorage()->getLatency());
                r.setBandwidth(this->parent->getStorage()->getBandwidth());
                r.setIot(this->parent->getStorage()->getIots());
                res.setData(r);

                //send report
                if(this->sendMessage(fd, res)) {
                    
                }
                
            }
        }
    }else if(m.getType() == Message::Type::NOTIFY) {
        if(m.getCommand() == Message::Command::UPDATE) {
            if(m.getArgument() == Message::Argument::NODES) {
                //data contains 2 array: new and deleted nodes
                vector<Message::node> ipsNew;
                vector<Message::node> ipsRem;
                if(!m.getData(ipsNew,ipsRem))
                    return;
                //update the nodes
                this->parent->getStorage()->updateNodes(ipsNew,ipsRem);
            }
        }
    }   
}

vector<Message::node> Connections::requestNodes(Message::node ipS) {
    int Socket = openConnection(ipS.ip, ipS.port);
    
    if(Socket < 0) {
        return vector<Message::node>();
    }

    //build request message
    Message m;
    m.setSender(this->parent->getMyNode());
    m.setType(Message::Type::REQUEST);
    m.setCommand(Message::Command::GET);
    m.setArgument(Message::Argument::NODES);

    vector<Message::node> nodes;
    bool result = false;
    //send request message
    if(this->sendMessage(Socket, m)) {
        Message res;
        if(this->getMessage(Socket, res)) {
            if( res.getType()==Message::Type::RESPONSE &&
                res.getCommand() == Message::Command::NODELIST &&
                res.getArgument() == Message::Argument::POSITIVE) {
                if(res.getData(nodes)) {
                    result = true;
                }
            }
        }
    }
    close(Socket);
    if(result)
        return nodes;
    return vector<Message::node>();
}

vector<Message::node> Connections::requestMNodes(Message::node ipS) {
    int Socket = openConnection(ipS.ip, ipS.port);
    
    if(Socket < 0) {
        return vector<Message::node>();
    }

    fflush(stdout);

    //build request message
    Message m;
    m.setSender(this->parent->getMyNode());
    m.setType(Message::Type::REQUEST);
    m.setCommand(Message::Command::GET);
    m.setArgument(Message::Argument::MNODES);

    vector<Message::node> nodes;
    bool result = false;
    //send request message
    if(this->sendMessage(Socket, m)) {
        Message res;
        if(this->getMessage(Socket, res)) {
            if( res.getType()==Message::Type::RESPONSE &&
                res.getCommand() == Message::Command::MNODELIST &&
                res.getArgument() == Message::Argument::POSITIVE) {
                if(res.getData(nodes)) {
                    result = true;
                }
            }
        }
    }
    close(Socket);
    if(result)
        return nodes;
    return vector<Message::node>();
}

bool Connections::sendHello(Message::node ipS) {
    cout << "Trying server " << ipS.id << " " << ipS.ip << ":" << ipS.port <<endl;
    int Socket = openConnection(ipS.ip, ipS.port);
    if(Socket < 0) {
        return false;
    }

    fflush(stdout);
    char buffer[10];

    //build hello message
    Message m;
    m.setSender(this->parent->getMyNode());
    m.setType(Message::Type::NOTIFY);
    m.setCommand(Message::Command::HELLO);
    Report r;
    
    r.setHardware(this->parent->getStorage()->getHardware());
    m.setData(r);
    bool result = false;

    //send hello message
    if(this->sendMessage(Socket, m)) {
        Message res;
        if(this->getMessage(Socket, res)) {
            if( res.getType()==Message::Type::RESPONSE &&
                res.getCommand() == Message::Command::HELLO &&
                res.getArgument() == Message::Argument::POSITIVE) {
                Message::node node;
                vector<Message::node> vec;
                if(res.getData(node, vec)) {
                    cout << "My id: " << node.id << " " << node.ip << ":" << node.port << endl;
                    this->parent->setMyId(node.id);
                    this->parent->getStorage()->setFilter(ipS.ip);
                    this->parent->getStorage()->refreshNodes(vec);
                    result = true;
                    cout << "Server: " << ipS.id << " " << ipS.ip << ":" << ipS.port << endl;
                }
            }
        }
    }
    close(Socket);
    return result;
}

bool Connections::sendUpdate(Message::node ipS) {
    int Socket = openConnection(ipS.ip, ipS.port);
    
    if(Socket < 0) {
        return false;
    }

    fflush(stdout);
    char buffer[10];

    //build update message
    Message m;
    m.setSender(this->parent->getMyNode());
    m.setType(Message::Type::NOTIFY);
    m.setCommand(Message::Command::UPDATE);
    m.setArgument(Message::Argument::REPORT);
    Report r;
    
    r.setHardware(this->parent->getStorage()->getHardware());
    r.setLatency(this->parent->getStorage()->getLatency());
    r.setBandwidth(this->parent->getStorage()->getBandwidth());
    r.setIot(this->parent->getStorage()->getIots());

    m.setData(r);

    

    bool result = false;

    //send update message
    if(this->sendMessage(Socket, m)) {
        Message res;
        if(this->getMessage(Socket, res)) {
            if( res.getType()==Message::Type::RESPONSE &&
                res.getCommand() == Message::Command::UPDATE &&
                res.getArgument() == Message::Argument::POSITIVE) {
                
                result = true;
            }
        }
    }
    close(Socket);
    return result;
}

int Connections::sendStartIperfTest(Message::node ip) {
    int Socket = openConnection(ip.ip, ip.port);
    
    if(Socket < 0) {
        return -1;
    }

    fflush(stdout);
    char buffer[10];

    //build start iperf message
    Message m;
    m.setSender(this->parent->getMyNode());
    m.setType(Message::Type::REQUEST);
    m.setCommand(Message::Command::START);
    m.setArgument(Message::Argument::IPERF);

    int port = -1;

    //send start iperf message
    if(this->sendMessage(Socket, m)) {
        Message res;
        if(this->getMessage(Socket, res)) {
            if( res.getType()==Message::Type::RESPONSE &&
                res.getCommand() == Message::Command::START &&
                res.getArgument() == Message::Argument::POSITIVE) {
                
                res.getData(port);
            }
        }
    }
    close(Socket);
    return port;
}

int Connections::sendStartEstimateTest(Message::node ip, std::string &myIp) {
    int Socket = openConnection(ip.ip, ip.port);
    
    if(Socket < 0) {
        return -1;
    }

    fflush(stdout);
    char buffer[10];

    //build start estimate message
    Message m;
    m.setSender(this->parent->getMyNode());
    m.setType(Message::Type::REQUEST);
    m.setCommand(Message::Command::START);
    m.setArgument(Message::Argument::ESTIMATE);

    int port = -1;

    //send start estimate message
    if(this->sendMessage(Socket, m)) {
        Message res;
        if(this->getMessage(Socket, res)) {
            if( res.getType()==Message::Type::RESPONSE &&
                res.getCommand() == Message::Command::START &&
                res.getArgument() == Message::Argument::POSITIVE) {
                Message::node val;
                res.getData(val);
                port = stol(val.port);
                myIp = val.ip;
            }
        }
    }
    close(Socket);
    return port;
}