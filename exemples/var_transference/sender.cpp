#include "../../include/EasyModules.hpp"
#include "../../include/EasySocket.h"
#include "../../include/Nsock.h"
#include "../../include/Events.h"
#include <cstring>
#include <vector>
#include <fstream>
class Clsexemple{
    public:
        string nome;
        int age;
        int date;
        int Numbers[5];
};
Clsexemple temp;
void response(void *arg){
    if(((EasySocket*)arg)->getInput().find("RecVar")==0){
        EasyModule::sendVar<Clsexemple>(((EasySocket*)arg),temp);
    }
}

int main(){
    Events Ev;
    EasySocket socket("127.0.0.1",25565,&response,&Ev);
    socket.Connect();
    temp.age=30;
    temp.date=1041999;
    temp.nome="teste";
    for(int i=0;i<5;i++){
        temp.Numbers[i]=i+1;
    }

    socket.SendMsg("sendVar");
    int x;
    cin>>x;
    return 0;
}