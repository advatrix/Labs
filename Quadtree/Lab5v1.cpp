

//двусвязные списки, границы диапазонов ключей типа float
#include "pch.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)

/*
TO DO:::
2. Переделать функции load и create, они должны работать так:
	сначала вводим параметры дерева
	потом пытаемся загрузить файл (это надо делать после создания дерева, потому что иначе мы не сможем вставить прочитанные элементы)
	если загрузить файл не удалось, создаем его

	пусть dload, считав необходимые данные, передает управление функции createTree, которая уже внутри себя попытается загрузить файл
	ну или создать его по необходимости

3. Сохранение в файл
4. Требования со звездочкой
5. Визуализация всех типов






*/

struct elem {
	int x;
	int y;
	char* info;
};

struct itemList {
	elem* data;
	itemList* prev;
	itemList* next;
};

struct Node {
	int busy;
	float xmax;
	float xmin;
	float ymax;
	float ymin;
	Node* child;
	itemList itemshead;
};

struct QTree {
	int N;
	int n;
	Node* root;
	FILE* fd;
	char* filename;
};
/*
структура файла

n - число элементов в таблице
элементы:
	ключ х
	ключ у
	длина информации
	информация

*/

const char *msgs[] = { "0. Quit", "1. Add new element", "2. Find an element", "3. Delete an element", "4. Show table", "5. Save table", "6. Table properties"};
const char *errmsgs[] = { "Ok", "Error: Duplicate key 1", "Error: Duplicate key 2", "Error: Table overflow", "Table nullpointer", "Data nullptr" };
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
const char *errmsgsdel[] = { "Ok", "Error: item not found" , "Table nullpointer", "Wrong keyspace" };
const char *errload[] = { "Ok", "Tree nullptr", "Filename nullptr", "N is negative" };
const char *errsave[] = { "Ok", "Nullptr file descriptor" };
const char *errins[] = { "Ok", "Tree nullptr", "Element nullptr", "Error: this point already exists", "Incorrect keys: the point doesn\'t fit into the tree" };
const char *errdel[] = {"Ok", ""};
const char *showmsgs[] = { "1. X direct order", "2. Keys range", "3. Tree view" };

int dload(QTree* t);
int dsave(QTree* t);
int delTree(QTree* t);
int dinsert(QTree* t);
int dsearch(QTree* t);
int dremove(QTree* t);
int dshow(QTree* t);
int properties(QTree *t);
//int dgarbageCollector(QTree* t);//а нужна ли тут вообще сборка мусора?
//int garbageCollector(QTree* t);
int save(QTree* t);
int rmv(QTree* t, int x, int y);
int insert(QTree* t, elem* e);
int getInt(int* t, int mode);
int createTree(QTree* t, char* fname, int xmin, int xmax, int ymin, int ymax, int N);
int createFile(QTree* t, char* fname);
int load(QTree* t, char* fname);
int dialog(const char* msgs[], int N);
int quadrant(int x, int y, float xmin, float xmax, float ymin, float ymax);
int divide(Node* parent);
int linsert(elem* item, itemList* head);
char* getStr(int mode);
char* makeStr(const char buf[]);
int search(QTree* t, Node** node, itemList** answer, int x, int y);
itemList*** traverse(Node* node, itemList*** arrptr, int* n);
int showXDirectOrder(QTree* t);
int showRange(QTree* t);
int inRange(itemList*, int xmin, int xmax, int ymin, int ymax);
int showTree(QTree* t);
int treeView(Node* r, int lvl);
int delNode(Node* r, int i);

int delNode(Node* r, int i) {
	itemList* tmp = r->itemshead.next;
	while (tmp) {
		free(tmp->data->info);
		free(tmp->data);
		itemList* next = tmp->next;
		free(tmp);
		tmp = next;
	}
	if (r->child) for (int i = 0; i < 4; i++) delNode(&(r->child[i]), i);
	if (i == 4) free(r);
	return 0;
}
int treeView(Node* r, int lvl) {
	printf("\n");
	for (int i = 0; i < lvl; i++) printf("\t");
	printf("lvl%d [(%.0f; %.0f) - (%.0f; %.0f)] Busy %d\n", lvl, r->xmin, r->ymin, r->xmax, r->ymax, r->busy);
	if (r->busy) {
		itemList* tmp = r->itemshead.next;
		while (tmp) {
			for (int i = 0; i < lvl; i++) printf("\t");
			printf("(%d; %d) %s\n", tmp->data->x, tmp->data->y, tmp->data->info);
			tmp = tmp->next;
		}
	}
	if (r->child) {
		lvl++;
		for (int i = 0; i < 4; i++) treeView(&r->child[i], lvl);
	}
	return 0;
}

