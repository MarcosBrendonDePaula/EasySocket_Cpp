#include "../../include/EasyModules.hpp"
#include "../../include/EasySocket.h"
#include "../../include/Nsock.h"
#include "../../include/Events.h"
#include <cstring>
#include <vector>
#include <fstream>

void response(void *arg){
    if(((EasySocket*)arg)->getInput().find("getmusica.mp3")==0){
        ifstream arq("musica.mp3",ios::binary|ios::in);
        EasyModule::sendFile(((EasySocket*)arg)->getSocket(),arq);
        cout<<"Enviado!"<<endl;
    }
}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    Events Ev;
    EasySocket socket("192.168.1.10",25565,&response,&Ev);
    socket.Connect();
    socket.SendMsg("RecvArq");
    int x;
    cin>>x;
    return 0;
}