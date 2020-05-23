#include "../../include/EasyModules.hpp"
#include "../../include/EasySocket.h"
#include "../../include/Nsock.h"
#include "../../include/Events.h"
#include <cstring>
#include <vector>
#include <fstream>

void response(void *arg){
    if(((EasySocket*)arg)->getInput().find("getmusica.mp3")==0){
        EasyModule::sendFile(((EasySocket*)arg)->getSocket(),"musica.mp3");
    }
}

int main(){
    Events Ev;
    EasySocket socket("127.0.0.1",25565,&response,&Ev);
    socket.Connect();
    socket.SendMsg("RecvArq");
    int x;
    cin>>x;
    return 0;
}