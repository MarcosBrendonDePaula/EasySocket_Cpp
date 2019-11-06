#include <iostream>
#include "EasyMultServer.h"
#include <winsock2.h>
#include <stdexcept>
#include <sstream>
#include "Json.h"
using namespace std;
EasyMultServer sv(25565);
Json novo;
void tratamento(void *arg){
	nsock* socket=(nsock*)arg;
	try{
        cout<<socket->getEntrada()<<endl;
        socket->Enviar(novo.to_string());
    }catch(exception& erro){
        cout<<erro.what()<<endl;
    }
}
int main(){
    
    novo.addVar("anime","aldnoha:Zero");
    novo.addVarPath("mensagens","1","Aloha");
    novo.addVarPath("mensagens","2","bilibili");
    novo.addVarPath("mensagens","3","kakaka");
    novo.addVarPath("mensagens","4","digo doido");
    novo.addVarPath("mensagens","5","maluko");
    novo.addVarPath("mensagens","6","vish");
    novo.addVarPath("kits","pvp","espada,arco,flexa");
    novo.addVarPath("kits","sobrevivencia","machado,faca,picareta");

	sv.StartServer(&tratamento);
	
	while(true){
		int x;
		cin>>x;
	}
}