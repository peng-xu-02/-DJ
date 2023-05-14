#define _WINSOCK_DEPRECATED_NO_WARNINGS 

// ����˴���
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include "ServerTCP.h"
#pragma comment (lib, "ws2_32.lib")

#define PORT 1234 // �������˿�
#define ROWS 3 // ��ά���������
#define COLS 3 // ��ά���������
extern void init_route_table(Graph* G,route_table *R);
extern void copy_table(Graph* G, route_table* old, route_table* update);
extern void Distance_vector_routing(Graph* G,route_table *R,route_table *update);
extern void print_route_table(Graph* G,route_table *R,vector<char> arr[][10]);



int ServerTCP(Graph *G,route_table *R, route_table* update, vector<char> arr[][10]) {
    // ��ʼ��socket��
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }



    // ����һ��socket
    SOCKET server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock == INVALID_SOCKET) {
        printf("socket failed: %d\n", WSAGetLastError());
        WSACleanup();
        exit(EXIT_FAILURE);
    }


    /*��������ʹ����socket����������һ��socket��������Ƿ񴴽��ɹ���socket��һ��ͨ�ŵĶ˵㣬���������ڲ�ͬ�Ľ��̻����֮�䷢�ͺͽ������ݡ�
    socket��������������һ���ǵ�ַ�壬AF_INET��ʾʹ��IPv4Э�飬�ڶ��������ͣ�SOCK_STREAM��ʾʹ�ÿɿ����ֽ������񣬵�������Э�飬0��ʾʹ��Ĭ�ϵ�Э�飬��TCPЭ�顣

    socket��������һ��SOCKET���͵�ֵ����������ɹ�������һ����Ч��socket���������������ʧ�ܣ�����INVALID_SOCKET��
    ����������һ��if������ж�socket�����ķ���ֵ�Ƿ����INVALID_SOCKET������ǣ���˵��socket����ʧ���ˣ�Ȼ��ʹ�ӡ��������Ϣ
    ��WSAGetLastError�������Է�����������Ĵ�����룬Ȼ��͵���WSACleanup����������socket�⣬�����˳�����
    WSACleanup���������ͷ�socket��ռ�õ���Դ������Windows��ʹ��socket��̱����������һ����*/


    // ���÷�������ַ�Ͷ˿�
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // �������ַ


    /*��������ʹ����һ���ṹ��sockaddr_in����ʾ�������ĵ�ַ�Ͷ˿ڣ�����ṹ�����ĸ���Ա��
    sin_family��ʾ��ַ�壬sin_port��ʾ�˿ںţ�sin_addr��ʾIP��ַ��sin_zero��һ������ֽڣ����ڶ��롣
    ����ʹ��memset������������ṹ��������ֽڶ���ʼ��Ϊ0��Ȼ������е�������Ա��ֵ��
    sin_family��ֵΪAF_INET����ʾʹ��IPv4Э�飬sin_port��ֵΪhtons(PORT)����ʾʹ������֮ǰ����ĺ�PORT��Ϊ�˿ںţ�
    htons�������Խ������ֽ���ת��Ϊ�����ֽ�����Ϊ��ͬ�Ļ��������в�ͬ���ֽ�������������ͨ��ʱ��Ҫͳһ�ֽ���
    sin_addr.s_addr��ֵΪINADDR_ANY����ʾ�������ַ��ΪIP��ַ��*/

    // ��socket����������ַ�Ͷ˿�
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("bind failed: %d\n", WSAGetLastError());
        closesocket(server_sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }


    /*��������ʹ����bind��������socket����������ַ�Ͷ˿ڣ�������Ƿ�󶨳ɹ���
    bind������������������һ��������֮ǰ������socket���������ڶ�����һ��ָ��sockaddr�ṹ���ָ�룬��ʾҪ�󶨵ķ������ĵ�ַ�Ͷ˿ڣ�������������ṹ��Ĵ�С��
    ע������������Ҫ��sockaddr_in�ṹ��ǿ��ת��Ϊsockaddr�ṹ�壬��Ϊbind����ֻ����sockaddr���͵Ĳ�����
    bind��������һ������ֵ�����Ϊ0����ʾ�󶨳ɹ��������Ϊ0����ʾ��ʧ�ܡ�
    ����������һ��if������ж�bind�����ķ���ֵ�Ƿ����SOCKET_ERROR������ǣ���˵����ʧ���ˣ�Ȼ��ʹ�ӡ��������Ϣ
    ��WSAGetLastError�������Է�����������Ĵ�����룬Ȼ��͵���closesocket�������ر�socket��WSACleanup����������socket�⣬�����˳�����
    closesocket�������Թر�һ��socket���������ͷ���ռ�õ���Դ��*/

    // ������������
    if (listen(server_sock, 3) == SOCKET_ERROR) {
        printf("listen failed: %d\n", WSAGetLastError());
        closesocket(server_sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on port %d\n", PORT);

    while (1) {
        // ����һ����������
        struct sockaddr_in client_addr;
        int client_len = sizeof(client_addr);
        SOCKET client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_len);
        if (client_sock == INVALID_SOCKET) {
            printf("accept failed: %d\n", WSAGetLastError());
            closesocket(server_sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        printf("Accepted a connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /*��������ʹ����һ��whileѭ�������ϵؽ����������󣬲�����ÿ�����ӡ�
        ��ѭ�����У�����ʹ����accept����������һ���������󣬲�����Ƿ���ܳɹ���
        accept������������������һ��������֮ǰ�������󶨲�������socket���������ڶ�����һ��ָ��sockaddr�ṹ���ָ�룬�����洢���ܵ��Ŀͻ��˵ĵ�ַ�Ͷ˿ڣ�
        ��������һ��ָ������ֵ��ָ�룬�����洢����ṹ��Ĵ�С��
        accept��������һ��SOCKET���͵�ֵ����ʾ�½�������ͻ���ͨ�ŵ�socket�����������ΪINVALID_SOCKET����ʾ����ʧ�ܡ�
        ����������һ��if������ж�accept�����ķ���ֵ�Ƿ����INVALID_SOCKET������ǣ���˵������ʧ���ˣ�Ȼ��ʹ�ӡ��������Ϣ��
        WSAGetLastError�������Է�����������Ĵ�����룬Ȼ��͵���closesocket�������رշ�����socket��WSACleanup����������socket�⣬�����˳�����
        closesocket�������Թر�һ��socket���������ͷ���ռ�õ���Դ��

       ������ܳɹ��ˣ����Ǿʹ�ӡ��һ����Ϣ����ʾ�Ѿ�������һ�����Կͻ��˵����ӣ�����ʾ�ͻ��˵�IP��ַ�Ͷ˿ںš�
       inet_ntoa�������Խ��޷��ų����͵���ֵ��ʽ��IP��ַת��Ϊ���ʮ���Ƶ��ַ�����ʽ��IP��ַ��ntohs�������Խ������ֽ���ת��Ϊ�����ֽ���*/

       // ���տͻ��˷��͵Ķ�ά����
        int recv_array[ROWS][COLS];
        if (recv(client_sock, (char*)recv_array, sizeof(recv_array), 0) == SOCKET_ERROR) {
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(client_sock);
            closesocket(server_sock);
            WSACleanup();
            exit(EXIT_FAILURE);
        }
        printf("Received array from client.\n");

        // ��ӡ���յ��Ķ�ά����
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
        Distance_vector_routing(G,R,update);
        print_route_table(G,R,arr);

        route_table table[20];       //��������Ҫ���͵�·�ɱ���Ϣ��
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
        // ���ض�ά������ͻ���
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
           


        // �رտͻ���socket
        closesocket(client_sock);
    }

    // �رշ�����socket
    closesocket(server_sock);
    WSACleanup();
    return 0;
}
