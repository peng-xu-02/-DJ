#include<string.h>
#pragma once
typedef struct Graph {
	int vNumber;    //��������
	char vertex[20];   //����ֵ��
	int edg[20][20];    //���������ı�Ȩֵ��

}Graph;

typedef struct route_node {               //·�ɽڵ㣻
	char destinnation;    //Ŀ�Ľڵ㣻
	int distance;         //��̿�����
	char out;              //�����ڵ㣻
	char pass[10];
	int num; //·����Ԫ�ظ�����
}route_node;

typedef struct route_table {
	char route_name;          //·�ɱ�����֣�
	route_node items[20];     //·�ɱ����Ŀ��
}route_table;

typedef int Status;


