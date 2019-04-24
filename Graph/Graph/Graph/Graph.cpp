﻿/*

сделать нормальную структуру стека

пофиксить выход из программы при завершении ручного создания графа


сделать разложение на сильно связные компоненты



при первом поиске в глубину вершины надо записывать в стек по времени f

сделать нормальный getDouble
сделать удаление графа из памяти




координаты вещественые числа

вывод графа в виде списков смежности

а у нас может быть мультиграф??? - нет
у нас могут быть петли??? - да

1. Написать функцию считывания вещественных чисел (getDouble (double* t));
2. Создание графа:
- загрузка из файла
- задать вручную
- рандомная генерация


3. процесс работы

вывод возможностей загрузки графа
1. Из файла
2. Сгенерировать
	входные данные:
		количество вершин
		количество ребер
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

структура файла:
v
e
точки
ребра

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
#define UNDEFINED_VERTEX 3
#define VERTEX_NOT_FOUND 2
#define POINT_NULLPTR 2
#define SAVE_ERROR 3
#define FIRST_NOT_FOUND 2
#define SECOND_NOT_FOUND 3
#define DUPLICATE_ID 3
#define ARRAY_NULLPTR 1
#define ADJLIST_NULLPTR 1
#define TIME_NULLPTR 2
#define PARENT_NULLPTR 1
#define CHILD_NULLPTR 2
#define NODE_NULLPTR 1

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
	Vertex* prev;
	Vertex** child; //ссылка на массив потомков в дереве поиска
	int nChilds; //количество потомков в дереве поиска
};

struct Edge {
	int fid;
	int sid;
};

struct AdjList {
	Vertex* vertex;
	AdjList* next;
};

struct Graph {
	AdjList* adjlist;
	int v;
	int e;
};

struct Stack {
	AdjList* vertex;
	Stack* next;
};

const char* loadmsgs[] = { "0. Quit", "1. Load graph", "2. Generate graph", "3. Create graph manually" };
const char* menu[] = { "0. Quit", "1. Add vertex", "2. Add edge", "3. Delete vertex", "4. Strongly connected components", "5. Display graph (as adjacency lists)", "6. Save graph", "7. Timing", "8. Graph properties" };
const char* loaderrs[] = { "Ok", "Graph nullptr", "Filename nullptr", "Failed to load the file" };
const char* createmsgs[] = { "0. Quit", "1. Add vertex", "2. Add edge" };
const char* insertVertexErrs[] = { "Ok", "Graph nullptr", "Duplicate point", "Duplicate ID" };
const char* insertEdgeErrs[] = { "Ok", "Graph nullptr", "First vertex not found", "Second vertex not found" };
const char* removeVertexMenu[] = { "0. Quit", "1. Enter coordinates", "2. Enter id" };
const char* removeVertexErrs[] = { "Ok", "Graph nullptr", "Vertex not found", "Undefined vertex" };
const char* saveerrs[] = { "Ok", "Graph nullptr", "Filename nullptr", "Saving error" };

const int NLoadMsgs = sizeof(loadmsgs) / sizeof(loadmsgs[0]);
const int NMenu = sizeof(menu) / sizeof(menu[0]);
const int NCreate = sizeof(createmsgs) / sizeof(createmsgs[0]);
const int NRemoveVertexMenu = sizeof(removeVertexMenu) / sizeof(removeVertexMenu[0]);

int dialog(const char* msgs[], int N);
int getInt(int *t, int mode = -1);
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
//Graph scc(Graph*);
//int dfs(Graph*);
//int dfst(Graph*);
int load(Graph* g, char* fname);
int pointInsert(Graph* g, Point* p);
int getDouble(double* t);
int vertexInsert(Graph* g, double x, double y, int id);
int vertexCordSearch(Graph* g, double x, double y);
int vertexIDSearch(Graph* g, int id);
int vertexRemove(Graph* g, double* x, double* y, int* id);
int edgeInsert(Graph* g, int fid, int sid);
int save(Graph* g, char* fname);
char* getStr(int mode);
AdjList* edgeSearch(Graph* g, int fid, int sid);
Graph transpose(Graph*);
//int dfsVisit(AdjList* a, int* time);
int insertChild(Vertex* parent, Vertex* child);


int(*mfptr[])(Graph*) = { NULL, dvertexInsert, dedgeInsert, dvertexRemove, decompose, display, dsave, timing, properties };
int(*lfptr[])(Graph*) = { NULL, dload, dgenerate, dcreate };
int(*rvfptr[])(Graph*) = { NULL, getCords, getId };
/*




*/

