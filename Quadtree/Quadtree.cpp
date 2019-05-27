/*
Q-дерево с данными в листьях
*/

#include "pch.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#pragma warning(disable:4996)

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
	Node* parent;
	itemList itemshead;
};

struct QTree {
	int N;
	int n;
	Node* root;
	FILE* fd;
	char* filename;
};


const char *msgs[] = { "0. Quit", "1. Add new element", "2. Find an element", "3. Delete an element", "4. Show table", "5. Save table", "6. Table properties", "7. Timing", "8. Find nearest neighbour" };
const char *errmsgs[] = { "Ok", "Error: Duplicate key 1", "Error: Duplicate key 2", "Error: Table overflow", "Table nullpointer", "Data nullptr" };
const char *errmsgsdel[] = { "Ok", "Error: item not found" , "Table nullpointer", "Wrong keyspace" };
const char *errload[] = { "Ok", "Tree nullptr", "Filename nullptr", "N is negative" };
const char *errsave[] = { "Ok", "Nullptr file descriptor" };
const char *errins[] = { "Ok", "Tree nullptr", "Element nullptr", "Error: this point already exists", "Incorrect keys: the point doesn\'t fit into the tree" };
const char *errdel[] = { "Ok", "" };
const char *showmsgs[] = { "1. X direct order", "2. Keys range", "3. Tree view" };

const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int dload(QTree* t);
int dsave(QTree* t);
int delTree(QTree* t);
int dinsert(QTree* t);
int dsearch(QTree* t);
int dremove(QTree* t);
int dshow(QTree* t);
int properties(QTree *t);
int save(QTree* t);
int rmv(QTree* t, int x, int y);
int insert(QTree* t, elem* e);
int getInt(int* t, int mode);
int createTree(QTree* t, char* fname, int xmin, int xmax, int ymin, int ymax, int N);
int createFile(QTree* t, char* fname);
int load(QTree* t, char* fname);
int dialog(const char* msgs[], int N);
int quadrant(int x, int y, float xmin, float xmax, float ymin, float ymax, int mode);
int divide(Node* parent);
int linsert(elem* item, itemList* head);
char* getStr(int mode);
char* makeStr(const char buf[]);
int search(QTree* t, Node** node, itemList** answer, int x, int y);
itemList*** traverse(Node* node, itemList*** arrptr, int* n);
int showXDirectOrder(QTree* t);
int showXRange(QTree* t);
int showTree(QTree* t);
int treeView(Node* r, int lvl);
int delNode(Node* r, int i);
int timing(QTree* t);
int sort(itemList*** a, int n);
int rangeTraverse(Node* node, int xmin, int xmax, int ymin, int ymax);
double sqrdist(itemList* p1, itemList* p2);
int dnearest(QTree* t);
itemList* checkNode(Node* node, int x, int y);
int nearestNeighbour(QTree* t, int x, int y);
int nearTraverse(Node* node, int xmax, int xmin, int ymax, int ymin, double* mindist, itemList** nearest, itemList* p);

int(*sfptr[])(QTree*) = { NULL, showXDirectOrder, showXRange, showTree };
int(*fptr[])(QTree*) = { NULL, dinsert, dsearch, dremove, dshow, dsave, properties, timing, dnearest };

int sort(itemList*** a, int n) {
	itemList* tmp;
	for (int i = 0; i < n - 1; i++) for (int j = 0; j < n - i - 1; j++) if ((*a)[j]->data->x > (*a)[j + 1]->data->x) {
		tmp = (*a)[j];
		(*a)[j] = (*a)[j + 1];
		(*a)[j + 1] = tmp;
	}
	return 0;
}

int nearestNeighbour(QTree* t, int x, int y) {
	itemList* ans, *p = NULL;
	Node* r = t->root;
	search(t, &r, &ans, x, y);
	if (!ans) {
		printf("Item not found\n");
		return 1;
	}
	if (t->n == 1) {
		printf("There is only one point in the tree, it is alone\n");
		return 1;
	}
	Node* tmp = r;
	while (tmp) {
		p = checkNode(tmp, x, y);
		if (p) break;
		tmp = tmp->parent;
	}
	double mindist = sqrdist(p, ans);
	if (t->n == 2) {
		printf("Nearest neighbour:\n(%d; %d) %s\ndist = %f\n", p->data->x, p->data->y, p->data->info, sqrt(mindist));
		return 1;
	}
	int xmin, xmax, ymin, ymax;
	int a = abs(x - p->data->x);
	int b = abs(x - p->data->y);
	a = (a > b) ? a : b;
	xmin = x - a;
	xmax = x + a;
	ymin = y - a;
	ymax = y + a;
	itemList* nearest = p;
	tmp = t->root;
	nearTraverse(tmp, xmin, xmax, ymin, ymax, &mindist, &nearest, ans);
	printf("Nearest neighbour:\n(%d; %d) %s\ndist = %f\n", nearest->data->x, nearest->data->y, nearest->data->info, sqrt(mindist));
	return 0;
}

