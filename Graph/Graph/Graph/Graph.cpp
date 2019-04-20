/*
координаты вещественые числа

вывод графа в виде списков смежности

а у нас может быть мультиграф???
у нас могут быть петли???

1. Написать функцию считывания вещественных чисел (getDouble (double* t));
2. Создание графа:
- загрузка из файла
- задать вручную
- рандомная генерация


3. процесс работы

вывод возможностей загрузки графа
1. Из файла
	- структура файла???
2. Сгенерировать
	какие входные данные нужны для генерации?
	- (макс) число вершин
	- рёбра????
3. Вручную
	- ручная вставка вершин и ребер в граф


Меню:
0. Выход
1. Добавить вершину
2. Добавить ребро
3. Удалить вершину (а удалять просто ребро не надо???)
4. Разложить на сильно связные компоненты
5. Вывод графа (в виде списка смежности)
6. Сохранить граф в файл
	- ввести имя файла
7. Таймирование разложения (hmmmmmmm)

файл должен быть отвязан от графа
код ошибки определяется через define

*/
#include "pch.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#pragma warning(disable:4996)

#define GRAPH_NULLPTR 1
#define FNAME_NULLPTR 2
#define LOADING_FAILURE 3
#define DUPLICATE_POINT 2
#define NOT_FOUND -1
#define EDGE_DUPLICATE 2
#define UNDEFINED_VERTEX 2
#define VERTEX_NOT_FOUND 3


#define WHITE 0
#define GRAY 1
#define BLACK 2

struct Point {
	double x;
	double y;
	int id;
};

struct Vertex {
	Point* point;
	int color;
	int dTime;
	int fTime;
};

struct Edge {
	int idFirst;
	int idSecond;
};



struct AdjList {
	Vertex* vertex;
	AdjList* next;
};

struct Graph {
	AdjList* adjlist;
	int vertexN;
	int edgeN;
};

const char* loadmsgs[] = { "0. Quit", "1. Load graph", "2. Generate graph", "3. Create graph manually" };
const char* menu[] = { "0. Quit", "1. Add vertex", "2. Add edge", "3. Delete vertex", "4. Strondly connected components", "5. Display graph (as adjacency lists)", "6. Save graph", "7. Timing", "8. Graph properties" };
const char* loaderrs[] = { "Ok", "Graph nullptr", "Filename nullptr", "Failed to load the file" };
const char* createmsgs[] = { "0. Quit", "1. Add vertex", "2. Add edge" };
const char* insertVertexErrs[] = { "Ok", "Graph nullptr", "Duplicate point" };
const char* insertEdgeErrs[] = { "Ok", "Graph nullptr" };
const char* removeVertexMenu[] = { "0. Quit", "1. Enter coordinates", "2. Enter id" };
const char* removeVertexErrs[] = { "Ok", "Graph nullptr", "Undefined vertex", "Vertex not found" };

const int NLoadMsgs = sizeof(loadmsgs) / sizeof(loadmsgs[0]);
const int NMenu = sizeof(menu) / sizeof(menu[0]);
const int NCreate = sizeof(createmsgs) / sizeof(createmsgs[0]);
const int NRemoveVertexMenu = sizeof(removeVertexMenu) / sizeof(removeVertexMenu[0]);

int dialog(const char* msgs[], int N);
int getInt(int *t, int mode = 0);
int dvertexInsert(Graph*);
int dedgeInsert(Graph*);
int dvertexRemove(Graph*);
int decompose(Graph*);
int display(Graph*);
int dsave(Graph*);
int timing(Graph*);
int properties(Graph*);
int dload(Graph*);
int dgenerate(Graph*);
int dcreate(Graph*);
int getCords(Graph*);
int getId(Graph*);
int load(Graph* g, char* fname);
int getDouble(double* t);
int vertexInsert(Graph* g, double x, double y);
int vertexSearch(Graph* g, double x, double y);
int vertexRemove(Graph* g, double* x, double* y, int* id);
int edgeInsert(Graph* g, int fid, int sid);
char* getStr(int mode);
AdjList* edgeSearch(Graph* g, int fid, int sid);


int(*mfptr[])(Graph*) = { NULL, dvertexInsert, dedgeInsert, dvertexRemove, decompose, display, dsave, timing, properties };
int(*lfptr[])(Graph*) = { NULL, dload, dgenerate, dcreate };
int(*rvfptr[])(Graph*) = { NULL, getCords, getId };

int vertexRemove(Graph*)

int getCords(Graph* g) {
	if (!g) return GRAPH_NULLPTR;
	double x, y;
	puts("Enter coordinates:");
	printf("X: --> ");
	getDouble(&x);
	printf("Y: --> ");
	getDouble(&y);
	int rc = vertexRemove(g, &x, &y, NULL);
	printf("%s\n", removeVertexErrs[rc]);
	return 0;
}

int getId(Graph* g) {
	if (!g) return GRAPH_NULLPTR;
	int id;
	printf("Id: --> ");
	getInt(&id);
	int rc = vertexRemove(g, NULL, NULL, &id);
	printf("%s\n", removeVertexErrs[rc]);
	return 0;
}

