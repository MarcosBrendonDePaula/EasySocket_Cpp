#include "EasyMultServer.h"
#include <map>
using namespace std;
EasyMultServer Servidor;
map<string,string> resposta;

int ServerKey=1;
void tratamento(void *arg){
    nsock *Cliente = (nsock*)arg;
    string req = Cliente->getEntrada();
    if(resposta[req]!=""){

    }else{
        
    }
    
}
int main(){
    Servidor.StartServer(&tratamento,1);
    cout<<"Digite 0 para encerrar"<<endl;
    int x=1;
    while(x){
        cin>>x;
    }
}