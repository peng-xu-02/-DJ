#include<stdio.h>
#include<stdlib.h>
#include"Graph.h"
#include<graphics.h>
#include<time.h>
#include<vector>
#include"ServerTCP.h"
using namespace std;
#define WIN_x 600
#define WIN_y 600

vector<char> arr[10][10]; //���·����Ϣ;






route_table table[20];           //��·�ɱ�
route_table temp_table[20];     //��·�ɱ�

void readfile(Graph* G)      //���ļ��ж�ȡ���ݣ�
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



void init_route_table(Graph* G)      //��ʼ��·�ɱ�
{
	for (int i = 0; i < G->vNumber; i++) {
		table[i].route_name = G->vertex[i];
		for (int j = 0; j < G->vNumber; j++) {
				
					table[i].items[j].destinnation = G->vertex[j];
					table[i].items[j].distance = G->edg[i][j];
					table[i].items[j].out = G->vertex[j];
				
				
			
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



void Distance_vector_routing(Graph* G) {
	int count = 0;        //���¼���
	bool is_change = true;        //·�ɱ��Ƿ���£�


	//��û�ȶ����ͽ���һ�ָ���
	while (is_change)
	{    
		printf(" ");
		for (int i = 0; i < G->vNumber; i++) {
			printf(" %c", table[i].route_name);
		}
		printf("\n");
			for (int i = 0; i < G->vNumber; i++) {
				printf("%c ", table[i].route_name);
				int k = 0;
				for (int j = 0; j < G->vNumber; j++) {
					printf("%d ", table[i].items[j].distance);
				}
				printf("\n");
			}
			printf("\n");

		count++;
		is_change = false;
		//�Ȱ��ϴε�·�ɱ�ȫ����������
		for (int c = 0; c < G->vNumber; c++) {
			copy_table(G, &table[c], &temp_table[c]);
		}


		//����temp_table�����θ���ÿ����� m ��·�ɱ����º����table
		for (int m = 0; m < G->vNumber; m++) {

			//��������·�ɱ�����ÿһ������ q 
			for (int q = 0; q < G->vNumber; q++) {
				
			
				int curdis = temp_table[m].items[q].distance; //��¼m->q��С����

				if (m == q) { 
					table[m].items[q].distance = 0;

				}
				else {
				   if(curdis!=0){
					   //������m���ڵĽ��
					   for (int u = 0; u < G->vNumber; u++) {


						   if (G->edg[m][u] != 0 && temp_table[u].items[q].distance != 0) {


							   if (G->edg[m][u] + temp_table[u].items[q].distance < curdis) {

								   is_change = true;  //�޸ı��
								  
								   curdis = G->edg[m][u] + temp_table[u].items[q].distance;
								   table[m].items[q].out = G->vertex[u];
								   table[m].items[q].distance = curdis;




							   }

						   }
					   }
					   

  

				   }
						
				   else {                 //�㷨�д�
							//������m���ڵĽ��

					   int min;
							for (int u = 0; u < G->vNumber; u++) {

								if (G->edg[m][u] != 0 && temp_table[u].items[q].distance != 0 /*table[u].items[q].distance!=0*/) {

									is_change = true;

									int curdis = G->edg[m][u] + temp_table[u].items[q].distance;
									table[m].items[q].destinnation = G->vertex[q]; //���Ŀ�Ľ��
									table[m].items[q].out = G->vertex[u];
									table[m].items[q].distance = curdis;//��̾����޸�
								}
							}
							
						}
				}

			}


		}
		
	}
	printf("���´���Ϊ��%d\n", count);
}

void print_route_table(Graph *G) {

	for (int i = 0; i < G->vNumber; i++) {
		printf("%c��·�ɱ�\n", table[i].route_name);
		
		printf("Ŀ�Ľڵ�       ���ڽڵ�        ���·��         ����    \n");
		for (int j = 0; j < G->vNumber; j++) {
			printf("%c\t\t", table[i].items[j].destinnation);
			printf("%c\t\t", table[i].items[j].out);

			arr[i][j].push_back(table[i].items[j].out);
			if (i != j) {
				for (int k = 0; k < G->vNumber; k++) {
					if (table[i].items[j].out == G->vertex[k] && j != k) {
						arr[i][j].push_back(table[k].items[j].out);
						table[i].items[j].out = table[k].items[j].out;
						k = 0;
						if (table[i].items[j].out == table[i].items[j].destinnation)
							break;
					}
				}


			}
			int m = 0;
			vector<char>::iterator it = arr[i][j].begin();
			for (; it != arr[i][j].end(); it++, m++) {
				table[i].items[j].pass[m] = *it;
			}
			for (int m = 0; m < arr[i][j].size(); m++) {
				printf("%c", table[i].items[j].pass[m]);
			}
			table[i].items[j].num = arr[i][j].size();
			


			printf("\t\t %d\n", table[i].items[j].distance);
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

void addtex(Graph& G) {
	int n;
	printf("������Ҫ��ӵĽڵ������\n");
	scanf_s("%d", &n);
	G.vNumber += n;
	printf("������Ҫ��ӵĽڵ㣺\n");
	for (int i = 0; i < n; i++) {
		scanf("%c", &G.vertex[G.vNumber - n + i]);
	}
	char c = getchar();
	printf("������Ҫ��ӵ�·������\n");
	int m;
	scanf_s("%d", &m);
	char c1 = getchar();
	int i, j;
	for (int k = 0; k < m; k++) {
		printf("������Ҫ��д��������������ݺ��������ߵ�Ȩֵ(�ܱ���Ϊ��%d)��\n", m);
		char v1, v2;
		int w;
		scanf("%c%c%d", &v1, &v2, &w);
		i = LocateVex(G, v1);
		j = LocateVex(G, v2);
		G.edg[i][j] = w;
		G.edg[j][i] = G.edg[i][j];
		char c2 = getchar();
	}
	
	printf("%d", G.vNumber);

}

int r[3][4] = {
	{30,20,130,60},
	{170,20,280,60},
	{300,20,360,60}
};
int button_judge(int x, int y)
{
	if (x > r[0][0] && x<r[0][2] && y>r[0][1] && y < r[0][3])  return 1;
	if (x > r[1][0] && x<r[1][2] && y>r[1][1] && y < r[1][3])  return 2;
	if (x > r[2][0] && x<r[2][2] && y>r[2][1] && y < r[2][3])  return 3;
	return 0;
}
void meun()
{
	initgraph(WIN_x, WIN_y, 1);
	int event = 0;
	//outtextxy(200, 200, L"helloworld");
	for (int i = 0; i < 256; i += 5) {
		setbkcolor(RGB(i, i, i));
		cleardevice();
		Sleep(20);
	}

	RECT R1 = { r[0][0],r[0][1],r[0][2],r[0][3] };
	RECT R2 = { r[1][0],r[1][1],r[1][2],r[1][3] };
	RECT R3 = { r[2][0],r[2][1],r[2][2],r[2][3] };

	LOGFONT f;
	gettextstyle(&f);
	_tcscpy(f.lfFaceName, _T("����"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(BLACK);
	drawtext("��������", &R1, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext("�ļ���ȡ", &R2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	drawtext("�˳�", &R3, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setlinecolor(BLACK);
	rectangle(r[0][0], r[0][1], r[0][2], r[0][3]);
	rectangle(r[1][0], r[1][1], r[1][2], r[1][3]);
	rectangle(r[2][0], r[2][1], r[2][2], r[2][3]);

}

void next()
{
	char c;
	c = getchar();
	system("cls");
	exit(0);
}


int main()
{
	//meun();
	//Graph G;
	//Graph M;
	//int event = 0;
	//MOUSEMSG m;
	//setrop2(R2_NOTXORPEN);
	//while (true) {
	//	m = GetMouseMsg();
	//	switch (m.uMsg) {
	//	case WM_MOUSEMOVE:
	//		setrop2(R2_XORPEN);
	//		setlinecolor(LIGHTCYAN);
	//		setlinestyle(PS_SOLID, 3);
	//		setfillcolor(WHITE);
	//		if (button_judge(m.x, m.y) != 0) {
	//			if (event != button_judge(m.x, m.y)) {
	//				event = button_judge(m.x, m.y);
	//				fillrectangle(r[event - 1][0], r[event - 1][1], r[event - 1][2], r[event - 1][3]);
	//			}
	//		}
	//		else {
	//			if (event != 0) {
	//				fillrectangle(r[event - 1][0], r[event - 1][1], r[event - 1][2], r[event - 1][3]);
	//				event = 0;
	//			}
	//		}
	//		break;
	//	case WM_LBUTTONDOWN:
	//		setrop2(R2_XORPEN);
	//		for (int i = 0; i < 10; i++) {
	//			setlinecolor(RGB(25 * i, 25 * i, 25 * i));
	//			circle(m.x, m.y, 2 * i);
	//			Sleep(50);
	//			circle(m.x, m.y, 2 * i);
	//		}
	//		switch (button_judge(m.x, m.y)) {
	//		case 1:
	//			closegraph();
	//			CreatUDN(G);
	//			init_route_table(&G);  //��ʼ������·�ɱ�


	//			Distance_vector_routing(&G);   //ʹ���㷨


	//			print_route_table(&G);
	//			event = 0;
	//			next();
	//			break;


			//case 2:
			//	//closegraph();
			//	readfile(&M);
			//	init_route_table(&M);  //��ʼ������·�ɱ�
			//	Distance_vector_routing(&M);   //ʹ���㷨
			//	print_route_table(&M);
			//	//print_route_way(&M);
			//	event = 0;
			//	next();
			//	break;


	//		case 3:
	//			closegraph();
	//			exit(0);
	//		default:
	//			FlushMouseMsgBuffer();
	//			break;
	//		}

	//	}

	//}



	//Graph G;
	//CreatUDN(G);
	//init_route_table(&G);  //��ʼ������·�ɱ�


	//Distance_vector_routing(&G);   //ʹ���㷨��
	//print_route_table(&G);
	//addtex(G);
	//init_route_table(&G);  //��ʼ������·�ɱ�
	//Distance_vector_routing(&G);
	//print_route_table(&G);
	Graph G;
	ServerTCP(&G);

	//Graph M;
	//readfile(&M);
	//init_route_table(&M);  //��ʼ������·�ɱ�
	//Distance_vector_routing(&M);   //ʹ���㷨
	//print_route_table(&M);


	return 0;
}

