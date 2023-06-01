#define _WINSOCK_DEPRECATED_NO_WARNINGS 

// 服务端代码
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "ServerTCP.h"
#pragma comment (lib, "ws2_32.lib")

#define PORT 1234 // 服务器端口
#define ROWS 3 // 二维数组的行数
#define COLS 3 // 二维数组的列数
extern void init_route_table(Graph* G,route_table *R);
extern void copy_table(Graph* G, route_table* old, route_table* update);
extern void print_route_table(Graph* G,route_table *R,vector<char> arr[][10]);
extern void Distance_vector_routing1(Graph* G, route_table* R, route_table* update);



int ServerTCP(Graph *G,route_table *R, route_table* update, vector<char> arr[][10]) {
    // 初始化socket库
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }



    // 创建一个socket
    SOCKET server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }


    /*第三个是协议，0表示使用默认的协议，即TCP协议。

    socket函数返回一个SOCKET类型的值，如果创建成功，就是一个有效的socket描述符，如果创建失败，就是INVALID_SOCKET。
    WSACleanup函数可以释放socket库占用的资源，这是Windows下使用socket编程必须做的最后一步。*/


    // 设置服务器地址和端口
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;  //协议版本，IPV4版本
    server_addr.sin_port = htons(PORT);//端口号
    server_addr.sin_addr.s_addr = INADDR_ANY; // 绑定任意地址，可以接收


    /*这里我们使用了一个结构体sockaddr_in来表示服务器的地址和端口，这个结构体有四个成员，sin_zero是一个填充字节，用于对齐。
    htons函数可以将主机字节序转换为网络字节序，因为不同的机器可能有不同的字节序，所以在网络通信时需要统一字节序。
    sin_addr.s_addr赋值为INADDR_ANY，表示绑定任意地址作为IP地址。*/

    // 绑定socket到服务器地址和端口
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("bind failed: %d\n", WSAGetLastError());
        closesocket(server_sock);//WSACleanup函数来清理socket库，最后就退出程序。closesocket函数可以关闭一个socket描述符，释放其占用的资源。
        WSACleanup();        //释放资源；
        exit(EXIT_FAILURE);
    }


    // 监听连接请求
    if (listen(server_sock, 3) == SOCKET_ERROR) {
        printf("listen failed: %d\n", WSAGetLastError());
        closesocket(server_sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on port %d\n", PORT);

    while (1) {
        // 接受一个连接请求
        struct sockaddr_in client_addr; //用来存储接收到的客户端的IP地址和端口；
        int client_len = sizeof(client_addr);
        SOCKET client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        if (client_sock == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
            closesocket(server_sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        printf("Accepted a connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /*这里我们使用了一个while循环来不断地接受连接请求，并处理每个连接。
        在循环体中，我们使用了accept函数来接受一个连接请求，并检查是否接受成功。
        accept函数返回一个SOCKET类型的值，表示新建立的与客户端通信的socket描述符，如果为INVALID_SOCKET，表示接受失败。
        */

       // 接收客户端发送的二维数组
        int recv_array[ROWS][COLS];
        if (recv(client_sock, (char*)recv_array, sizeof(recv_array), 0) == SOCKET_ERROR) {
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(client_sock);
            closesocket(server_sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
        printf("Received array from client.\n");

        // 打印接收到的二维数组
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                printf("%d ", recv_array[i][j]);
                
            }
            printf("\n");
        }
        printf("\n");
          G->vNumber = ROWS;
          char seed[5] = { 'A','B','C','D','E'};
          for (int i = 0; i < ROWS; i++) {
              G->vertex[i] = seed[i];
            for (int j = 0; j < COLS; j++) {
                G->edg[i][j] = recv_array[i][j];
            }
        }


        init_route_table(G,R);
        Distance_vector_routing1(G,R,update);
        print_route_table(G,R,arr);

        route_table table[20];       //用来保存要发送的路由表信息；
        for (int i = 0; i < G->vNumber; i++) {
            table[i].route_name = R[i].route_name;
            for (int j = 0; j < G->vNumber; j++) {
                table[i].items[j].destinnation = R[i].items[j].destinnation;
                table[i].items[j].distance = R[i].items[j].distance;
                table[i].items[j].num = R[i].items[j].num;
                for (int k = 0; k < R[i].items[j].num; k++) {
                    table[i].items[j].pass[k] = R[i].items[j].pass[k];
                }
            }
        }
        // 返回二维数组给客户端
        if (send(client_sock, (char*)recv_array, sizeof(recv_array), 0) == SOCKET_ERROR) {
            printf("send failed: %d\n", WSAGetLastError());
            closesocket(client_sock);
            closesocket(server_sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        printf("Sent array back to client.\n");
        if (send(client_sock, (char*)&table, sizeof(table), 0) == SOCKET_ERROR) {
            printf("send failed: %d\n", WSAGetLastError());
            closesocket(client_sock);
            closesocket(server_sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
           


        // 关闭客户端socket
        closesocket(client_sock);
    }

    // 关闭服务器socket
    closesocket(server_sock);
    WSACleanup();
    return 0;
}
