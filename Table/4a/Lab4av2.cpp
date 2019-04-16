#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#pragma warning(disable:4996)

const int SIZE = 10;

typedef struct item {
	int i1;
	int key2;
	char* info;
} item;

typedef struct keyItem {
	int k;
	item* elem;
};

typedef struct table {
	int n;
	keyItem* ks1;
	keyItem* ks2;
};

typedef struct olditem {
	int key1;
	int key2;
	char* info;
} olditem;

const char *msgs[] = { "0. Quit", "1. Add new element", "2. Find an element", "3. Delete an element", "4. Show a table" };
const char *errmsgs[] = { "Ok", "Error: Duplicate key 1", "Error: Duplicate key 2", "Error: Table overflow" };
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
const char *errmsgsdel[] = { "Ok", "Error: item not found" , "Table nullpointer", "Wrong keyspace", "Table is empty"};

int add(table *);
int find(table *);
int del(table *);
int show(table *);
int getInt(int *t);
int search1(table *t, int key);
int search2(table *t, int key);
int insert(table* t, int key1, int key2, char* info);
int insert1(table* t, int key1, item* elem);
int insert2(table* t, item* elem);
int rmv(table*t, int ks, int key);
char *getStr();
olditem search(table*t, int *key1, int* key2);

int(*fptr[])(table*) = { NULL, add, find, del, show };

int add(table* t) {
	if (!t) return 0;
	int k1, k2, rc;
	char* info = NULL;
	if (t->n == SIZE) rc = 3;
	else {
		printf("Enter key1: --> ");
		getInt(&k1);
		printf("Enter key2: --> ");
		getInt(&k2);
		printf("Enter info: --> ");
		info = getStr();
		rc = insert(t, k1, k2, info);
	}
	printf("%s\n", errmsgs[rc]);
	return 1;
}

int search1(table* pt, int key) {
	if (!pt) return -1;
	for (int i = 0; i < (pt->n); i++) if (pt->ks1[i].k == key) return i;
	return -1;
}

int search2(table* pt, int key) {
	if (!pt) return -1;
	int i = 0, m = (pt->n) - 1, j;
	while (i <= m) {
		j = (i + m) / 2;
		if ((pt->ks2[j].k) == key) return j;
		else if ((pt->ks2[j].k > key)) m = j - 1;
		else i = j + 1;
	}
	return -1;
}

olditem search(table*t, int *key1, int* key2) {
	olditem answer = { 0, 0, NULL };
	if (!t) return answer;
	if (key1 && key2) {
		int i1 = search1(t, *key1), i2 = search2(t, *key2);
		if (i1 != -1 && i2 != -1 && t->ks1[i1].elem == t->ks2[i2].elem) {
			answer.key1 = *key1;
			answer.key2 = *key2;
			answer.info = t->ks1[i1].elem->info;
		}
	}
	else if (key1 && !key2) {
		int i1 = search1(t, *key1);
		if (i1 != -1) {
			answer.key1 = *key1;
			answer.key2 = t->ks1[i1].elem->key2;
			answer.info = t->ks1[i1].elem->info;
		}
	}
	else if (!key1 && key2) {
		int i2 = search2(t, *key2);
		if (i2 != -1) {
			answer.key1 = t->ks1[t->ks2[i2].elem->i1].k;
			answer.key2 = *key2;
			answer.info = t->ks2[i2].elem->info;
		}
	}
	return answer;
}

int find(table *t) {
	if (!t) return 0;
	int k1, k2, rc, a, f = 1;
	char *info = NULL;
	olditem answer = { 0, 0, NULL };
	while (f) {
		puts("1 - Find elem with key 1");
		puts("2 - Find elem with key 2");
		puts("3 - Find elem with both key 1 and key 2");
		getInt(&a);
		switch (a) {
		case 1: {
			printf("Enter key 1: --> ");
			getInt(&k1);
			answer = search(t, &k1, NULL);
			f--;
		} break;
		case 2: {
			printf("Enter key 2: --> ");
			getInt(&k2);
			answer = search(t, NULL, &k2);
			f--;
		} break;
		case 3: {
			printf("Enter key 1: --> ");
			getInt(&k1);
			printf("Enter key 2: --> ");
			getInt(&k2);
			answer = search(t, &k1, &k2);
			f--;
		} break;
		default: {
			puts("I don't understand you!");
			continue;
		}
		}
	}
	if (!a) return 1;
	if (answer.info) printf("Key1\tKey2\tInfo\n%d\t%d\t%s\n", answer.key1, answer.key2, answer.info);
	else puts("There is no item with this key! :(");
	return 1;
}

