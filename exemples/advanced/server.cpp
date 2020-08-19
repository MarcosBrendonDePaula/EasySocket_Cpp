#include <fstream>
#include <iostream>

#include "../../include/EasyModules.hpp"
#include "../../include/EasyMultServer.h"
#include "../../include/Events.h"

using namespace std;

class Config{
    public:
        string BaseDir;
        int maxConnections;
        int port;
        int chkt;
};

Config config;

Events eventos;
EasyMultServer servidor(0,&eventos);

void loadConfig(){
    ifstream confF("server_config.conf");
    string input;
    while(confF>>input){
        vector<string> inputs{EasyModule::split(input,',')};
        if(inputs[0]=="LocalDir"){
            config.BaseDir=inputs[1];
        }else
        if(inputs[0]=="MaxSconnections"){
            config.maxConnections=stoi(inputs[1]);
        }else
        if(inputs[0]=="port"){
            config.port=stoi(inputs[1]);
        }else
        if(inputs[0]=="ckt"){
            config.chkt=stoi(inputs[1]);
        }  
    }
    confF.close();
}

void Response(void *arg){
    Nsock *Socket = (Nsock*)arg;
    string msg=Socket->getInput();
    vector<string> inputs{EasyModule::split(msg,'æ')};
    if(inputs[0]=="sendF"){
        ofstream file(inputs[1],ios::binary|ios::out);
        EasyModule::recvFile(*Socket->getClient(),&file,1000);
        file.close();
    }else if(inputs[0]=="receivF"){
        ifstream file(inputs[1],ios::binary|ios::in);
        EasyModule::sendFile(*Socket->getClient(),file,1000);
    }
}

int main(){
    loadConfig();
    //cout<<config.BaseDir<<endl<<config.maxConnections<<endl<<config.port<<endl;
    servidor = EasyMultServer(config.port,&eventos);
    if(servidor.Start(Response,config.maxConnections)){
        cout<<"Servidor iniciado com porta:"<<config.port;
        cout<<"main loop check time "<<config.chkt<<" segundos"<<endl;
        while(true){
            sleep(config.chkt*1000);
        }    
    }
    else
        cout<<"Erro ao iniciar o Servidor";
    return 0;
}