Graph transpose(Graph* g) {
	Graph t;
	for (int i = 0; i < g->v; i++) vertexInsert(&t, g->adjlist[i].vertex->point->x, g->adjlist[i].vertex->point->y, g->adjlist[i].vertex->point->id);
	for (int i = 0; i < g->v; i++) {
		AdjList* tmp = g->adjlist[i].next;
		while (tmp) {
			edgeInsert(&t, tmp->vertex->point->id, g->adjlist[i].vertex->point->id);
			tmp = tmp->next;
		}
	}
	return t;
}

int push(Stack* stack, AdjList* a) {
	if (!a) return ADJLIST_NULLPTR;
	Stack* tmp = (Stack*)malloc(sizeof(Stack));
	tmp->vertex = a;
	tmp->next = stack;
	stack = tmp;
	return 0;
}

int dfsVisit(AdjList* a, Stack* stack, int* time) {
	if (!a) return ADJLIST_NULLPTR;
	if (!time) return TIME_NULLPTR;
	a->vertex->color = GRAY;
	a->vertex->dTime = *time;
	(*time)++;
	AdjList* tmp = a->next;
	while (tmp) {
		if (tmp->vertex->color == WHITE) {
			tmp->vertex->prev = a->vertex;
			insertChild(a->vertex, tmp->vertex);
			dfsVisit(tmp, stack, time);
		}
		tmp = tmp->next;
	}
	a->vertex->color = BLACK;
	(*time)++;
	a->vertex->fTime = *time;
	push(stack, a);
	return 0;
}

int dfs(Graph* g, Stack* stack) {
	if (!g) return GRAPH_NULLPTR;
	for (int i = 0; i < g->v; i++) {
		g->adjlist[i].vertex->color = WHITE;
		g->adjlist[i].vertex->prev = NULL;
		g->adjlist[i].vertex->child = NULL;
	}
	int time = 0;
	for (int i = 0; i < g->v; i++) if (g->adjlist[i].vertex->color == WHITE) dfsVisit(&g->adjlist[i], stack, &time);
	return 0;
}

AdjList* pop(Stack* stack) {
	if (!stack) return NULL;
	/*Vertex* t = stack->vertex;
	AdjList* tmp = stack;
	stack = stack->next;
	free(tmp);
	return t;*/
	AdjList* t = stack->vertex;
	Stack* tmp = stack;
	stack = stack->next;
	free(tmp);
	return t;
}

int insertChild(Vertex* p, Vertex* c) {
	if (!p) return PARENT_NULLPTR;
	if (!c) return CHILD_NULLPTR;
	p->child = (Vertex**)realloc(p->child, p->nChilds);
	p->child[p->nChilds] = c;
	p->nChilds++;
	return 0;
}

int dfstVisit(AdjList* a, Stack* stack) {
	if (!a) return ADJLIST_NULLPTR;
	a->vertex->color = GRAY;
	AdjList* tmp = a;
	while (tmp) {
		if (tmp->vertex->color == WHITE) {
			tmp->vertex->prev = a->vertex;
			insertChild(a->vertex, tmp->vertex);
			dfstVisit(tmp, stack);
		}
		tmp = tmp->next;
	}
	a->vertex->color = BLACK;
	return 0;
}

int dfst(Graph* g, Stack* stack) {
	if (!g) return GRAPH_NULLPTR;
	for (int i = 0; i < g->v; i++) {
		g->adjlist[i].vertex->color = WHITE;
		g->adjlist[i].vertex->prev = NULL;
		g->adjlist[i].vertex->child = NULL;
	}
	AdjList* tmp;
	while (tmp = pop(stack)) if (tmp->vertex->color == WHITE) dfstVisit(tmp, stack);
	return 0;
}

Graph scc(Graph* g) {
	Stack* vts = NULL;
	dfs(g, vts);
	Graph t = transpose(g);
	dfst(&t, vts);
	return t;
}

int displayNode(Vertex* v) {
	if (!v) return UNDEFINED_VERTEX;
	printf("Vertex %d (%lf; %lf)\n", v->point->id, v->point->x, v->point->y);
	if (v->child) for (int i = 0; i < v->nChilds; i++) displayNode(v->child[i]);
	return 0;
}