int dvertexRemove(Graph* g) {
	double x, y;
	int id;
	//реализовать удаление как по координатам, таки по имени вершины
	int m = dialog(removeVertexMenu, NRemoveVertexMenu);
	if (!(rvfptr[m](g))) return 0;
	return 0;
}

AdjList* edgeSearch(Graph*g, int fid, int sid) {
	if (!g) return NULL;
	AdjList* tmp = g->adjlist[fid].next;
	while (tmp) {
		if (tmp->vertex->point->id == sid) return tmp;
		tmp = tmp->next;
	}
	return tmp;
}


int edgeInsert(Graph* g, int fid, int sid) {
	if (!g) return GRAPH_NULLPTR;
	//if (edgeSearch(g, fid, sid) return EDGE_DUPLICATE; если мультиребра вне закона
	AdjList* a = (AdjList*)malloc(sizeof(AdjList));
	a->vertex = g->adjlist[sid].vertex;
	a->next = g->adjlist[fid].next;
	g->adjlist[fid].next = a;
	g->edgeN++;
	return 0;
}

int dedgeInsert(Graph *g) {
	if (!g) return GRAPH_NULLPTR;
	display(g);
	int fid, sid;
	printf("Enter the first vertex id: --> ");
	getInt(&fid, 0);
	printf("Enter the second vertex id: --> ");
	getInt(&sid, 0);
	int rc = edgeInsert(g, fid, sid);
	printf("%s\n", insertEdgeErrs[rc]);
	return 0;
}


int vertexSearch(Graph* g, double x, double y) {
	for (int i = 0; i < g->vertexN; i++) {
		if (g->adjlist[i].vertex->point->x == x && g->adjlist[i].vertex->point->y == y) return i;
	}
	return NOT_FOUND;
}

int getDouble(double* t) {
	int r = scanf("%lf",t);
	printf("%d\n", r);
	scanf("%c");
	return 0;
}

int vertexInsert(Graph* g, double x, double y) {
	if (!g) return GRAPH_NULLPTR;
	if (vertexSearch(g, x, y) != NOT_FOUND) return DUPLICATE_POINT;
	Point* p = (Point*)calloc(1, sizeof(Point));
	p->x = x;
	p->y = y;
	g->vertexN++;
	p->id = g->vertexN;
	g->adjlist = (AdjList*)realloc(g->adjlist, g->vertexN);
	Vertex* v = (Vertex*)calloc(1, sizeof(Vertex));
	v->point = p;
	g->adjlist[g->vertexN].next = NULL;
	g->adjlist[g->vertexN].vertex = v;
	return 0;
}

int dcreate(Graph* g) {
	if (!g) return GRAPH_NULLPTR;
	int m;
	do {
		
		m = dialog(createmsgs, NCreate);
		if (m == 1) dvertexInsert(g);
		if (m == 2) dedgeInsert(g);
	} while (m);
	return 0;
}

int dvertexInsert(Graph* g) {
	if (!g) return GRAPH_NULLPTR;
	int rc;
	double x, y;
	printf("Enter x: --> ");
	getDouble(&x);
	printf("Enter y: --> ");
	getDouble(&y);
	rc = vertexInsert(g, x, y);
	if (!rc) printf("Created a new vertex, id = %d", g->vertexN);
	printf("%s", insertVertexErrs[rc]);
}

char *getStr(int mode = 1) {
	char *ptr = (char*)malloc(sizeof(char));
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
	if (mode) scanf_s("%*c");
	do {
		n = scanf_s("%80[^\n]", buf, 81);
		if (n < 0) {
			free(ptr);
			ptr = NULL;
			continue;
		}
		if (n == 0) scanf_s("%*c");
		else {
			len += strlen(buf);
			ptr = (char*)realloc(ptr, len + 1);
			strcat(ptr, buf);
		}
	} while (n > 0);
	return ptr;
}

int dload(Graph* g) {
	if (!g) return GRAPH_NULLPTR;
	puts("Enter file name:");
	char* fname = getStr(0);
	int rc = load(g, fname);
	printf("%s\n", loaderrs[rc]);
}

int load(Graph* g, char* fname) {
	if (!g) return GRAPH_NULLPTR;
	if (!fname) return FNAME_NULLPTR;
	FILE* fd = NULL;
	fopen_s(&fd, fname, "r+b");
	if (!fd) return LOADING_FAILURE;
	puts("test");
	fclose(fd);
	return 0;
}


int dialog(const char* msgs[], int N) {
	int rc;
	do {
		for (int i = 0; i < N; i++) puts(msgs[i]);
		puts("Make your choice: -->");
		if (!getInt(&rc, 0)) rc = 0;
		if (rc >= 0 && rc < N) continue;
		puts("I don't understand you. Try again");
	} while (rc < 0 || rc > N);
	return rc;
}


int main() {
	Graph g = { NULL, 0, 0 };
	double d, int rc;
	while (rc = dialog(loadmsgs, NLoadMsgs)) if (!lfptr[rc](&g)) break;
	return 0;
}