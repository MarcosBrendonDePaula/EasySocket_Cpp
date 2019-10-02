#include <iostream>
#include "EasyMultServer.h"
#include <winsock2.h>
#include <sstream>
//#include "EasySocket.h"
using namespace std;
EasyMultServer sv(25565);

void tratamento(void *arg){
	nsock* socket=(nsock*)arg;
}
int main(){
	sv.StartServer(&tratamento);
	while(true){
		int x;
		cin>>x;
	}
}