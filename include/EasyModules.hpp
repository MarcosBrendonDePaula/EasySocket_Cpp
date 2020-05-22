#include "EasyMultServer.h"
#include "EasySocket.h"
#include <fstream>
namespace EasyModule{
    void reciveFile(int socket,string out){
        ofstream file(out,ios::out | ios::binary);
        int size;
        do{
            char buff[1499]={};
            size = recv(socket,buff,1499,0);
            if(strcmp(buff,"{-endf-}")==0)
                break;
            file.write(buff,size);
        }while(size==1499);
        file.close();
    }

    void sendFile(int socket,string in){
        ifstream file(in,ios::binary|ios::in|ios::ate);
        int size = file.tellg();
        file.seekg (0, ios::beg);
        if(size>=1499){
            char buff[1499]={};
            file.read(buff,1499);
            do{
                send(socket,buff,1499,0);
            }while(file.read(buff,1499));
        }else{
            char buff[size]={};
            file.read(buff,size);
            send(socket,buff,size,0);
        }
        char buffF[]="{-endf-}";
        send(socket,buffF,8,0);
    }
    //-------------------------------------------------------------------Nsock
    template<typename __type> bool sendVar(Nsock* socket,__type var){
			char *temp = (char*)&var;
			if(send(socket->getClient(),temp,sizeof(__type),0)>=0){return true;}
			return false;
	}

    template<typename __type> __type* ReciveVar(Nsock* socket,__type* rec){
        __type *temp=rec;
        if(recv(socket->getClient(),(char*)temp,sizeof(__type),0)>0){return true;};
        return temp;
    }

    //-------------------------------------------------------------------EasySocket
    template<typename __type> bool sendVar(EasySocket* socket,__type var){
        char *temp = (char*)&var;
        if(send(socket->getSocket(),temp,sizeof(__type),0)>=0){return true;}
        return false;
	}

    template<typename __type> __type* ReciveVar(EasySocket*  socket,__type* rec){
        __type *temp=rec;
        if(recv(socket->getSocket(),(char*)temp,sizeof(__type),0)>0){return true;};
        return temp;
    }
}