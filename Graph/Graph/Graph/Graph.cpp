/*

куда утекает память???????????????????????????????????????????????????????????????????????????????????????????

сделать нормальный getDouble

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
id
x
y
ребра
fid
sid

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
#define EDGE_NOT_FOUND 4
#define STACK_NULLPTR 2
#define INCORRECT_VERTICES_NUMBER 2
#define INCORRECT_EDGES_NUMBER 3
#define TOO_MANY_EDGES 4


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
	Vertex** child;
	int nChilds;
	int ind; 
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
	int ind;
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
const char* generr[] = { "Ok", "Graph nullptr", "Incorrect number of vertices", "Incorrect number of edges", "Too many edges for this number of vertices" };

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
int insertChild(Vertex* parent, Vertex* child);
int delGraph(Graph* g);

int(*mfptr[])(Graph*) = { NULL, dvertexInsert, dedgeInsert, dvertexRemove, decompose, display, dsave, timing, properties };
int(*lfptr[])(Graph*) = { NULL, dload, dgenerate, dcreate };
int(*rvfptr[])(Graph*) = { NULL, getCords, getId };

int delGraph(Graph* g) {
	if (!g) return GRAPH_NULLPTR;
	for (int i = 0; i < g->v; i++) {
		AdjList* tmp = g->adjlist[i].next;
		while (tmp) {
			AdjList* next = tmp->next;
			free(tmp);
			tmp = next;
		}
		free(g->adjlist[i].vertex->point);
		free(g->adjlist[i].vertex->child);
		free(g->adjlist[i].vertex);
	}
	free(g->adjlist);
	return 0;
}

Graph transpose(Graph* g) {
	Graph t = { NULL, 0, 0};
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

int push(Stack** stack, AdjList* a) {
	if (!a) return ADJLIST_NULLPTR;
	if (!stack) return STACK_NULLPTR;
	Stack* tmp = (Stack*)malloc(sizeof(Stack));
	tmp->ind = a->vertex->ind;
	tmp->next = (*stack)->next;
	(*stack)->next = tmp;
	return 0;
}

int dfsVisit(Graph* g, AdjList* a, Stack** stack, int* time) {
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
			dfsVisit(g, &g->adjlist[tmp->vertex->ind], stack, time);
		}
		tmp = tmp->next;
	}
	a->vertex->color = BLACK;
	(*time)++;
	a->vertex->fTime = *time;
	push(stack, a);
	return 0;
}


Stack* dfs(Graph* g, Stack* stack) {
	if (!g) return NULL;
	if (!stack) return NULL;
	for (int i = 0; i < g->v; i++) {
		g->adjlist[i].vertex->color = WHITE;
		g->adjlist[i].vertex->prev = NULL;
		g->adjlist[i].vertex->child = NULL;
	}
	int time = 0;
	for (int i = 0; i < g->v; i++) if (g->adjlist[i].vertex->color == WHITE) dfsVisit(g, &g->adjlist[i], &stack, &time);
	return stack;
}

AdjList* pop(Graph* g, Stack** stack) {
	if (!g) return NULL;
	if (!stack) return NULL;
	Stack* tmp = (*stack)->next;
	if (!tmp) {
		free(*stack);
		return NULL;
	}
	AdjList* t = &g->adjlist[tmp->ind];
	(*stack)->next = (*stack)->next->next;
	free(tmp);
	return t;
}

int insertChild(Vertex* p, Vertex* c) {
	if (!p) return PARENT_NULLPTR;
	if (!c) return CHILD_NULLPTR;
	p->child = (Vertex**)realloc(p->child, sizeof(Vertex*)*(p->nChilds + 1));
	p->child[p->nChilds] = c;
	p->nChilds++;
	return 0;
}

int dfstVisit(Graph* g, AdjList* a) {
	if (!a) return ADJLIST_NULLPTR;
	a->vertex->color = GRAY;
	AdjList* tmp = a->next;
	while (tmp) {
		if (tmp->vertex->color == WHITE) {
			tmp->vertex->prev = a->vertex;
			insertChild(a->vertex, tmp->vertex);
			dfstVisit(g, &g->adjlist[tmp->vertex->ind]);
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
	while (tmp = pop(g, &stack)) if (tmp->vertex->color == WHITE) dfstVisit(g, tmp);
	return 0;
}

Graph scc(Graph* g) {
	Stack* vts = (Stack*)malloc(sizeof(Stack));
	*vts = { -1, NULL };
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
	delGraph(&t);
	return 0;
}

int maxEdgeN(int v) {
	if (v < 0) return 0;
	return v * (v - 1) / 2;
}


int generate(Graph* g, int v, int e) {
	if (!g) return GRAPH_NULLPTR;
	if (v <= 0) return INCORRECT_VERTICES_NUMBER;
	if (e < 0) return INCORRECT_EDGES_NUMBER;
	if (e > maxEdgeN(v)) return TOO_MANY_EDGES;
	int tv = 0, te = 0;
	srand(time(NULL));
	while (tv < v) {
		double x = (double)rand();
		double y = (double)rand();
		int id = tv;
		if (!vertexInsert(g, x, y, id)) tv++;
	}
	while (te < e) {
		int fid = rand() % v;
		int sid = rand() % v;
		if (!edgeInsert(g, fid, sid)) te++;
	}
	return 0;
}

int dgenerate(Graph*g) {
	if (!g) return GRAPH_NULLPTR;
	int v, e;
	printf("Enter number of vertices: --> ");
	getInt(&v, 1);
	printf("Enter number of edges: --> ");
	getInt(&e, 1);
	int rc = generate(g, v, e);
	printf("%s\n", generr[rc]);
	if (rc == 4) printf("You want a graph with %d edges meanwhile maximum is %d\n", e, maxEdgeN(v));
	return 0;
}

int properties(Graph* g) {
	if (!g) return GRAPH_NULLPTR;
	printf("%d vertices\n%d edges\n", g->v, g->e);
	printf("ID\tX\tY\tInd\tFT\tDT\n");
	for (int i = 0; i < g->v; i++)
		printf("%d\t%lf\t%lf\t%d\t%d\t%d\n", g->adjlist[i].vertex->point->id, g->adjlist[i].vertex->point->x, g->adjlist[i].vertex->point->y,
			g->adjlist[i].vertex->ind, g->adjlist[i].vertex->fTime, g->adjlist[i].vertex->dTime);
	return 0;
}

int timing(Graph*g) {
	if (!g) return GRAPH_NULLPTR;
	srand(time(NULL));
	clock_t first, last;
	int n = 1;
	while (n++ < 10) {
		Graph t = { 0, 0, NULL };
		first = clock();
		generate(g, n * 10000, n * 5000);
		last = clock();
		printf("Test #%d, %d vertices, %d edges, time = %d\n", n, n * 10000, n * 5000, last - first);
		first = clock();
		Graph p = scc(&t);
		int k = 0;
		for (int i = 0; i < p.v; i++) if (p.adjlist[i].vertex->prev == NULL) k++;
		last = clock();
		printf("%d\n", p.v);
		printf("%d SCC, time = %d\n\n", k, last - first);
		delGraph(&p);
		delGraph(&t);
	}
	return 0;
}

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
		ind = vertexCordSearch(g, *x, *y);
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
			if (tmp->vertex->ind == ind) {
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
	g->adjlist[ind].vertex->ind = ind;
	g->v--;
	g->adjlist = (AdjList*)realloc(g->adjlist, sizeof(AdjList)*(g->v));
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
	int m = dialog(removeVertexMenu, NRemoveVertexMenu);
	if (!(rvfptr[m](g))) return 0;
	return 0;
}

AdjList* edgeSearch(Graph*g, int fid, int sid) {
	if (!g) return NULL;
	int f = vertexIDSearch(g, fid);
	if (f == -1) return NULL;
	int s = vertexIDSearch(g, sid);
	if (s == -1) return NULL;
	AdjList* tmp = g->adjlist[f].next;
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


int vertexInsert(Graph* g, double x, double y, int id) {
	if (!g) return GRAPH_NULLPTR;
	if (vertexCordSearch(g, x, y) != NOT_FOUND) return DUPLICATE_POINT;
	if (vertexIDSearch(g, id) != NOT_FOUND) return DUPLICATE_ID;
	Point* p = (Point*)calloc(1, sizeof(Point));
	p->x = x;
	p->y = y;
	p->id = id;
	pointInsert(g, p);
	g->v++;
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
	printf("Enter ID --> ", g->v);
	getInt(&id);
	rc = vertexInsert(g, x, y, id);
	printf("%s\n", insertVertexErrs[rc]);
	return 0;
}


int getDouble(double* t) {
	int r;
	do {
		r = scanf_s("%lf", t);
		if (r == 1) return 0;
		printf("Input is incorrect. You should enter a real number\n");
		scanf_s("%*[^\n]");
	} while (1);
	return 1;
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
	char* fname = getStr(1);
	int rc = load(g, fname);
	printf("%s\n", loaderrs[rc]);
	return 0;
}

int pointInsert(Graph* g, Point* p) {
	if (!g) return GRAPH_NULLPTR;
	if (!p) return POINT_NULLPTR;
	if (g->v) g->adjlist = (AdjList*)realloc(g->adjlist, sizeof(AdjList)*(g->v + 1));
	else g->adjlist = (AdjList*)malloc(sizeof(AdjList));
	Vertex* v = (Vertex*)calloc(1, sizeof(Vertex));
	v->point = p;
	v->ind = g->v;
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
	for (int i = 0; i < g->v; i++) {
		fwrite(&g->adjlist[i].vertex->point->id, sizeof(int), 1, fd);
		fwrite(&g->adjlist[i].vertex->point->x, sizeof(double), 1, fd);
		fwrite(&g->adjlist[i].vertex->point->y, sizeof(double), 1, fd);
	}
	for (int i = 0; i < g->v; i++) {
		AdjList* adjtmp = g->adjlist[i].next;
		while (adjtmp) {
			fwrite(&g->adjlist[i].vertex->point->id, sizeof(int), 1, fd); //fid
			fwrite(&adjtmp->vertex->point->id, sizeof(int), 1, fd); //sid
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
	int v, e;
	fread(&v, sizeof(int), 1, fd);
	fread(&e, sizeof(int), 1, fd);
	for (int i = 0; i < v; i++) {
		Point* p = (Point*)malloc(sizeof(Point));
		fread(&p->id, sizeof(int), 1, fd);
		fread(&p->x, sizeof(double), 1, fd);
		fread(&p->y, sizeof(double), 1, fd);
		pointInsert(g, p);
		g->v++;
	}
	for (int i = 0; i < e; i++) {
		int fid, sid;
		fread(&fid, sizeof(int), 1, fd);
		fread(&sid, sizeof(int), 1, fd);
		edgeInsert(g, fid, sid);
	}
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

int graphMemTest() {
	for (int i = 0; i < 50; i++) {
		Graph g = { 0, 0, NULL };
		generate(&g, 1000, 200000);
		delGraph(&g);
	}
	return 0;
}


int main() {
	Graph g = { NULL, 0, 0 };
	double d;
	int rc;
	while (rc = dialog(loadmsgs, NLoadMsgs)) if (!lfptr[rc](&g)) break;
	while (rc = dialog(menu, NMenu)) mfptr[rc](&g);
	delGraph(&g);
	//graphMemTest();
	puts("Program finished");
	return 0;
}