int displayTree(Graph* g) {
	if (!g) return GRAPH_NULLPTR;
	for (int i = 0; i < g->v; i++) if (g->adjlist[i].vertex->prev == NULL) {
		displayNode(g->adjlist[i].vertex);
		printf("\n\n\n");
	}
	return 0;
}

int decompose(Graph* g) {
	if (!g) return GRAPH_NULLPTR;
	Graph t = scc(g);
	displayTree(&t);
	return 0;

}

int dgenerate(Graph*g) {
	if (!g) return GRAPH_NULLPTR;
	puts("text");
	return 0;
}

int properties(Graph* g) {
	if (!g) return GRAPH_NULLPTR;
	printf("%d vertices\n %d edges\n", g->v, g->e);
	return 0;
}

int timing(Graph*g) {
	if (!g) return GRAPH_NULLPTR;
	puts("test");
	return 0;
}
/*
int displayNode(Vertex* node) {
	if (!node) return NODE_NULLPTR;
	for (int i = 0; i < )
}

int displaySCC(Graph* t) {
	if (!t) return GRAPH_NULLPTR;
	for (int i = 0; i < t->v; i++) {
		if (!t->adjlist[i].vertex->prev) displayTree(t->adjlist[i].vertex);
	}
	return 0;
}

int decompose(Graph* g) {
	if (!g) return GRAPH_NULLPTR;
	Graph t = scc(g);
	displaySCC(&t);
	return 0;
}

int dfsVisit(AdjList* a, int* time) {
	if (!a) return ADJLIST_NULLPTR;
	if (!time) return TIME_NULLPTR;
	a->vertex->color = GRAY;
	a->vertex->dTime = *time;
	(*time)++;
	AdjList* tmp = a->next;
	while (tmp) {
		if (tmp->vertex->color == WHITE) {
			tmp->vertex->prev = a->vertex;
			insertChild(a->vertex, tmp->vertex);
			dfsVisit(tmp, time);
		}
		tmp = tmp->next;
	}
	a->vertex->color = BLACK;
	(*time)++;
	a->vertex->fTime = *time;
	return 0;
}

int insertChild(Vertex* parent, Vertex* child) {
	if (!parent) return PARENT_NULLPTR;
	if (!child) return CHILD_NULLPTR;
	parent->child = (Vertex**)realloc(parent->child, parent->nChilds + 1);
	parent->child[parent->nChilds] = child;
	return 0;
}

Graph transpose(Graph* g) {
	Graph t;
	for (int i = 0; i < g->v; i++) vertexInsert(&t, g->adjlist[i].vertex->point->x, g->adjlist[i].vertex->point->y, g->adjlist[i].vertex->point->id);
	for (int i = 0; i < g->v; i++) {
		AdjList* tmp = g->adjlist[i].next;
		while (tmp) {
			edgeInsert(&t, tmp->vertex->point->id, g->adjlist[i].vertex->point->id);
			tmp = tmp->next;
		}
	}
	return t;
}

int dfs(Graph* g) {
	if (!g) return GRAPH_NULLPTR;
	for (int i = 0; i < g->v; i++) {
		Vertex* v = g->adjlist[i].vertex;
		v->color = WHITE;
		v->prev = NULL;
		v->child = NULL;
	}
	int time = 0;
	for (int i = 0; i < g->v; i++) if (g->adjlist[i].vertex->color == WHITE) dfsVisit(&g->adjlist[i], &time);
	return 0;
}

int timeSort(AdjList** a, int n) {
	if (!a) return ARRAY_NULLPTR;
	AdjList* tmp;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (a[j]->vertex->dTime < a[j + 1]->vertex->dTime) {
				tmp = a[j];
				a[j] = a[j + 1];
				a[j + 1] = tmp;
			}
		}
	}
	return 0;
}

int dfstVisit(AdjList* a) {
	if (!a) return ADJLIST_NULLPTR;
	a->vertex->color = GRAY;
	AdjList* tmp = a->next;
	while (tmp) {
		if (tmp->vertex->color == WHITE) {
			tmp->vertex->prev = a->vertex;
			insertChild(a->vertex, tmp->vertex);
			dfstVisit(tmp);
		}
		tmp = tmp->next;
	}
	a->vertex->color = BLACK;
	return 0;
}

int dfst(Graph* g) {
	if (!g) return GRAPH_NULLPTR;
	AdjList** ptrarr = (AdjList**)malloc(g->v * sizeof(AdjList*));
	timeSort(ptrarr, g->v);
	for (int i = 0; i < g->v; i++) {
		ptrarr[i]->vertex->color = WHITE;
		ptrarr[i]->vertex->prev = NULL;
	}
	for (int i = 0; i < g->v; i++) if (ptrarr[i]->vertex->color == WHITE) dfstVisit(ptrarr[i]);
	return 0;
}

Graph scc(Graph* g) {
	dfs(g);
	Graph t = transpose(g);
	dfst(&t);
	return t;
}*/

