#pragma warning(disable:4996)
#include <iostream>
#include <string>
#include <cstring>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

// 你要链接的服务器IP和端口
uint16_t serverPort = 8080;
string serverIp = "8.156.77.78";

int main()
{
    // 初始化
    WSAData wsd; 

    //启动Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0) { //进行WinSocket的初始化, windows 初始化socket网络库，申请2，2的版本，windows socket编程必须先初始化。
        cout << "WSAStartup Error = " << WSAGetLastError() << endl;
        return 0;
    }
    else {
        cout << "WSAStartup Success" << endl;
    }

    // AF_INET 协议族:决定了要用ipv4地址（32位的）与端口号（16位的）的组合
    // SOCK_DGRAM --  UDP类型，不保证数据接收的顺序，非可靠连接；
    SOCKET csock = socket(AF_INET, SOCK_DGRAM, 0); // 创建套接字
    if (csock == SOCKET_ERROR)
    {
        cout << "socket Error = " << WSAGetLastError() << endl;
        return 1;
    }

    // 设置目标服务器的IP+Port
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server)); // 先清空server变量
    server.sin_family = AF_INET;
    server.sin_port = htons(serverPort);
    server.sin_addr.s_addr = inet_addr(serverIp.c_str());

    // 开始通信
#define NUM 1024
    string line;
    char inbuffer[NUM];
    while (true)
    {
        // 发送数据
        cout << "Please Enter# ";
        getline(cin, line); // 从cin中获取, 然后放到line中
        int n = sendto(csock, line.c_str(), line.size(), 0, (struct sockaddr*)&server, sizeof(server));
        if (n < 0) 
        {
            cerr << "sendto error..." << endl;
            break; // 发送失败, 直接退出
        }

        struct sockaddr_in peer;
        int peerlen = sizeof(peer);
        // 接收数据
        inbuffer[0] = 0; // C语言风格的清空
        recvfrom(csock, inbuffer, sizeof(inbuffer - 1), 0, (struct sockaddr*)&peer, &peerlen);
        if (n > 0) // 读取成功
        {
            inbuffer[n] = 0;
            cout << "Server return msg# " << inbuffer << endl;
        }
        else break;
    }

    closesocket(csock);
    WSACleanup(); // 释放资源
	return 0;
}