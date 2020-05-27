#include "../../include/EasyModules.hpp"
#include "../../include/EasySocket.h"
#include "../../include/Nsock.h"
#include "../../include/Events.h"
#include <cstring>
#include <vector>
#include <fstream>
void response(void *arg){
    if(((EasySocket*)arg)->getInput().find("getmusica.mp3")==0){
        EasyModule::sendPreFile(((EasySocket*)arg)->getSocket(),"Among.rar",true);
        cout<<"Enviado!"<<endl;
    }
}

int main(){
    Events Ev;
    EasySocket socket("177.129.123.246",25565,&response,&Ev);
    socket.Connect();
    socket.SendMsg("RecvArq");
    int x;
    cin>>x;
    return 0;
}