int insert1(table* t, int key1, item* elem) {
	if (!t) return 0;
	t->ks1[t->n].k = key1;
	t->ks1[t->n].elem = elem;
	return 1;
}

int insert2(table* t, item* elem) {
	if (!t) return 0;
	int i = t->n - 1;
	while (i >= 0 && (t->ks2[i].k > elem->key2)) {
		t->ks2[i + 1] = t->ks2[i];
		i--;
	}
	t->ks2[i + 1].k = elem->key2;
	t->ks2[i + 1].elem = elem;
	return 1;
}

int insert(table* t, int key1, int key2, char* info) {
	if (!t) return 4;
	if (t->n == SIZE) return 3;
	int i1;
	if (i1 = search1(t, key1) != -1) return 1;
	if (search2(t, key2) != -1) return 2;
	item* elem = (item*)malloc(sizeof(item));
	*elem = { t->n , key2, info };
	insert1(t, key1, elem);
	insert2(t, elem);
	(t->n)++;
	return 0;
}

int dialog(const char*msgs[], int N) {
	int rc;
	int i, n;
	do {
		for (i = 0; i < N; ++i) puts(msgs[i]);
		puts("Make your choice: --> ");
		n = getInt(&rc);
		if (!(n)) rc = 0;
		if (rc >= 0 && rc < N) continue;
		puts("I don't understand you. Please try again.");
	} while (rc < 0 || rc >= N);
	return rc;
}

int rmv(table*t, int ks, int key) {
	if (!t) return 2;
	int i1, i2;
	if (ks == 1) {
		i1 = search1(t, key);
		if (i1 == -1) return 1;
		i2 = search2(t, t->ks1[i1].elem->key2);
	}
	else if (ks == 2) {
		i2 = search2(t, key);
		if (i2 == -1) return 1;
		i1 = t->ks2[i2].elem->i1;
	}
	else return 3;
	item* tmp = t->ks2[i2].elem;
	(t->ks1)[i1] = (t->ks1)[(t->n) - 1];
	(t->ks1)[i1].elem->i1 = i1;
	for (int j = i2; j < (t->n) - 1; j++) (t->ks2)[j] = (t->ks2)[j + 1];
	(t->n)--;
	free(tmp);
	return 0;
}

int del(table *t) {
	if (!t) return 1;
	int key, ks, i1, i2, rc = 4;
	while (t->n) {
		puts("1 - Key space 1");
		puts("2 - Key space 2");
		getInt(&ks);
		if (ks == 1) {
			printf("Enter key 1 --> ");
			getInt(&key);
			rc = rmv(t, 1, key);
			break;
		}
		else if (ks == 2) {
			printf("Enter key 2 --> ");
			getInt(&key);
			rc = rmv(t, 2, key);
			break;
		}
		else continue;
	}
	printf("%s\n", errmsgsdel[rc]);
	return 1;
}

int getInt(int *t) {
	int r;
	do {
		r = scanf_s("%d", t);
		if (r == 1) return *t;
		printf("Input is incorrect. You should enter an integer. Try again.\n");
		scanf_s("%*[^\n]");
	} while (1);
}

char *getStr() {
	char *ptr = (char*)malloc(sizeof(char));
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
	scanf_s("%*c");
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

int show(table *pt) {
	if (pt->n == 0) printf("Table is empty\n");
	else {
		printf("Key1\tKey2\tInfo\n");
		for (int i = 0; i < (pt->n); i++) {
			printf("%d\t%d\t%s\n", pt->ks1[i].k, pt->ks1[i].elem->key2, (pt->ks1[i].elem->info));
		}
	}
	return 1;
}

int delTable(table *pt) {
	for (int i = 0; i < pt->n; i++) {
		free(pt->ks1[i].elem->info);
		free(pt->ks1[i].elem);
	}
	(pt->n) = 0;
	return 1;
}

int main() {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	int rc = 1;
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
//	while (rc = dialog(msgs, NMsgs)) if (!fptr[rc](&t)) break;
/*	while (1) {
		table t = { 0, ks1, ks2 };
		for (int i = 0; i < 5; i++) {
			char buf[] = "info";
			char *s = (char*)malloc(5);
			*s = '\0';
			strcat(s, buf);
			insert(&t, i + 1, i + 3, s);
		}
		delTable(&t);
	}*/
	while (1) {
		int i = 1;
		insert(&t, i, i + 1, s);
		rmv(&t, 2, i + 1);
		
	}
	puts("Program finished");
	delTable(&t);
	return 0;
}