int inRange(itemList* item, int xmin, int xmax, int ymin, int ymax) {
	int x = item->data->x, y = item->data->y;
	if (x >= xmin && x <= xmax && y <= ymax && y >= ymin) return 1;
	else return 0;
}

int showRange(QTree* t) {
	int xmin, xmax, ymin, ymax;
	printf("Enter key range:\n");
	printf("Min x: -->");
	getInt(&xmin, -1);
	printf("Max x: -->");
	getInt(&xmax, -1);
	printf("Min y: -->");
	getInt(&ymin, -1);
	printf("Max y: -->");
	getInt(&ymax, -1);
	if (xmin > t->root->xmax || xmax < t->root->xmin || ymin > t->root->ymax || ymax < t->root->xmin) {
		printf("This area doesn't belong to the tree\n");
		return 1;
	}
	itemList** arrptr = (itemList**)malloc(t->n * sizeof(itemList*));
	int n = 0;
	traverse(t->root, &arrptr, &n);
	for (int i = 0; i < n; i++) 
		if (inRange(arrptr[i], xmin, xmax, ymin, ymax)) 
			printf("%d\t%d\t%s\n", arrptr[i]->data->x, arrptr[i]->data->y, arrptr[i]->data->info);
	return 1;
}

int properties(QTree* t) {
	if (!t) return 1;
	printf("Busy: %d\n", t->n);
	printf("Node capacity: %d\n", t->N);
	printf("Filename: %s\n", t->filename);
	return 1;
}

itemList*** traverse(Node* node, itemList*** arrptr, int* n) {
	if (node->child == NULL) {
		itemList* tmp = node->itemshead.next;
		while (tmp) {
			(*arrptr)[*n] = tmp;
			(*n)++;
			tmp = tmp->next;
		}
		return arrptr;
	}
	else {
		arrptr = traverse(&node->child[2], arrptr, n);
		arrptr = traverse(&node->child[3], arrptr, n);
		arrptr = traverse(&node->child[1], arrptr, n);
		arrptr = traverse(&node->child[0], arrptr, n);
	}
}

int showXDirectOrder(QTree* t) {
	itemList** arrptr = (itemList**)malloc(t->n * sizeof(itemList*));
	int n = 0;
	traverse(t->root, &arrptr, &n);
	if (!n) {
		printf("Table is empty\n");
		return 1;
	}
	printf("X\tY\tInfo\n");
	for (int i = 0; i < n; i++) printf("%d\t%d\t%s\n", arrptr[i]->data->x, arrptr[i]->data->y, arrptr[i]->data->info);
	free(arrptr);
	return 0;
}

int showTree(QTree* t) {
	Node* r = t->root;
	treeView(r, 0);
	return 1;
}

int(*sfptr[])(QTree*) = { NULL, showXDirectOrder, showRange, showTree };
int(*fptr[])(QTree*) = { NULL, dinsert, dsearch, dremove, dshow, dsave, properties};

int dshow(QTree* t) {
	if (!t) return 1;
	if (!t->n) {
		printf("Tree is empty\n");
		return 2;
	}
	int k;
	do {
		for (int i = 0; i < 3; i++) printf("%s\n", showmsgs[i]);
		printf("Choose your destiny: -->");
		getInt(&k, 1);
		if (k > 3) printf("I don\'t understand you!!11!n");
	} while (k > 3);
	if (sfptr[k](t)) printf("Ok\n");
	return 1;
}

int dsearch(QTree* t) {
	if (!t) return 1;
	int x, y;
	printf("Enter x-->");
	getInt(&x, -1);
	printf("Enter y -->");
	getInt(&y, -1);
	itemList* ans = NULL;
	Node* r = t->root;
	search(t, &r, &ans, x, y);
	if (!ans) printf("Item not found\n");
	else {
		printf("X: %d\n", ans->data->x);
		printf("Y: %d\n", ans->data->y);
		printf("Info: %s\n", ans->data->info);
	}
	return 1;
}

