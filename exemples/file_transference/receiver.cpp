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
        ofstream *arq = new ofstream("arq.rec",ios::binary|ios::out);
        if(EasyModule::recvFile(*sock->getClient(),arq)){
            cout<<"terminou"<<endl;
        }else
        {
            cout<<"Erro ao Receber"<<endl;
        }
        arq->close();
        cout<<"ok"<<endl;
    }

}

int main(){
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    Events Eventos;
    EasyMultServer servidor(25565,&Eventos);
    servidor.Start(&Request);
    Eventos.Paralel=true;
    int x;
    cin>>x;
    return 0;
}