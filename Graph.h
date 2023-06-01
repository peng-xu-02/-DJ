#pragma once
#include<string.h>
#pragma once
typedef int Status;
typedef  char Vertxetype;

typedef struct Graph {
	int vNumber;    //顶点数；
	char vertex[20];   //顶点值；
	int edg[20][20];    //顶点相连的边权值；

}Graph;

typedef struct ArcNode { //边节点信息；
	int adjvex;
	int weight;
	struct ArcNode* nextarc;
}ArcNode;

typedef struct vNode {
	char data;//顶点信息；
	ArcNode* firstarc;
}VNode, Adjlist[20];

typedef struct ALGraph {  //图的邻接表；
	Adjlist vertices;
	int vexnum, arcnum;
}ALGraph;

typedef struct route_node {               //路由节点；
	char destinnation;    //目的节点；
	int distance;         //最短开销；
	char out;              //经过节点；
	char pass[10];
	int num; //路径的元素个数；

}route_node;

typedef struct route_collect {
	char route_name;
	route_node items[20];
}route_collect;

typedef struct route_table {
	char route_name;          //路由表的名字；
	route_node items[20];     //路由表的项目；
	route_collect collect[20];//保留交换信息；
}route_table;





