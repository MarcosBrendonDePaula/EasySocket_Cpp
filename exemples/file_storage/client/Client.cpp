#include "../../../include/EasySocket.h"
#include "../../../include/Events.h"
#include "../../../include/EasyModules.hpp"
#define ticknum 100
void Response(void *arg){
    EasySocket *Socket = (EasySocket*)arg;
    int sock = Socket->getSocket();
    string message = Socket->getInput();
    if(message.find("getFile,")==0){
        vector<string> inputs{EasyModule::split(message,',')}; //[0] COMAND , [1] FILENAME
        ofstream arq(inputs[1],ios::binary|ios::out);
        EasyModule::recvFile(sock,&arq,ticknum);
        arq.close();
    }else
    if(message.find("sendFile,")==0){
        vector<string> inputs{EasyModule::split(message,',')}; //[0] COMAND , [1] FILENAME
        ifstream temp(inputs[1].c_str(),ios::binary|ios::in);
        if(temp.is_open()){
            Socket->SendMsg("getFile,"+inputs[1]);
            EasyModule::sendFile(sock,temp,ticknum);
        }
        temp.close();
    }
}

int main(){
    string ip;
    cout<<"informe O ip : ";
    cin>>ip;
    cout<<endl;
    EasySocket Socket(ip,3334,&Response,new Events);
    if(Socket.Connect()){
        while(true){
            cout<<"0 -> SendFile"<<endl;
            cout<<"1 -> RecvFile"<<endl;
            int command;
            cin>>command;
            string filename;
            if(command==1){
                cout<<"Recive File Name: ";
                cin>>filename;
                cout<<endl;
                Socket.SendMsg("sendFile,"+filename);
            }
            else if(!command){
                cout<<"send file name: ";
                cin>>filename;
                cout<<endl;
                ifstream temp(filename,ios::binary|ios::in|ios::ate);
                if(temp.is_open()){
                    Socket.SendMsg("getFile,"+filename);
                    EasyModule::sendFile(Socket.getSocket(),temp,ticknum);
                }
            }
        }
    }
    return 0;
}