#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"
#include<graphics.h>
#include<time.h>
#include<vector>
#include"ServerTCP.h"
#include<string.h>
using namespace std;
#define WIN_x 600
#define WIN_y 600

//vector<char> arr[10][10]; //���·����Ϣ;

void InitGraph(Graph* G) {       //��ʼ��ͼ
	// ���ö������ͱ���Ϊ0
	G->vNumber = 0;
	// ���ö���ֵΪĬ��ֵ
	for (int i = 0; i < 20; i++) {
		G->vertex[i] = '\0';
	}
	// ���ñ�ȨֵΪĬ��ֵ
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			G->edg[i][j] = 0;
		}
	}
}


void readfile(Graph *G)      //���ļ��ж�ȡ���ݣ�
{
	FILE * file1,*file2,*file3;

	file1 = fopen("vNumber.txt","r");
	if (file1 == NULL) {
		printf("���ļ�ʧ�ܣ�\n");
	}
	else {
		fscanf(file1, "%d", &G->vNumber);
	}
	fclose(file1);

	file2 = fopen("vertex.txt", "r");
	if (file2 == NULL) {
		printf("���ļ�ʧ�ܣ�\n");
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
		printf("���ļ�ʧ�ܣ�\n");
	}
	else {
		while (fgets(s, 20, file3)) {      //���ж�ȡ�ļ����ݣ�
			char v1 = s[0];
			char v2 = s[1];
		
			G->edg[v1 - 65][v2 - 65] =s[2]-48;
			
			G->edg[v2 - 65][v1 - 65] = G->edg[v1 - 65][v2 - 65];

		}
	}

	fclose(file3);






}



void init_route_table(Graph *G,route_table *R)      //��ʼ��·�ɱ�
{
	for (int i = 0; i < G->vNumber; i++) {
		R[i].route_name = G->vertex[i];
		for (int j = 0; j < G->vNumber; j++) {
				
			R[i].items[j].destinnation = G->vertex[j];
			R[i].items[j].distance = G->edg[i][j];
			R[i].items[j].out = G->vertex[j];
				
				
			
		}
	}
}
void copy_table(Graph* G, route_table *old, route_table *update)      //����·�ɱ�
{
	       update->route_name = old->route_name;
		for (int j = 0; j < G->vNumber; j++) {
			update->items[j].destinnation = old->items[j].destinnation;
			update->items[j].distance = old->items[j].distance;
			update->items[j].out = old->items[j].out;
		
		}
	}



void Distance_vector_routing(Graph* G,route_table *R,route_table *update) {
	int count = 0;        //���¼���
	bool is_change = true;        //·�ɱ��Ƿ���£�


	//��û�ȶ����ͽ���һ�ָ���
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
		//�Ȱ��ϴε�·�ɱ�ȫ����������
		for (int c = 0; c < G->vNumber; c++) {
			copy_table(G, &R[c], &update[c]);
		}


		//����temp_table�����θ���ÿ����� m ��·�ɱ����º����table
		for (int m = 0; m < G->vNumber; m++) {

			//��������·�ɱ�����ÿһ������ q 
			for (int q = 0; q < G->vNumber; q++) {
				
			
				int curdis = update[m].items[q].distance; //��¼m->q��С����

				if (m == q) { 
					R[m].items[q].distance = 0;

				}
				else {
				   if(curdis!=0){
					   //������m���ڵĽ��
					   for (int u = 0; u < G->vNumber; u++) {


						   if (G->edg[m][u] != 0 && update[u].items[q].distance != 0) {


							   if (G->edg[m][u] + update[u].items[q].distance < curdis) {

								   is_change = true;  //�޸ı��
								  
								   curdis = G->edg[m][u] + update[u].items[q].distance;
								   R[m].items[q].out = G->vertex[u];
								   R[m].items[q].distance = curdis;




							   }

						   }
					   }
					   

  

				   }
						
				   else {                 //�㷨�д�
							//������m���ڵĽ��

							for (int u = 0; u < G->vNumber; u++) {

								if (G->edg[m][u] != 0 && update[u].items[q].distance != 0) {

									is_change = true;

									int curdis = G->edg[m][u] + update[u].items[q].distance;
									R[m].items[q].destinnation = G->vertex[q]; //���Ŀ�Ľ��
									R[m].items[q].out = G->vertex[u];
									R[m].items[q].distance = curdis;//��̾����޸�
								}
							}
							
						}
				}

			}


		}
		
	}
	printf("���´���Ϊ��%d\n", count);
}

void print_route_table(Graph *G,route_table *R,vector<char> arr[][10]) {

	for (int i = 0; i < G->vNumber; i++) {
		printf("%c��·�ɱ�\n", R[i].route_name);
		
		printf("Ŀ�Ľڵ�       ���ڽڵ�        ���·��         ����    \n");
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


	



Status LocateVex(Graph G, char u)   //Ѱ�ҽڵ��±ꣻ
{
	int i;
	for (i = 0; i < G.vNumber; i++) {
		if (u == G.vertex[i]) {
			return i;
		}
	}
	return -1;
}
void CreatUDN(Graph& G)  //���������� 
{
	int i, j,n;
	printf("�������������ܶ��������ܱ�����\n");
	scanf("%d%d", &G.vNumber, &n);
	printf("������%d����������ݣ�\n", G.vNumber);
	char c, c1, c2;
	c = getchar();
	for (i = 0; i < G.vNumber; i++) {
		scanf("%c", &G.vertex[i]);
	}
	c1 = getchar();
	for (i = 0; i < G.vNumber; ++i)      //��ʼ���ڽӾ��� 
		for (j = 0; j < G.vNumber; ++j)
			G.edg[i][j] = 0;


	for (int k = 0; k < n; k++) {
		printf("������Ҫ��д��������������ݺ��������ߵ�Ȩֵ(�ܱ���Ϊ��%d)��\n", n);
		char v1, v2;
		int w;
		scanf("%c%c%d", &v1, &v2, &w);
		i = LocateVex(G, v1);
		j = LocateVex(G, v2);
		G.edg[i][j] = w;
		G.edg[j][i] = G.edg[i][j];
		c2 = getchar();
	}
}


void InitRouteTable(route_table* R) {
	// ����·�ɱ������ΪĬ��ֵ
	R->route_name = '\0';
	// ����·�ɱ��ÿ����ĿΪĬ��ֵ
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


int main()
{
	printf("---------------------\n");
	printf("     1.�ļ���ȡ\n");
	printf("     2.��������\n");
	printf("     3.�ͻ�������\n");
	printf("     0.�˳�\n");
	printf("---------------------\n");
	Graph G;
	while (true) {
		memset(&G, 0, sizeof(G));
		InitGraph(&G);
		route_table R[20];
		route_table update[20];
		InitRouteTable(R);
		InitRouteTable(update);
		vector<char> arr[10][10];
		int num;
		printf("���������\n");
		scanf_s("%d", &num);
		switch (num) {
		case 1:
			readfile(&G);
			init_route_table(&G,R);  //��ʼ������·�ɱ�
			Distance_vector_routing(&G,R,update);   //ʹ���㷨
			print_route_table(&G,R,arr);
			break;

		case 2:
			CreatUDN(G);
			init_route_table(&G,R);  //��ʼ������·�ɱ�
			Distance_vector_routing(&G,R,update);   //ʹ���㷨��
			print_route_table(&G,R,arr);
			break;

		case 3:
			ServerTCP(&G,R,update,arr);
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

