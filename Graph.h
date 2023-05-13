#include<string.h>
#pragma once
typedef struct Graph {
	int vNumber;    //顶点数；
	char vertex[20];   //顶点值；
	int edg[20][20];    //顶点相连的边权值；

}Graph;

typedef struct route_node {               //路由节点；
	char destinnation;    //目的节点；
	int distance;         //最短开销；
	char out;              //经过节点；
	char pass[10];
	int num; //路径的元素个数；
}route_node;

typedef struct route_table {
	char route_name;          //路由表的名字；
	route_node items[20];     //路由表的项目；
}route_table;

typedef int Status;


