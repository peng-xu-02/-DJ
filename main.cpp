#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"
#include<graphics.h>
#include<time.h>
#include<vector>
#include<string.h>
#include"ServerTCP.h"
using namespace std;


void readfile(Graph* G)      //从文件中读取内容；
{
	FILE* file1, * file2, * file3;

	file1 = fopen("vNumber.txt", "r");
	if (file1 == NULL) {
		printf("打开文件失败！\n");
	}
	else {
		fscanf(file1, "%d", &G->vNumber);
	}
	fclose(file1);

	file2 = fopen("vertex.txt", "r");
	if (file2 == NULL) {
		printf("打开文件失败！\n");
	}
	else {
		for (int i = 0; i < G->vNumber; i++) {
			fscanf(file2, "%c", &G->vertex[i]);
		}


	}
	fclose(file2);

	char s[20];
	file3 = fopen("edge.txt", "r");
	if (file3 == NULL) {
		printf("打开文件失败！\n");
	}
	else {
		while (fgets(s, 20, file3)) {      //按行读取文件类容；
			char v1 = s[0];
			char v2 = s[1];

			G->edg[v1 - 65][v2 - 65] = s[2] - 48;

			G->edg[v2 - 65][v1 - 65] = G->edg[v1 - 65][v2 - 65];

		}
	}

	fclose(file3);






}

void init_route_table(Graph* G, route_table* R)      //初始化路由表；
{
	for (int i = 0; i < G->vNumber; i++) {
		R[i].route_name = G->vertex[i];
		memset(R[i].collect, 0, sizeof(R[i].collect));
		for (int j = 0; j < G->vNumber; j++) {
			R[i].items[j].destinnation = 0;
			R[i].items[j].distance = 0;
			R[i].items[j].out = 0;
			if (G->edg[i][j] != 0) {
				R[i].items[j].destinnation = G->vertex[j];
				R[i].items[j].distance = G->edg[i][j];
				R[i].items[j].out = G->vertex[j];
			}
			if (i == j) {
				R[i].items[j].destinnation = G->vertex[j];
				R[i].items[j].out = G->vertex[j];
			}





		}
	}
}

void compine(ALGraph* G, Graph* M) {
	M->vNumber = G->vexnum;
	for (int i = 0; i < M->vNumber; i++) {
		M->vertex[i] = G->vertices[i].data;
		for (int j = 0; j < M->vNumber; j++) {
			M->edg[i][j] = 0;
		}
		ArcNode* p = G->vertices[i].firstarc;
		while (p != NULL) {
			M->edg[i][p->adjvex] = p->weight;
			p = p->nextarc;
		}

	}
}

void copy_table(Graph* G, route_table* old, route_table* update)      //拷贝路由表；
{
	update->route_name = old->route_name;
	for (int j = 0; j < G->vNumber; j++) {
		update->items[j].destinnation = old->items[j].destinnation;
		update->items[j].distance = old->items[j].distance;
		update->items[j].out = old->items[j].out;

	}
}

