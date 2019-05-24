#include "pch.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#pragma warning(disable:4996)

struct item {
	int key1;
	int ind1;
	int key2;
	int offset;
	int len;
};

struct fitem {
	int key1;
	int key2;
	char* info;
};

struct keySpace {
	int k;
	item* elem;
};

struct table {
	int SZ;
	int n;
	keySpace* ks1;
	keySpace* ks2;
	FILE *fd;
	char* fname;
};

const char *msgs[] = { "0. Quit", "1. Add new element", "2. Find an element", "3. Delete an element", "4. Show table", "5. Table properties", "6. Garbage collect" };
const char *errmsgs[] = { "Ok", "Error: Duplicate key 1", "Error: Duplicate key 2", "Error: Table overflow", "Table nullpointer", "Data nullptr" };
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);
const char *errmsgsdel[] = { "Ok", "Error: item not found" , "Table nullpointer", "Wrong keyspace" };

int add(table *);
int find(table *);
int del(table *);
int show(table *);
int getInt(int *t);
int insert(table *pt, item* elem);
int insert1(table* pt, item* elem);
int insert2(table* pt, item* elem);
int search1(table* pt, int key);
int search2(table* pt, int key);
int add(table *pt);
int dialog(const char*msgs[], int N);
int load(table *t, char *fname);
int create(table *t, char *fname, int sz);
int dload(table* t);
int save(table* t);
int dsave(table *t);
int delTable(table *pt);
int rmv(table *t, int space, int key);
int properties(table *t);
char* getStr(int mode);
char* findInfo(table t, int* key1, int* key2);
fitem search(table* t, int* key1, int* key2);
int garbageCollector(table *t);
int dgc(table* t);
long int ftell1(table *t);


int(*fptr[])(table*) = { NULL, add, find, del, show, properties, dgc };

long int ftell1(table *t) {
	long int f = ftell(t->fd);
	if (f == -1) printf("Incorrect position\n");
	return f;
}

int dgc(table*t) {
	int rc = garbageCollector(t);
	if (rc) printf("Ok\n");
	else printf("Nullptr file descriptor\n");
	return 1;
}

int properties(table *t) {
	if (!t) return 0;
	printf("Max size: %d\n", t->SZ);
	printf("Busy: %d\n", t->n);
	printf("File: %s\n", t->fname);
	long int pos = ftell(t->fd);
	if (pos == -1) return 0;
	fseek(t->fd, 0, SEEK_END);
	long int sz = ftell(t->fd);
	if (sz == -1) {
		printf("incorrect file position\n");
		return 0;
	}
	printf("File size: %ld\n", sz);
	fseek(t->fd, pos, SEEK_SET);
	return 1;
}

int delTable(table* pt) {
	if (!pt) return 0;
	for (int i = 0; i < pt->n; i++) free(pt->ks1[i].elem);
	free(pt->ks1);
	free(pt->ks2);
	free(pt->fname);
	(pt->n) = 0;
	return 1;
}

int dsave(table *t) {
	if (save(t)) {
		printf("Saved successfully\n");
		return 1;
	}
	else return 0;
}

int save(table *t) {
	if (!t) return 0;
	fseek(t->fd, sizeof(int), SEEK_SET);
	fwrite(&t->n, sizeof(int), 1, t->fd);
	for (int i = 0; i < t->n; i++) fwrite(t->ks2[i].elem, sizeof(item), 1, t->fd);
	fclose(t->fd);
	t->fd = NULL;
	return 1;
}

int dload(table *t) {
	int sz = 0, l = -1;
	char *fname = NULL;
	while (l == -1) {
		printf("Enter file name: -->");
		fname = getStr(0);
		if (!(fname)) return 0;
		l = load(t, fname);
		if (l == -1) printf("File is too big to read\n");
	} 
	if (!l) {
		while (!sz) {
			printf("Enter possible vector size: -->");
			getInt(&sz);
			if (!sz) {
				printf("Zero vector size, are you joking?\n");
				continue;
			}
			if (create(t, fname, sz) == -1) {
				printf("Too big size\n");
				sz = 0;
			}
		}
	}
	return 1;
}

int create(table *t, char *fname, int sz) {
	t->SZ = sz;
	t->n = 0;
	if (!fname) return 0;
	t->fname = fname;
	if (fopen_s(&(t->fd), fname, "w+b")) {
		t->ks1 = NULL;
		t->ks2 = NULL;
		return 0;
	}
	t->ks1 = (keySpace*)calloc(t->SZ, sizeof(keySpace));
	t->ks2 = (keySpace*)calloc(t->SZ, sizeof(keySpace));
	if (!(t->ks2 && t->ks1)) {
		free(t->ks1);
		free(t->ks2);
		fclose(t->fd);
		return -1;
	}
	item *elem = (item*)calloc(1, sizeof(item));
	fwrite(&t->SZ, sizeof(int), 1, t->fd);
	fwrite(&t->n, sizeof(int), 1, t->fd);
	for (int i = 0; i < t->SZ; i++) fwrite(elem, sizeof(item), 1, t->fd);
	free(elem);
	return 1;
}