int nearTraverse(Node* node, int xmin, int xmax, int ymin, int ymax, double* mindist, itemList** ans, itemList* p) {
	if (!node) return 1;
	if (node->child == NULL) {
		itemList* tmp = node->itemshead.next;
		while (tmp) {
			if (tmp->data->x >= xmin && tmp->data->x <= xmax && tmp->data->y >= ymin && tmp->data->y <= ymax)
				if (sqrdist(tmp, p) < *mindist && sqrdist(tmp, p)) {
					*mindist = sqrdist(tmp, p);
					*ans = tmp;
				}
			tmp = tmp->next;
		}
		return 0;
	}
	else {
		int qmin = quadrant(xmin, ymin, node->xmin, node->xmax, node->ymin, node->ymax, 0);
		int qmax = quadrant(xmax, ymax, node->xmin, node->xmax, node->ymin, node->ymax, 0);
		if (qmin == qmax) nearTraverse(&node->child[qmin], xmin, xmax, ymin, ymax, mindist, ans, p);
		else if (abs(qmin - qmax) == 2) {
			for (int i = 0; i < 4; i++) nearTraverse(&node->child[i], xmin, xmax, ymin, ymax, mindist, ans, p);
		}
		else {
			nearTraverse(&node->child[qmin], xmin, xmax, ymin, ymax, mindist, ans, p);
			nearTraverse(&node->child[qmax], xmin, xmax, ymin, ymax, mindist, ans, p);
		}
	}
	return 0;
}

itemList* checkNode(Node* node, int x, int y) {
	itemList* a;
	if (node->itemshead.next) 
			if (node->itemshead.next->data->x != x || node->itemshead.next->data->y != y) return node->itemshead.next;
			else if (node->itemshead.next->next)
				if (node->itemshead.next->next->data->x != x || node->itemshead.next->next->data->y != y)
					return node->itemshead.next->next;
	if (node->child) {
		if (!(a = checkNode(&node->child[0], x, y))) {
			if (!(a = checkNode(&node->child[1], x, y))) {
				if (!(a = checkNode(&node->child[2], x, y))) {
					return checkNode(&node->child[3], x, y);
				}
				else return a;
			}
			else return a;
		}
		else return a;
	}
	else return NULL;
}

double sqrdist(itemList* p1, itemList* p2) {
	return ((pow(p1->data->x - p2->data->x, 2) + pow(p1->data->y - p2->data->y, 2)));
}

int dnearest(QTree*t) {
	if (!t) return 1;
	if (!t->n) {
		printf("Tree is empty\n");
		return 1;
	}
	if (t->n == 1) {
		printf("There is only one point in the tree, it is alone...\n");
		return 1;
	}
	int x, y;
	printf("Enter x:-->");
	getInt(&x, -1);
	printf("Enter y:-->");
	getInt(&y, -1);
	nearestNeighbour(t, x, y);
	return 1;
}


int timing(QTree* t) {
	if (!t) return 2;
	QTree testTree = { 0, NULL, NULL };
	createTree(&testTree, NULL, INT_MIN, INT_MAX, INT_MIN, INT_MAX, 10);
	clock_t first, last;
	int n = 10, x[100000], y[100000], cnt = 400000, i, m;
	srand(time(NULL));
	while (n-- > 0) {
		for (i = 0; i < 100000; ++i) {
			x[i] = i;
			y[i] = i;
		}
		for (i = 0; i < cnt; ) {
			elem* te = (elem*)malloc(sizeof(elem));
			char* info = makeStr("text");
			te->x = rand();
			te->y = rand();
			te->info = info;
			if (!insert(&testTree, te)) ++i;
		}
		m = 0;
		first = clock();
		for (i = 0; i < 100000; i++) {
			itemList* ans;
			Node*r = testTree.root;
			search(&testTree, &r, &ans, x[i], y[i]);
			if (ans) m++;
		}
		last = clock();
		printf("%d items were found\n", m);
		printf("test #%d, number of nodes = %d, time = %d\n", 10 - n, (10 - n)*cnt, last - first);
	}
	delTree(&testTree);
	return 1;
}

int delNode(Node* r, int i) {
	if (!r) return 1;
	itemList* tmp = r->itemshead.next;
	while (tmp) {
		free(tmp->data->info);
		free(tmp->data);
		itemList* next = tmp->next;
		free(tmp);
		tmp = next;
	}
	if (r->child) for (int i = 3; i >= 0; i--) delNode(&(r->child[i]), i);
	if (!i) free(r);
	return 0;
}

int treeView(Node* r, int lvl) {
	if (!r) return 1;
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

int showXRange(QTree* t) {
	if (!t) return 2;
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
		printf("This area doesn\'t belong to the tree\n");
		return 1;
	}
	Node* tmp = t->root;
	rangeTraverse(tmp, xmin, xmax, ymin, ymax);
	return 0;
}

