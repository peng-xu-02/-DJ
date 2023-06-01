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
extern void print_route_table(Graph* G,route_table *R,vector<char> arr[][10]);
extern void Distance_vector_routing1(Graph* G, route_table* R, route_table* update);



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


    /*��������Э�飬0��ʾʹ��Ĭ�ϵ�Э�飬��TCPЭ�顣

    socket��������һ��SOCKET���͵�ֵ����������ɹ�������һ����Ч��socket���������������ʧ�ܣ�����INVALID_SOCKET��
    WSACleanup���������ͷ�socket��ռ�õ���Դ������Windows��ʹ��socket��̱����������һ����*/


    // ���÷�������ַ�Ͷ˿�
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;  //Э��汾��IPV4�汾
    server_addr.sin_port = htons(PORT);//�˿ں�
    server_addr.sin_addr.s_addr = INADDR_ANY; // �������ַ�����Խ���


    /*��������ʹ����һ���ṹ��sockaddr_in����ʾ�������ĵ�ַ�Ͷ˿ڣ�����ṹ�����ĸ���Ա��sin_zero��һ������ֽڣ����ڶ��롣
    htons�������Խ������ֽ���ת��Ϊ�����ֽ�����Ϊ��ͬ�Ļ��������в�ͬ���ֽ�������������ͨ��ʱ��Ҫͳһ�ֽ���
    sin_addr.s_addr��ֵΪINADDR_ANY����ʾ�������ַ��ΪIP��ַ��*/

    // ��socket����������ַ�Ͷ˿�
    if (bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        printf("bind failed: %d\n", WSAGetLastError());
        closesocket(server_sock);//WSACleanup����������socket�⣬�����˳�����closesocket�������Թر�һ��socket���������ͷ���ռ�õ���Դ��
        WSACleanup();        //�ͷ���Դ��
        exit(EXIT_FAILURE);
    }


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
        struct sockaddr_in client_addr; //�����洢���յ��Ŀͻ��˵�IP��ַ�Ͷ˿ڣ�
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
        accept��������һ��SOCKET���͵�ֵ����ʾ�½�������ͻ���ͨ�ŵ�socket�����������ΪINVALID_SOCKET����ʾ����ʧ�ܡ�
        */

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
        Distance_vector_routing1(G,R,update);
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