void Distance_vector_routing1(Graph* G, route_table* R, route_table* update) {
	int count = 0;        //更新计数
	bool is_change = true;        //路由表是否更新；


	//还没稳定，就进行一轮更新
	while (is_change)
	{
		printf(" ");
		for (int i = 0; i < G->vNumber; i++) {
			printf(" %c", R[i].route_name);
		}
		printf("\n");
		for (int i = 0; i < G->vNumber; i++) {
			printf("%c ", R[i].route_name);
			for (int j = 0; j < G->vNumber; j++) {
				printf("%d ", R[i].items[j].distance);
			}
			printf("\n");
		}
		printf("\n");

		count++;
		is_change = false;
		//先把上次的路由表全部保存起来
		for (int c = 0; c < G->vNumber; c++) {
			copy_table(G, &R[c], &update[c]);
		}


		//依据temp_table，依次更新每个结点 m 的路由表，更新后的至table
		for (int m = 0; m < G->vNumber; m++) {

			//遍历它的路由表，更新每一个表项 q 
			for (int q = 0; q < G->vNumber; q++) {


				int curdis = update[m].items[q].distance; //记录m->q最小距离

				if (m == q) {
					R[m].items[q].distance = 0;

				}
				else {
					if (curdis != 0) {


						R[m].collect[q].route_name = R[q].route_name;
						for (int k = 0; k < G->vNumber; k++) {
							R[m].collect[q].items[k] = R[q].items[k];
						}
						for (int u = 0; u < G->vNumber; u++) {
							if (G->edg[m][u] != 0 && R[m].collect[u].items[q].distance != 0) {
								if (G->edg[m][u] + R[m].collect[u].items[q].distance < curdis) {
									is_change = true;
									curdis = G->edg[m][u] + R[m].collect[u].items[q].distance;
									R[m].items[q].out = G->vertex[u];
									R[m].items[q].distance = curdis;
								}
							}
						}




					}


					else {
						//遍历与m相邻的结点

						for (int u = 0; u < G->vNumber; u++) {

							if (G->edg[m][u] != 0 && R[m].collect[u].items[q].distance != 0) {

								is_change = true;

								int curdis = G->edg[m][u] + R[m].collect[u].items[q].distance;
								R[m].items[q].destinnation = G->vertex[q]; //添加目的结点
								R[m].items[q].out = G->vertex[u];
								R[m].items[q].distance = curdis;//最短距离修改
							}
						}

					}


				}

			}


		}

	}
	printf("更新次数为：%d\n", count);
}

void print_route_table(Graph* G, route_table* R, vector<char> arr[][10]) {

	for (int i = 0; i < G->vNumber; i++) {
		printf("%c的路由表：\n", R[i].route_name);

		printf("目的节点       相邻节点        最短路径         开销    \n");
		for (int j = 0; j < G->vNumber; j++) {
			printf("%c\t\t", R[i].items[j].destinnation);
			printf("%c\t\t", R[i].items[j].out);

			arr[i][j].push_back(R[i].items[j].out);
			if (i != j) {
				for (int k = 0; k < G->vNumber; k++) {
					if (R[i].items[j].out == G->vertex[k] && j != k) {
						arr[i][j].push_back(R[k].items[j].out);
						R[i].items[j].out = R[k].items[j].out;
						k = 0;
						if (R[i].items[j].out == R[i].items[j].destinnation)
							break;
					}
				}


			}
			int m = 0;
			vector<char>::iterator it = arr[i][j].begin();
			for (; it != arr[i][j].end(); it++, m++) {
				R[i].items[j].pass[m] = *it;
			}
			for (int m = 0; m < arr[i][j].size(); m++) {
				printf("%c", R[i].items[j].pass[m]);
			}
			R[i].items[j].num = arr[i][j].size();



			printf("\t\t %d\n", R[i].items[j].distance);
		}
		printf("\n");
	}
}

Status LocateVex1(ALGraph G, char u)   //寻找节点下标；
{
	int i;
	for (i = 0; i < G.vexnum; i++) {
		if (u == G.vertices[i].data) {
			return i;
		}
	}
	return -1;
}

void CreatUDN1(ALGraph& G) {              //创建邻接表无向网；
	printf("请输入节点个数：\n");
	scanf_s("%d", &G.vexnum);
	printf("请输入边数：\n");
	scanf_s("%d", &G.arcnum);
	char c = getchar();
	printf("请输入%d个节点数据：\n", G.vexnum);
	for (int i = 0; i < G.vexnum; i++) {
		scanf("%c", &G.vertices[i].data);
		G.vertices[i].firstarc = NULL;
	}
	for (int i = 0; i < G.vexnum; i++) {
		printf("%c", G.vertices[i].data);
	}
	char c1 = getchar();

	for (int i = 0; i < G.arcnum; i++) {
		printf("请输入第%d条边的两个边顶点和权值：\n", i + 1);
		char v1, v2;
		int weight;
		scanf("%c%c%d", &v1, &v2, &weight);
		char c2 = getchar();
		int j = LocateVex1(G, v1);
		int k = LocateVex1(G, v2);
		ArcNode* p1 = (ArcNode*)malloc(sizeof(ArcNode));
		p1->adjvex = j;
		p1->weight = weight;
		p1->nextarc = G.vertices[k].firstarc;
		G.vertices[k].firstarc = p1;

		ArcNode* p2 = (ArcNode*)malloc(sizeof(ArcNode));
		p2->adjvex = k;
		p2->weight = p1->weight;
		p2->nextarc = G.vertices[j].firstarc;
		G.vertices[j].firstarc = p2;

	}


}