char* makeStr(const char buf[]) {
	char *s = (char*)malloc(strlen(buf) + 1);
	*s = '\0';
	strcat(s, buf);
	return s;
}

int garbageCollector(table *t) {
	if (!(t->fd)) return 0;
	FILE *oldf = t->fd, *newf = NULL;
	fopen_s(&newf, "tmp.dat", "w+b");
	fseek(newf, 0, SEEK_SET);
	fwrite(&(t->SZ), sizeof(int), 1, newf);
	fwrite(&(t->n), sizeof(int), 1, newf);
	item *empty_elem = (item*)calloc(1, sizeof(item));
	for (int i = 0; i < t->SZ; i++) fwrite(empty_elem, sizeof(item), 1, newf);
	free(empty_elem);
	for (int i = 0; i < t->n; i++) {
		char *tmpinfo = (char*)malloc(t->ks2[i].elem->len);
		fseek(t->fd, t->ks2[i].elem->offset, SEEK_SET);
		fread(tmpinfo, sizeof(char), t->ks2[i].elem->len, oldf);
		fseek(newf, 0, SEEK_END);
		if (ftell(newf) == -1) {
			printf("incorrect file position");
			return 0;
		}
		t->ks2[i].elem->offset = ftell(newf);
		fwrite(tmpinfo, sizeof(char), t->ks2[i].elem->len, newf);
		free(tmpinfo);
	}
	t->fd = newf;
	fseek(t->fd, 2 * sizeof(int), SEEK_SET);
	for (int i = 0; i < t->n; i++) fwrite(t->ks2[i].elem, sizeof(item), 1, t->fd);
	fclose(oldf);
	remove(t->fname);
	fclose(t->fd);
	rename("tmp.dat", t->fname);
	fopen_s(&(t->fd), t->fname, "r+b");
	return 1;
}

int load(table *t, char *fname) {
	if (!t) return 0;
	if (!fname) return 0;
	fopen_s(&(t->fd), fname, "r+b");
	if (!(t->fd)) return 0;
	t->fname = fname;
	fread(&t->SZ, sizeof(int), 1, t->fd);
	t->ks1 = (keySpace*)calloc(t->SZ, sizeof(keySpace));
	t->ks2 = (keySpace*)calloc(t->SZ, sizeof(keySpace));
	if (!(t->ks1 && t->ks2)) {
		free(t->ks1);
		free(t->ks2);
		fclose(t->fd);
		return -1;
	}
	fread(&t->n, sizeof(int), 1, t->fd);
	for (int i = 0; i < t->n; i++) {
		item* tmp = (item*)malloc(sizeof(item));
		t->ks2[i].elem = tmp;
		fread(t->ks2[i].elem, sizeof(item), 1, t->fd);
		t->ks2[i].k = t->ks2[i].elem->key2;
		t->ks1[i].elem = t->ks2[i].elem;
		t->ks1[i].elem->ind1 = i;
		t->ks1[i].k = t->ks2[i].elem->key1;
	}
	return 1;
}

int rmv(table *t, int space, int key) {
	if (!t) return 2;
	int i1, i2;
	if (space == 1) {
		i1 = search1(t, key);
		if (i1 == -1) return 1;
		i2 = search2(t, t->ks1[i1].elem->key2);
	}
	else if (space == 2) {
		i2 = search2(t, key);
		if (i2 == -1) return 1;
		i1 = t->ks2[i2].elem->ind1;
	}
	else return 3;
	item* tmp = t->ks2[i2].elem;
	t->ks1[i1] = t->ks1[(t->n) - 1];
	t->ks1[i1].elem->ind1 = i1;
	for (int j = i2; j < (t->n) - 1; j++) (t->ks2)[j] = (t->ks2)[j + 1];
	(t->n)--;
	free(tmp);
	return 0;
}

char* findInfo(table t, int* key1, int* key2) {
	if (!&t) return NULL;
	int offset, len, i;
	if (key2) {
		i = search2(&t, *key2);
		if (i < 0) return NULL;
		offset = t.ks2[i].elem->offset;
		len = t.ks2[i].elem->len;
	}
	else if (key1) {
		i = search1(&t, *key1);
		if (i < 0) return NULL;
		offset = t.ks1[i].elem->offset;
		len = t.ks1[i].elem->len;
	}
	else return NULL;
	char* info = NULL;
	if (i >= 0) {
		info = (char*)malloc(len);
		fseek(t.fd, offset, SEEK_SET);
		fread(info, sizeof(char), len, t.fd);
	}
	return info;
}

int show(table *t) {
	if (!t) return 0;
	if (!(t->n)) printf("Table is empty\n");
	else {
		printf("Key1\tKey2\tInfo\n");
		for (int i = 0; i < t->n; i++) {
			int len = t->ks1[i].elem->len;
			int offset = t->ks1[i].elem->offset;
			char* info = (char*)malloc(len);
			fseek(t->fd, offset, SEEK_SET);
			fread(info, sizeof(char), len, t->fd);
			printf("%d\t%d\t%s\n", t->ks1[i].k, t->ks1[i].elem->key2, info);
		}
	}
	return 1;
}

