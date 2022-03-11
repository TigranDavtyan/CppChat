#include <sockethelper.hpp>

const int MAX_CONN = 100;
SOCKET connections[MAX_CONN];
std::string usernames[MAX_CONN];
int cc = 0;

void SendAllBut(std::string message,int index){
    print(message);
    for(int i = 0; i < cc; ++i){
        if(i == index) continue;
        SK::Send(connections[i],message);
    }
}

void SendAll(std::string message){
    print(message);
    for(int i = 0;i < cc; ++i){
        SK::Send(connections[i],message);
    }
}

void ClientHandler(int index){
    std::string msg;
    while(true){
        msg = SK::Recv(connections[index]);
        if(msg == "/exit"){
            SendAllBut(usernames[index] + " left the chat.",index);
            closesocket(connections[index]);

            for(int i = index; i < cc-1; i++){
                connections[i] = connections[i+1];
                usernames[i] = usernames[i+1];
            }
            cc--;
            break;
        }
        SendAllBut(usernames[index] + " : " + msg,index);
    }
}

int main(){
    SK::Init();
    
    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    SOCKET sListen = socket(AF_INET,SOCK_STREAM,NULL);
    bind(sListen,(SOCKADDR*)&addr,sizeof(addr));
    listen(sListen,SOMAXCONN);

    std::string msg;
    print("---------------- CppChat ----------------\n");
    for(int i=0; i < MAX_CONN; i++){
        connections[cc] = accept(sListen,(SOCKADDR*)&addr,&sizeofaddr);
        if(connections[cc] == 0){
            std::cout<<"Error #2\n";
        } else {
            SK::Send(connections[cc],"---------------- CppChat ----------------\nType /exit to exit chat\nEnter your name\n");
            usernames[cc] = SK::Recv(connections[cc]);
            CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)ClientHandler,(LPVOID)(cc),NULL,NULL);
            cc++;
            SendAll(usernames[cc-1] + " joined chat.");
            print("Number of connections = ",cc);
        }
    }

    return 0;
}