int rmv(QTree*t, int x, int y) {
	Node*r = t->root;
	itemList* ans = NULL;
	search(t, &r, &ans, x, y);
	if (!r) return 1;
	if (!ans) return 1;
	ans->prev->next = ans->next;
	if (ans->next) ans->next->prev = ans->prev;
	free(ans->data->info);
	free(ans->data);
	free(ans);
	r->busy--;
	t->n--;
	return 0;
}

int dremove(QTree* t) {
	int x, y;
	printf("Enter x -->");
	getInt(&x, -1);
	printf("Enter y -->");
	getInt(&y, -1);
	int rc = rmv(t, x, y);
	printf("%s", errdel[rc]);
	return 1;
}

int dsave(QTree*t) {
	if (!t) return 0;
	int rc = save(t);
	printf("%s", errsave[rc]);
	return 1;
}

int save(QTree* t) {
	if (!t->fd) return 1;
	fseek(t->fd, 0, SEEK_SET);
	fwrite(&t->n, sizeof(int), 1, t->fd);
	itemList** items = (itemList**)malloc(t->n * sizeof(itemList*));
	int n = 0;
	traverse(t->root, &items, &n);
	for (int i = 0; i < n; i++) {
		fwrite(&items[i]->data->x, sizeof(int), 1, t->fd);
		fwrite(&items[i]->data->y, sizeof(int), 1, t->fd);
		int len = strlen(items[i]->data->info) + 1;
		fwrite(&len, sizeof(int), 1, t->fd);
		fwrite(items[i]->data->info, sizeof(char), len, t->fd);
	}
	fseek(t->fd, 0, SEEK_SET);
	free(items);
	return 0;
}

int delTree(QTree* t) {
	Node* r = t->root;
	delNode(r, 4);
	free(t->filename);
	return 0;
}

int linsert(elem* item, itemList* head) {
	itemList *tmp = head;
	itemList* newl = (itemList*)calloc(1, sizeof(itemList));
	newl->data = item;
	while (tmp->next) {
		if (tmp->next->data->x > item->x) break;
		if (tmp->next->data->x == item->x) {
			if (tmp->next->data->y == item->y) return 1;
			if (tmp->next->data->y > item->y) break;
		}
		tmp = tmp->next;
	}
	if (tmp->next) {
		tmp->next->prev = newl;
		newl->prev = tmp;
		newl->next = tmp->next;
		tmp->next = newl;
	}
	else {
		tmp->next = newl;
		newl->prev = tmp;
	}
	return 0;
}

int quadrant(int x, int y, float xmin, float xmax, float ymin, float ymax) {
	/*функция определяет, в каком квадранте ограниченной плоскости лежит точка с координатами х и у
	Возвращаемые значения
	0 -  квадрант
	1 - II квадрант
	2 - III квадрант
	3 - IV квадрант
	-1 - точка лежит за пределами региона


	Примечания:
	точка в самом центре, а также на верхней и правой осях принадлежит I квадранту
	точка на нижней оси принадлежит II квадранту
	точка на левой оси принадлежит IV квадранту
	*/
	if (x < xmin || x > xmax || y < ymin || y > ymax) return -1;
	if (x >= (xmax + xmin) / 2) {
		if (y >= (ymax+ymin) / 2) return 0;
		else return 1;
	}
	else {
		if (y >= (ymax+ymin) / 2) return 3;
		else return 2;
	}
}

int search(QTree* t, Node** node, itemList** answer, int x, int y) {
	while ((*node)->child) {
		int q = quadrant(x, y, (*node)->xmin, (*node)->xmax, (*node)->ymin, (*node)->ymax);
		if (q == -1) return 1;
		*node = &(*node)->child[q];
	}
	*answer = (*node)->itemshead.next;
	while (*answer) {
		if ((*answer)->data->x == x && (*answer)->data->y == y) return 0;
		else *answer = (*answer)->next;
	}
	return 0;
}