int vertexIDSearch(Graph* g, int id) {
	for (int i = 0; i < g->v; i++) if (g->adjlist[i].vertex->point->id == id) return i;
	return NOT_FOUND;
}

int display(Graph* g) {
	if (!g) return GRAPH_NULLPTR;
	if (g->v) {
		for (int i = 0; i < g->v; i++) {
			printf("Vertex %d (%lf; %lf) is connected with", g->adjlist[i].vertex->point->id, g->adjlist[i].vertex->point->x, g->adjlist[i].vertex->point->y);
			AdjList* tmp = g->adjlist[i].next;
			if (!tmp) printf(" no vertices");
			printf("\n");
			while (tmp) {
				printf("\t %d (%lf; %lf)\n", tmp->vertex->point->id, tmp->vertex->point->x, tmp->vertex->point->y);
				tmp = tmp->next;
			}
			printf("\n\n");
		}
	}
	else printf("Graph is empty\n");
	return 0;
}

int vertexRemove(Graph* g, double* x, double* y, int* id) {
	if (!g) return GRAPH_NULLPTR;
	int ind;
	if (!id) {
		if (!x || !y) return UNDEFINED_VERTEX;
		ind = vertexCordSearch(g, *x, *y); //тут может вылезти ошибка, потому что идет разыменовывание нулевого указателя
		//в таком случае можно попробовать так:
		/*
		int tempid = vertexSearch(g, *x, *y);
		id = &tempid;
		*/
	} else ind = vertexIDSearch(g, *id);
	if (ind == -1) return VERTEX_NOT_FOUND;
	AdjList* tmp = g->adjlist[ind].next, *next, *prev;
	while (tmp) {
		next = tmp->next;
		free(tmp);
		tmp = next;
		g->e--;
	}
	for (int i = 0; i < g->v; i++) {
		if (i == ind) continue;
		prev = &g->adjlist[i];
		tmp = prev->next;
		while (tmp) {
			if (tmp->vertex->point->id == *id) {
				tmp = tmp->next;
				free(prev->next);
				prev->next = tmp;
				g->e--;
			}
			else {
				prev = tmp;
				tmp = tmp->next;
			}
		}
	}
	free(g->adjlist[ind].vertex->point);
	free(g->adjlist[ind].vertex);
	g->adjlist[ind] = g->adjlist[g->v - 1];
	g->v--;
	g->adjlist = (AdjList*)realloc(g->adjlist, g->v - 1);
	return 0;
}

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
	if (edgeSearch(g, fid, sid)) return EDGE_DUPLICATE;
	AdjList* a = (AdjList*)malloc(sizeof(AdjList));
	int s = vertexIDSearch(g, sid);
	if (s == -1) return FIRST_NOT_FOUND;
	int f = vertexIDSearch(g, fid);
	if (f == -1) return SECOND_NOT_FOUND;
	a->vertex = g->adjlist[s].vertex;
	a->next = g->adjlist[f].next;
	g->adjlist[f].next = a;
	g->e++;
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


int vertexCordSearch(Graph* g, double x, double y) {
	for (int i = 0; i < g->v; i++) {
		if (g->adjlist[i].vertex->point->x == x && g->adjlist[i].vertex->point->y == y) return i;
	}
	return NOT_FOUND;
}

int getDouble(double* t) {
	int r = scanf("%lf",t);
	printf("%d\n", r);
	scanf("*%c");
	return 0;
}

