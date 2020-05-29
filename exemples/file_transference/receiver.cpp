#include "../../include/EasyModules.hpp"
#include "../../include/EasyMultServer.h"
#include "../../include/Events.h"
#include <sstream>
#include <fstream>
void Request(void *arg){
    Nsock *sock = ((Nsock*)arg);
    string msg = sock->getInput();
    if(msg.find("RecvArq")==0){
        sock->SendMsg("getmusica.mp3");
        EasyModule::File arq=EasyModule::recivePreFile(*sock->getClient(),"arq.rec",true);
        string outname = "saida.rar";
        arq.FileSave(outname);
        arq.ClearMem();
        cout<<"ok"<<endl;
    }

}

int main(){
    Events Eventos;
    EasyMultServer servidor(25565,&Eventos);
    servidor.Start(&Request);
    Eventos.Paralel=true;
    int x;
    cin>>x;
    return 0;
}