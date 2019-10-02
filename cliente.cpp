
#include "EasySocket.h"
#include <iostream>

using namespace std;
EasySocket *Socket;

void tratamento(void *arg){
    cout<<Socket->getEntrada()<<endl;
}
WSADATA dll;
int main(){
    WSAStartup(MAKEWORD(2,2), &dll);
    Socket=new EasySocket("127.0.0.1",25565,&tratamento);
	Socket->conectar();
    while(true){
        Socket->Enviar("Iguatapiringainguaaaaaaaaaa");
    }
    return 0;
}