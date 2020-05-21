#include "../../include/EasyModules.hpp"
#include "../../include/EasySocket.h"
#include "../../include/Nsock.h"
#include "../../include/Events.h"
#include <cstring>
#include <vector>
#include <fstream>
void response(void *arg){
    
}

int main(){
    Events Ev;
    EasySocket socket("127.0.0.1",25565,&response,&Ev);
    socket.Connect();
    socket.SendMsg("RecvArq");
    sleep(1);
    EasyModule::sendFile(socket.getSocket(),"sender.cpp");
    int x;
    cin>>x;
    return 0;
}