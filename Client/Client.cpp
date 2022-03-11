#include <sockethelper.hpp>

SOCKET connection;
std::string name = "";
bool first = true;
HANDLE handle;

void Reciever(){
    while(true){
        print("\b\b\b\b",SK::Recv(connection));
        std::cout<<"--> ";
    }
}

int main(){
    if(SK::Init() != 0) return 1;
    
    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    connection = socket(AF_INET,SOCK_STREAM,NULL);
    if(connect(connection,(SOCKADDR*)&addr,sizeof(addr)) != 0){
        std::cout<<"Error: failed to connect to server.\n";
        return 1;
    } else {
        std::string msg;
        std::cout<<"Connected!!!\n";
        handle = CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)Reciever,(LPVOID)(0),NULL,NULL);
        while(true){
            std::cout<<"--> ";
            std::getline(std::cin,msg);
            if(first) {
                name = msg;
                first = false;
            }
            if(msg == "/exit") {
                SK::Send(connection,"/exit");
                break;    
            };
            SK::Send(connection,msg);
        }
        CloseHandle(handle);
        closesocket(connection);
        WSACleanup();
    }

    return 0;
}