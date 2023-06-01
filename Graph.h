#pragma once
#include<string.h>
#pragma once
typedef int Status;
typedef  char Vertxetype;

typedef struct Graph {
	int vNumber;    //��������
	char vertex[20];   //����ֵ��
	int edg[20][20];    //���������ı�Ȩֵ��

}Graph;

typedef struct ArcNode { //�߽ڵ���Ϣ��
	int adjvex;
	int weight;
	struct ArcNode* nextarc;
}ArcNode;

typedef struct vNode {
	char data;//������Ϣ��
	ArcNode* firstarc;
}VNode, Adjlist[20];

typedef struct ALGraph {  //ͼ���ڽӱ�
	Adjlist vertices;
	int vexnum, arcnum;
}ALGraph;

typedef struct route_node {               //·�ɽڵ㣻
	char destinnation;    //Ŀ�Ľڵ㣻
	int distance;         //��̿�����
	char out;              //�����ڵ㣻
	char pass[10];
	int num; //·����Ԫ�ظ�����

}route_node;

typedef struct route_collect {
	char route_name;
	route_node items[20];
}route_collect;

typedef struct route_table {
	char route_name;          //·�ɱ�����֣�
	route_node items[20];     //·�ɱ����Ŀ��
	route_collect collect[20];//����������Ϣ��
}route_table;





