#include "../include/EasyMultServer.h"
#include "../include/Events.h"
using namespace std;

void process(void* arg){
    Nsock *Socketc = (Nsock*)arg;
    char *input = Socketc->getInputVector();
    cout<<input<<endl;
    free(input);
}   

void connection(void* arg){
    Nsock *Socketc = (Nsock*)arg;
}

void dconnection(void* arg){
    Nsock *Socketc = (Nsock*)arg;
}

int main(){
    Events Eventos;
    Eventos.Paralel = true;
    EasyMultServer servidor(25565,&Eventos);
    servidor.Start(&process,900,&connection,&dconnection);
    while(true){
        string input;
        cin>>input;
        for(auto i : servidor.Connections){
            i.second->SendMsg(input);
        }
    }
    return 0;
}