void InitRouteTable(route_table* R) {
	// 设置路由表的名字为默认值
	R->route_name = '\0';
	// 设置路由表的每个项目为默认值
	for (int i = 0; i < 20; i++) {
		R->items[i].destinnation = '\0';
		R->items[i].distance = 0;
		R->items[i].out = -1;
		for (int j = 0; j < 10; j++) {
			R->items[i].pass[j] = '\0';
		}
		R->items[i].num = 0;
	}
}

void addvtex(ALGraph& G) {     //添加节点；
	int n, m;
	char data;
	printf("请输入要添加的节点个数：\n");
	scanf("%d", &n);
	G.vexnum += n;
	char c = getchar();
	printf("请输入%d个节点数据：\n", n);
	for (int i = 0; i < n; i++) {
		scanf("%c", &data);
		G.vertices[G.vexnum - n + i].data = data;
		G.vertices[G.vexnum - n + i].firstarc = NULL;

	}
	char c1 = getchar();
	printf("请输入要添加的边数：\n");
	scanf("%d", &m);
	G.arcnum += m;
	char c3 = getchar();
	for (int i = 0; i < m; i++) {
		printf("请输入边的两个边顶点和权值：\n");
		char v1, v2;
		int weight;
		scanf("%c%c%d", &v1, &v2, &weight);
		char c2 = getchar();
		int j = LocateVex1(G, v1);
		int k = LocateVex1(G, v2);
		ArcNode* p1 = (ArcNode*)malloc(sizeof(ArcNode));
		p1->adjvex = j;
		p1->weight = weight;
		p1->nextarc = G.vertices[k].firstarc;
		G.vertices[k].firstarc = p1;

		ArcNode* p2 = (ArcNode*)malloc(sizeof(ArcNode));
		p2->adjvex = k;
		p2->weight = p1->weight;
		p2->nextarc = G.vertices[j].firstarc;
		G.vertices[j].firstarc = p2;


	}



}

void display(ALGraph* G) {
	for (int i = 0; i < G->vexnum; i++) {
		printf("%4d %3c ->", i, G->vertices[i].data);
		ArcNode* p = G->vertices[i].firstarc;
		while (p != NULL) {
			printf("数据%c (下标%d) 权值%d-> ", G->vertices[p->adjvex].data, p->adjvex, p->weight);
			p = p->nextarc;

		}
		printf("\n");
	}
}


int main()
{
	printf("---------------------\n");
	printf("     1.文件读取\n");
	printf("     2.键盘输入\n");
	printf("     3.客户端输入\n");
	printf("     0.退出\n");
	printf("---------------------\n");
	Graph G;
	ALGraph M;
	while (true) {
		memset(&G, 0, sizeof(G));
		memset(&M, 0, sizeof(M));
		route_table R[20];
		route_table update[20];
		InitRouteTable(R);
		InitRouteTable(update);
		vector<char> arr[10][10];
		vector<char>arr1[10][10];

		int num;
		printf("请输入命令：\n");
		scanf_s("%d", &num);
		switch (num) {
		case 1:
			readfile(&G);
			init_route_table(&G, R);  //初始化所有路由表；
			Distance_vector_routing1(&G, R, update);   //使用算法
			print_route_table(&G, R, arr);
			break;

		case 2:
			CreatUDN1(M);
			display(&M);
			compine(&M, &G);
			init_route_table(&G, R);  //初始化所有路由表
			Distance_vector_routing1(&G, R, update);   //使用算法；
			print_route_table(&G, R, arr);


			addvtex(M);
			display(&M);
			memset(&G, 0, sizeof(G));
			InitRouteTable(R);
			InitRouteTable(update);
			compine(&M, &G);
			init_route_table(&G, R);  //初始化所有路由表
			Distance_vector_routing1(&G, R, update);   //使用算法；
			print_route_table(&G, R, arr1);



			break;

		case 3:
			ServerTCP(&G, R, update, arr);
			break;

		case 0:

			exit(0);
			break;

		default:
			break;

		}


	}

	return 0;
}