int vertexInsert(Graph* g, double x, double y, int id) {
	if (!g) return GRAPH_NULLPTR;
	if (vertexCordSearch(g, x, y) != NOT_FOUND) return DUPLICATE_POINT;
	if (vertexIDSearch(g, id) != NOT_FOUND) return DUPLICATE_ID;
	Point* p = (Point*)calloc(1, sizeof(Point));
	p->x = x;
	p->y = y;
	g->v++;
	p->id = id;
	pointInsert(g, p);
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
	int rc, id = g->v;
	double x, y;
	printf("Enter x: --> ");
	getDouble(&x);
	printf("Enter y: --> ");
	getDouble(&y);
	printf("Enter ID (for example, %d): --> ", g->v);
	getInt(&id);
	rc = vertexInsert(g, x, y, id);
	if (!rc) printf("Created a new vertex, id = %d", g->v);
	printf("%s", insertVertexErrs[rc]);
	return 0;
}

int getInt(int *t, int mode) {
	int r;
	do {
		r = scanf_s("%d", t);
		if (r == 1) {
			if (mode == -1) return r;
			if (mode == 0) {
				if (t >= 0) return r;
				else {
					printf("Input is incorrect. You should enter a non-negative integer. Try again\n");
					scanf_s("%*[^\n]");
					continue;
				}
			}
			else if (mode == 1) {
				if (t > 0) return r;
				else {
					printf("Input is incorrect. You should enter a positive integer. Try again\n");
					scanf_s("%*[^\n]");
					continue;
				}
			}
			else return r;
		}
		printf("Input is incorrect. You should enter an integer. Try again.\n");
		scanf_s("%*[^\n]");
	} while (1);
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
	return 0;
}

int pointInsert(Graph* g, Point* p) {
	if (!g) return GRAPH_NULLPTR;
	if (!p) return POINT_NULLPTR;
	if (g->v) g->adjlist = (AdjList*)realloc(g->adjlist, g->v);
	else g->adjlist = (AdjList*)malloc(sizeof(AdjList));
	Vertex* v = (Vertex*)calloc(1, sizeof(Vertex));
	v->point = p;
	g->adjlist[g->v].next = NULL;
	g->adjlist[g->v].vertex = v;
	return 0;
}

int save(Graph* g, char* fname) {
	if (!g) return GRAPH_NULLPTR;
	if (!fname) return FNAME_NULLPTR;
	FILE* fd;
	fopen_s(&fd, fname, "w+b");
	if (!fd) return SAVE_ERROR;
	fseek(fd, 0, SEEK_SET);
	fwrite(&g->v, sizeof(int), 1, fd);
	fwrite(&g->e, sizeof(int), 1, fd);
	for (int i = 0; i < g->v; i++) fwrite(&g->adjlist[i].vertex->point, sizeof(Point), 1, fd);
	for (int i = 0; i < g->v; i++) {
		AdjList* adjtmp = g->adjlist[i].next;
		while (adjtmp) {
			Edge tmp = { i, adjtmp->vertex->point->id };
			fwrite(&tmp, sizeof(Edge), 1, fd);
			adjtmp = adjtmp->next;
		}
	}
	fclose(fd);
	free(fname);
	return 0;
}

int dsave(Graph* g) {
	printf("Enter file name: --> ");
	char* fname = getStr(1);
	int rc = save(g, fname);
	printf("%s\n", saveerrs[rc]);
	return 0;
}

int load(Graph* g, char* fname) {
	if (!g) return GRAPH_NULLPTR;
	if (!fname) return FNAME_NULLPTR;
	FILE* fd = NULL;
	fopen_s(&fd, fname, "r+b");
	if (!fd) {
		free(fname);
		return LOADING_FAILURE;
	}
	fread(&g->v, sizeof(int), 1, fd);
	fread(&g->e, sizeof(int), 1, fd);
	for (int i = 0; i < g->v; i++) {
		Point* p = (Point*)malloc(sizeof(Point));
		fread(p, sizeof(Point), 1, fd);
		pointInsert(g, p);
	}
	for (int i = 0; i < g->e; i++) {
		Edge tmp; //возможно тут надо её задавать маллоком, если будет ругань
		fread(&tmp, sizeof(Edge), 1, fd);
		edgeInsert(g, tmp.fid, tmp.sid);
	}
	//for (int i = 0; i < g->v; i++) g->adjlist[i].vertex->point->id = i; это не нужно, потому что у нас id это ключ в просматриваемой таблице,
	//а не индекс вершины в массиве вершин
	fclose(fd);
	free(fname);
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
	double d;
	int rc;
	while (rc = dialog(loadmsgs, NLoadMsgs)) if (!lfptr[rc](&g)) break;
	return 0;
}