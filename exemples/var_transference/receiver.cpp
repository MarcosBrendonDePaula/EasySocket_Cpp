#include "../../include/EasyModules.hpp"
#include "../../include/EasyMultServer.h"
#include "../../include/Events.h"
#include <sstream>
#include <fstream>

class Clsexemple{
    public:
        string nome;
        int age;
        int date;
        int Numbers[5];
};

void Request(void *arg){
    Nsock *sock = ((Nsock*)arg);
    string msg = sock->getInput();
    if(msg.find("sendVar")==0){
        sock->SendMsg("RecVar");
        Clsexemple *input = EasyModule::ReciveVar<Clsexemple>(sock,new Clsexemple());
        cout<<input->age<<" "<<input->date;
        for(int i=0;i<5;i++){
            cout<<" "<<input->Numbers[i];
        }
        cout<<endl;
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