int insert(QTree* t, elem* e) {
	if (!t) return 1;
	if (!e) return 2;
	if (e->x > t->root->xmax || e->x < t->root->xmin || e->y > t->root->ymax || e->y < t->root->ymin) {
		free(e);
		return 4;
	}
	Node *r = t->root;
	itemList* ans = NULL;
	search(t, &r, &ans, e->x, e->y);
	if (ans) return 3;
	while (r->busy == t->N) {
		divide(r);
		int q = quadrant(e->x, e->y, r->xmin, r->xmax, r->ymin, r->ymax);
		r = &r->child[q];
	}
	linsert(e, &r->itemshead);
	r->busy++;
	t->n++;
	return 0;
}

int dinsert(QTree*t) {
	if (!t) return 0;
	int x, y;
	char* info;
	printf("Enter x: ->");
	getInt(&x, 1);
	printf("Enter y: ->");
	getInt(&y, 1);
	printf("Enter info:\n");
	info = getStr(1);
	elem* e = (elem*)malloc(sizeof(elem));
	e->x = x;
	e->y = y;
	e->info = info;
	int rc = insert(t, e);
	printf("%s\n", errins[rc]);
	return 1;
}

int divide(Node* parent) {
	if (!parent) return 1;
	parent->child = (Node*)malloc(4 * sizeof(Node));
	for (int i = 0; i < 4; i++) {
		parent->child[i].busy = 0;
		parent->child[i].busy = 0;
		parent->child[i].itemshead.data = NULL;
		parent->child[i].itemshead.next = NULL;
		parent->child[i].itemshead.prev = NULL;
		parent->child[i].child = NULL;
	}
	parent->child[0].xmin = (parent->xmin + parent->xmax) / 2;
	parent->child[0].xmax = parent->xmax;
	parent->child[0].ymin = (parent->ymin + parent->ymax) / 2;
	parent->child[0].ymax = parent->ymax;
	parent->child[1].xmin = (parent->xmin + parent->xmax) / 2;
	parent->child[1].xmax = parent->xmax;
	parent->child[1].ymin = parent->ymin;
	parent->child[1].ymax = (parent->ymin + parent->ymax) / 2;
	parent->child[2].xmin = parent->xmin;
	parent->child[2].xmax = (parent->xmin + parent->xmax) / 2;
	parent->child[2].ymin = parent->ymin;
	parent->child[2].ymax = (parent->ymin + parent->ymax) / 2;
	parent->child[3].xmin = parent->xmin;
	parent->child[3].xmax = (parent->xmin + parent->xmax) / 2;
	parent->child[3].ymin = (parent->ymin + parent->ymax) / 2;
	parent->child[3].ymax = parent->ymax;
	itemList* tmp = parent->itemshead.next;
	while (tmp) {
		int q = quadrant(tmp->data->x, tmp->data->y, parent->xmin, parent->xmax, parent->ymin, parent->ymax);
		linsert(tmp->data, &parent->child[q].itemshead);
		parent->child[q].busy++;
		itemList* tmp1 = tmp;
		tmp = tmp->next;
		free(tmp1);
	}
	parent->itemshead.next = NULL;
	parent->busy = 0;
	return 0;
}

int create(QTree* t, char* fname, int xmin, int xmax, int ymin, int ymax, int N) {
	if (!t) return 1;
	if (!fname) return 2;
	if (N <= 1) return 3;
	if (xmin > xmax) {
		printf("Minimal X is greater than maximal one, it\'s interesting... They\'ll be swapped\n");
		int tmp = xmin;
		xmin = xmax;
		xmax = tmp;
	}
	if (xmin == xmax || ymin == ymax) {
		//а должна ли тут вылетать ошибка? Наверное, не обязательно...
	}
	if (ymin > ymax) {
		printf("Minimal Y is greater than maximal one, it\'s interesting... They\'ll be swapped\n");
		int tmp = ymin;
		ymin = ymax;
		ymax = tmp;
	}
	Node* root = (Node*)malloc(1 * sizeof(Node));
	t->root = root;
	t->N = N;
	t->n = 0;
	t->filename = fname;
	t->root->xmax = xmax;
	t->root->xmin = xmin;
	t->root->ymin = ymin;
	t->root->ymax = ymax;
	t->root->busy = 0;
	t->root->itemshead.data = NULL;
	t->root->itemshead.next = NULL;
	t->root->itemshead.prev = NULL;
	if (!(fopen_s(&(t->fd), fname, "w+b"))) t->root = NULL;
	return 0;
}

