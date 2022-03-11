#pragma once
#include <iostream>
#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <string>
#pragma warning(disable : 4996)

template <typename T>
void print(T t,char end = '\n'){
    std::cout<<t<<"\n";
}

template <typename T,typename TT>
void print(T t,TT tt,char end = '\n'){
    std::cout<<t<<tt<<end;
}

template <typename T,typename TT,typename TTT>
void print(T t,TT tt,TTT ttt,char end = '\n'){
    std::cout<<t<<tt<<ttt<<end;
}

namespace SK {
    int Init() {
        WSAData wsaData;
        WORD DLLVersion = MAKEWORD(2,1);
        if(WSAStartup(DLLVersion, &wsaData) != 0){
            std::cout<<"Error"<<std::endl;
            return 1;
        }
        return 0;
    }

    void Send(SOCKET connection,std::string message) {
        int msg_size = message.size();
        send(connection,(char*)&msg_size,sizeof(int),NULL);
        send(connection,message.c_str(),msg_size,NULL);
        Sleep(1);
    }

    std::string Recv(SOCKET connection) {
        int msg_size;
        recv(connection,(char*)&msg_size,sizeof(int),NULL);

        char* msg = new char[msg_size+1];
        recv(connection,msg,msg_size,NULL);
        msg[msg_size] = '\0';
        
        return std::string(msg);
    }
};