int del(table *t) {
	if (!t) return 1;
	int key, ks, rc = 4;
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

fitem search(table *t, int *key1, int* key2) {
	fitem answer = { 0, 0, NULL };
	if (!t) return answer;
	char *info = findInfo(*t, key1, key2);
	if (info) {
		if (key2) {
			int i = search2(t, *key2);
			if (i >= 0) answer = { t->ks2[i].elem->key1, *key2, info };
		}
		else {
			int i = search1(t, *key1);
			if (i >= 0) answer = { *key1, t->ks1[i].elem->key2, info };
		}
	}
	return answer;
}

int find(table* t) {
	if (!t) return 0;
	int k1, k2, a, f = 1;
	char *info = NULL;
	fitem answer = { 0, 0, NULL };
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

int getInt(int *t) {
	int r;
	do {
		r = scanf_s("%d", t);
		if (r == 1) return r;
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

int insert2(table *pt, item* elem) {
	if (!pt) return -2;
	if (!elem) return -3;
	if (search2(pt, elem->key2) == -1) {
		int i = pt->n - 1;
		while (i >= 0 && (pt->ks2[i].k > elem->key2)) {
			pt->ks2[i + 1] = pt->ks2[i];
			i--;
		}
		pt->ks2[i + 1].k = elem->key2;
		pt->ks2[i + 1].elem = elem;
		return 1;
	}
	else return -1;
}

int insert1(table *pt, item* elem) {
	pt->ks1[pt->n].k = elem->key1;
	pt->ks1[pt->n].elem = elem;
	return 1;
}

int insert(table* ptab, item* elem) {
	if (!ptab) return 4;
	if (((ptab)->n) >= ptab->SZ) return 3;
	if (search1(ptab, elem->key1) != -1) return 1;
	int k = insert2(ptab, elem);
	if (k == -1) return 2;
	else if (k < -1) return 5;
	else {
		insert1(ptab, elem);
		((ptab)->n)++;
	}
	return 0;
}

int add(table *ptab) {
	if (!ptab) return 0;
	int k1, k2, rc;
	char *info = NULL;
	if (ptab->n >= ptab->SZ) rc = 3;
	else {
		printf("Enter key1: --> ");
		getInt(&k1);
		printf("Enter key2: --> ");
		getInt(&k2);
		printf("Enter info: --> ");
		info = getStr();
		if (!info) return 0;
		item* elem = (item*)malloc(sizeof(item));
		fseek(ptab->fd, 0, SEEK_END);
		if (ftell(ptab->fd) == -1) {
			printf("incorrect file position\n");
			return 0;
		}
		*elem = { k1, ptab->n,  k2, ftell(ptab->fd), (int)strlen(info) + 1 };
		rc = insert(ptab, elem);
		fwrite(info, sizeof(char), ptab->ks1[(ptab->n) - 1].elem->len, ptab->fd);
		free(info);
	}
	printf("%s\n", errmsgs[rc]);
	return 1;
}

int memoryTest(table *t) {
	while (1) {
		int i = 1;
		char buf[] = "info";
		char *s = (char*)malloc(5);
		*s = '\0';
		strcat(s, buf);
		item* elem = (item*)malloc(sizeof(item));
		fseek(t->fd, 0, SEEK_END);
		*elem = { i+2, t->n, i+4, ftell(t->fd), (int)strlen(s) + 1 };
		insert(t, elem);
		fwrite(s, sizeof(char), t->ks1[(t->n) - 1].elem->len, t->fd);
		free(s);
		rmv(t, 2, i+4);
	}/*
	while (1) {
		char buffile[] = "file26.dat";
		char *filename = (char*)malloc(11);
		*filename = '\0';
		int sz = 0;
		strcat(filename, buffile);
		if (!(load(t, filename))) {
			while (!sz) {
				printf("Enter possible vector size: -->");
				getInt(&sz);
				if (!sz) printf("Zero vector size, are you joking?\n");
			}
			create(t, filename, sz);
		}
		for (int i = 0; i < 5; i++) {
			char buf[] = "info";
			char *s = (char*)malloc(5);
			*s = '\0';
			strcat(s, buf);
			item* elem = (item*)malloc(sizeof(item));
			fseek(t->fd, 0, SEEK_END);
			*elem = { i + 2, t->n, i + 4, ftell(t->fd), (int)strlen(s) + 1 };
			insert(t, elem);
			fwrite(s, sizeof(char), t->ks1[(t->n) - 1].elem->len, t->fd);
			free(s);
		}
		delTable(t);
	}*/
	return 1;
}

int main() {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	int rc;
	if (!(dload(&t))) return 1;
	while (rc = dialog(msgs, NMsgs)) if (!fptr[rc](&t)) break;
	//memoryTest(&t);
	dsave(&t);
	puts("Program finished!");
	delTable(&t);
	return 0;
}