int createFile(QTree*t, char* fname) {
	if (!t) return 1;
	if (!fname) return 2;
	fopen_s(&(t->fd), fname, "w+b");
	return 0;
}

int load(QTree *t, char *fname) {
	if (!t) return 1;
	if (!fname) return 2;
	fopen_s(&t->fd, fname, "r+b");
	if (!t->fd) return 3;
	t->filename = fname;
	int n;
	fread(&n, sizeof(int), 1, t->fd);
	int errs = 0;
	for (int i = 0; i < n; i++) {
		elem *tmp = (elem*)calloc(1, sizeof(elem));
		fread(&tmp->x, sizeof(int), 1, t->fd);
		fread(&tmp->y, sizeof(int), 1, t->fd);
		int len;
		fread(&len, sizeof(int), 1, t->fd);
		char* info = (char*)malloc(len);
		*info = '\0';
		tmp->info = info;
		fread(tmp->info, sizeof(char), len, t->fd);
		if (insert(t, tmp) == 4) errs++;
	}
	if (errs) printf("%d points weren\'t loaded because they don\'t fit the tree\n", errs);
	return 0;
}


int dload(QTree *t) {
	int rc = 1, xmin, xmax, ymin, ymax, N;
	char* fname = NULL;
	while(rc) {
		printf("Enter file name: --> ");
		fname = getStr(0);
		if (!fname) return 0;
		printf("Enter key range:\n");
		printf("Min x: -->");
		getInt(&xmin, -1);
		printf("Max x: -->");
		getInt(&xmax, -1);
		printf("Min y: -->");
		getInt(&ymin, -1);
		printf("Max y: -->");
		getInt(&ymax, -1);
		printf("N: -->");
		getInt(&N, 1);
		rc = createTree(t, fname, xmin, xmax, ymin, ymax, N);
		printf("%s\n", errload[rc]);
	}
}

int createTree(QTree* t, char* fname, int xmin, int xmax, int ymin, int ymax, int N) {
	if (!t) return 1;
	if (!fname) return 2;
	if (N < 1) return 3;
	if (xmin > xmax) {
		printf("Minimal X is greater than maximal one, it\'s interesting... They\'ll be swapped\n");
		int tmp = xmin;
		xmin = xmax;
		xmax = tmp;
	}
	if (ymin > ymax) {
		printf("Minimal Y is greater than maximal one, it\'s interesting... They\'ll be swapped\n");
		int tmp = ymin;
		ymin = ymax;
		ymax = tmp;
	}
	Node* root = (Node*)malloc(1 * sizeof(Node));
	t->root = root;
	t->N = N;
	t->n = 0;
	t->filename = fname;
	t->root->xmax = xmax;
	t->root->xmin = xmin;
	t->root->ymin = ymin;
	t->root->ymax = ymax;
	t->root->busy = 0;
	t->root->itemshead.data = NULL;
	t->root->itemshead.next = NULL;
	t->root->itemshead.prev = NULL;
	t->root->child = NULL;
	if (load(t, fname)) createFile(t, fname);
	/*if (!(fopen_s(&(t->fd), fname, "w+b"))) {
		t->root = NULL;
	}*/
	return 0;
}

char* makeStr(const char buf[]) {
	char *s = (char*)malloc(strlen(buf) + 1);
	*s = '\0';
	strcat(s, buf);
	return s;
}

int dialog(const char* msgs[], int N) {
	int rc;
	do {
		for (int i = 0; i < N; ++i) puts(msgs[i]);
		puts("Make your choice: --> ");
		if (!getInt(&rc, 0)) rc = 0;
		if (rc >= 0 && rc < N) continue;
		puts("I don't understand you. Please try again.");
	} while (rc < 0 || rc >= N);
	return rc;
}

int getInt(int *t, int mode = -1) {
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

int main() {
	QTree t = { 0, NULL, NULL };
	int rc;
	if (!dload(&t)) return 1;
	while (rc = dialog(msgs, NMsgs)) if (!fptr[rc](&t)) break;
	dsave(&t);
	puts("Program finished");
	fclose(t.fd);
	delTree(&t);
	return 0;
}
