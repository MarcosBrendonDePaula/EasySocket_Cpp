#include "include/EasyMultServer.h"
#include "include/Events.h"
#include <iostream>
using namespace std;

Events Eventos;
EasyMultServer Servidor(1234,&Eventos);

void Resposta(void *arg){
    Nsock *socket = (Nsock*)arg;
    string txt(socket->getInput());
    if(txt=="quem_sou_eu"){
        socket->SendMsg("Nem sei mano");
    }else
    if(txt=="qual_o_melhor_champ_do_lol"){
        socket->SendMsg("depende");
    }else
    if(txt=="meu_nome"){
        socket->SendMsg("Marcos");
    }
    else{
        socket->SendMsg("nao entendi");
    }
}

int main(){
    Servidor.Start(Resposta);
    
    int x;
    cin>>x;
    return 0;
}