int rangeTraverse(Node* node, int xmin, int xmax, int ymin, int ymax) {
	if (!node) return 1;
	if (node->child == NULL) {
		itemList* tmp = node->itemshead.next;
		while (tmp) {
			if (tmp->data->x >= xmin && tmp->data->x <= xmax && tmp->data->y >= ymin && tmp->data->y <= ymax)
				printf("(%d; %d) %s\n", tmp->data->x, tmp->data->y, tmp->data->info);
			tmp = tmp->next;
		}
		return 0;
	}
	else {
		int qmin = quadrant(xmin, ymin, node->xmin, node->xmax, node->ymin, node->ymax, 0);
		int qmax = quadrant(xmax, ymax, node->xmin, node->xmax, node->ymin, node->ymax, 0);
		if (qmin == qmax) rangeTraverse(&node->child[qmin], xmin, xmax, ymin, ymax);
		else if (abs(qmin - qmax) == 2) {
			for (int i = 0; i < 4; i++) rangeTraverse(&node->child[i], xmin, xmax, ymin, ymax);
		}
		else {
			rangeTraverse(&node->child[qmin], xmin, xmax, ymin, ymax);
			rangeTraverse(&node->child[qmax], xmin, xmax, ymin, ymax);
		}
	}
	return 0;
}

int properties(QTree* t) {
	if (!t) return 1;
	printf("Busy: %d\n", t->n);
	printf("Node capacity: %d\n", t->N);
	printf("Filename: %s\n", t->filename);
	return 1;
}

itemList*** traverse(Node* node, itemList*** arrptr, int* n) {
	if (!node) return NULL;
	if (!arrptr) return NULL;
	if (!n) return NULL;
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
	if (!t) return 2;
	itemList** arrptr = (itemList**)malloc(t->n * sizeof(itemList*));
	int n = 0;
	traverse(t->root, &arrptr, &n);
	if (!n) {
		printf("Table is empty\n");
		return 1;
	}
	sort(&arrptr, n - 1);
	printf("X\tY\tInfo\n");
	for (int i = 0; i < n; i++) printf("%d\t%d\t%s\n", arrptr[i]->data->x, arrptr[i]->data->y, arrptr[i]->data->info);
	free(arrptr);
	return 0;
}

int showTree(QTree* t) {
	if (!t) return 2;
	Node* r = t->root;
	treeView(r, 0);
	return 1;
}

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
	if (!t) return 2;
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
	if (!t) return 2;
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
	if (!t) return 2;
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
	if (!t) return 1;
	Node* r = t->root;
	delNode(r, 0);
	if (t->filename) free(t->filename);
	return 0;
}

int linsert(elem* item, itemList* head) {
	if (!item) return 2;
	if (!head) return 3;
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

int quadrant(int x, int y, float xmin, float xmax, float ymin, float ymax, int mode = 1) {
	if (mode) if (x < xmin || x > xmax || y < ymin || y > ymax) return -1;
	if (x >= (xmax + xmin) / 2) {
		if (y >= (ymax + ymin) / 2) return 0;
		else return 1;
	}
	else {
		if (y >= (ymax + ymin) / 2) return 3;
		else return 2;
	}
}

int search(QTree* t, Node** node, itemList** answer, int x, int y) {
	if (!t) return 2;
	if (!node) return 3;
	while ((*node)->child) {
		int q = quadrant(x, y, (*node)->xmin, (*node)->xmax, (*node)->ymin, (*node)->ymax, 1);
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
		free(e->info);
		free(e);
		return 4;
	}
	Node *r = t->root;
	itemList* ans = NULL;
	search(t, &r, &ans, e->x, e->y);
	if (ans) {
		free(e->info);
		free(e);
		return 3;
	}
	while (r->busy == t->N) {
		divide(r);
		int q = quadrant(e->x, e->y, r->xmin, r->xmax, r->ymin, r->ymax, 1);
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
		parent->child[i].parent = parent;
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
		int q = quadrant(tmp->data->x, tmp->data->y, parent->xmin, parent->xmax, parent->ymin, parent->ymax, 1);
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
	if (!t) return 2;
	int rc = 1, xmin, xmax, ymin, ymax, N;
	char* fname = NULL;
	while (rc) {
		printf("Enter file name: --> ");
		fname = getStr(0);
		if (!fname) return 1;
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
	return 0;
}

int createTree(QTree* t, char* fname, int xmin, int xmax, int ymin, int ymax, int N) {
	if (!t) return 1;
	//if (!fname) return 2;
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
	t->root->parent = NULL;
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
	if (fname) if (load(t, fname)) createFile(t, fname);
	//if (!(fopen_s(&(t->fd), fname, "w+b"))) t->root = NULL;
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
	if (dload(&t)) return 1;
	while (rc = dialog(msgs, NMsgs)) if (!fptr[rc](&t)) break;
	dsave(&t);
	puts("Program finished");
	fclose(t.fd);
	delTree(&t);
	return 0;
}
