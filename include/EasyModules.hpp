#include "EasyMultServer.h"
#include "EasySocket.h"
#include <fstream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
#include <cstdint>

namespace EasyModule{
	
	/*!
	* \fn vector<string> split(string str,char delimiter)
	* \param str string to be broken into n sub-strings.
	* \param delimiter string delimiter.
	*/
    vector<string> split(string str,char delimiter){
		stringstream ss(str);string token;vector<string> keys;
		while (getline(ss, token, delimiter)){keys.push_back(token);}
		return keys;
	}
	
	/*!
	* \fn template<typename __type> bool sendVar(int sock,__type var)
	* \brief send a variable of type type that was indicated in the template..
	* \param sock socket number referring to the connection.
	* \param var variable address whose data will be sent.
	*/
    template<typename __type> bool sendVar(int sock,__type var){
			char *temp = (char*)&var;
			if(send(sock,temp,sizeof(__type),0)>=0){return true;}
			return false;
	}
	
	/*!
	* \fn template<typename __type> __type* ReciveVar(int sock,__type* rec)
	* \brief receives a variable of type _type whose was indicated in the template.
	* \param sock socket number referring to the connection.
	* \param rec address of the variable that receives the data.
	*/
    template<typename __type> __type* ReciveVar(int sock,__type* rec){
        __type *temp=rec;
        if(recv(sock,(char*)temp,sizeof(__type),0)==sizeof(__type)){return temp;};
        return NULL;
    }

    /*!
	* \fn void sendFile(int socket,ifstream& in,uint64_t tick_percent)
	* \brief Function for sending files.
    * \param sock socket number referring to the connection.
    * \param in the file must be opened with (ios :: binary).
	* \param tick_percent maximum ticks to print the percentage.
    */
    void sendFile(int socket,ifstream& in,uint64_t tick_percent=0){
        in.seekg(0,ios::end);
        uint64_t file_size = in.tellg();
        in.seekg(0,ios::beg);
        
        sendVar<uint64_t>(socket,file_size);// Ignore.

        char *Buffer = new char[file_size];
        in.read(Buffer,file_size);
        uint64_t tick_counter=0;
        float a,b,c=100.0;
        for(uint64_t i=0;i<=file_size;){
            if((tick_counter==tick_percent)*(tick_percent>0)){
                tick_counter=0;
                a=i;b=file_size;
                cout<<"R:"<<((a/b)*c)<<"%"<<endl;
            }

            if((i+500)<file_size){
                send(socket,Buffer+i,500,0);
                i+=500;
                tick_counter++;
            }else{
                send(socket,Buffer+i,(file_size-i),0);
                i+=(file_size-i);
                tick_counter++;
                break;
            }
        }
        if(tick_percent){
            cout<<"R:100"<<"%"<<endl;
        }
        delete[] Buffer;
    }

    /*!
	* \fn bool recvFile(int socket,ofstream *out,uint64_t tick_percent).
	* \brief Function for sending files.
    * \param sock socket number referring to the connection.
    * \param out address of the variable responsible for receiving the file. must be open as (ios :: binary).
	* \param tick_percent maximum ticks to print the percentage.
    */
    bool recvFile(int socket,ofstream *out,uint64_t tick_percent=0){
        uint64_t *num_bits = ReciveVar<uint64_t>(socket,new uint64_t);
        if(num_bits==NULL){
            return false;
        }
        uint64_t b_seek,tick_counter=0;
        uint64_t b_max=500;
        char *Buffer = new char[b_max];
        float a,b,c=100.0;
        for(uint64_t i=0;i!=(*num_bits);) {
            
            if((tick_counter==tick_percent)*tick_percent){
                tick_counter=0;
                a=i;b=(*num_bits);
                cout<<"R:"<<((a/b)*c)<<"%"<<endl;
            }

            memset(Buffer,'\000',b_max);
            b_seek = recv(socket,Buffer,500,0);
            if(b_seek==-1){
                return false;
            }
            out->write(Buffer,b_seek);
            i+=b_seek;
            tick_counter++;
        }
        if(tick_percent){
            cout<<"R:100"<<"%"<<endl;
        }
        delete[] Buffer;
        return true;
    }
}