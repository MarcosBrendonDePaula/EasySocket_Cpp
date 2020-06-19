#include "../../../include/EasyMultServer.h"
#include "../../../include/Events.h"
#include "../../../include/EasyModules.hpp"
#define ticknum 100
using namespace std;
Events Eventos;
EasyMultServer *Servidor;

void Response(void *arg){
    Nsock *Socket = (Nsock*)arg;
    string message = Socket->getInput();
    int sock = *Socket->getClient();
    if(message.find("getFile,")==0){
        vector<string> inputs{EasyModule::split(message,',')}; //[0] COMAND , [1] FILENAME
        ofstream arq(inputs[1],ios::binary|ios::out);
        EasyModule::recvFile(sock,&arq,ticknum);
        arq.close();
    }else
    if(message.find("sendFile,")==0){
        vector<string> inputs{EasyModule::split(message,',')}; //[0] COMAND , [1] FILENAME
        ifstream temp(inputs[1].c_str(),ios::binary|ios::in);
        if(temp.is_open()){
            Socket->SendMsg("getFile,"+inputs[1]);
            EasyModule::sendFile(sock,temp,ticknum);
        }
        temp.close();
    }
}

int main(){
    Servidor = new EasyMultServer(3334,&Eventos);
    Servidor->Start(&Response);
    int x;
    cin>>x;
    return 0;
}