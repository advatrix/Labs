/*
Для каждой строки сформировать новую строку, поместив в неё слова исходной строки, начинающиеся и заканчивающиеся на ту же букву,
что и первое слово в строке
*/

#include "pch.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#pragma warning(disable:4996)
int(*fptr[])(table*) = { NULL, add, find, del, show };
char *getstr();
char *reorg(char *s);
char *skipWord(char *s);
char *skipSpace(char *s);
char *trim(char *s);

char *skipSpace(char *s) {
	int k = strspn(s, " \t");
	return s + k;
}

char *skipWord(char *s) {
	int k = strcspn(s, " \t");
	return s + k;
}

char *getstr() {
	char *ptr = (char*)malloc(1);
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
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

char *trim(char *s) {
	int k = strspn(s, " \t") - 1;
	char *tmp1 = s;
	while(*tmp1) {
		*tmp1 = *(tmp1 + k + 1);
		tmp1++;
	}
	tmp1 = s + strlen(s) - 1;
	while (tmp1 != s) {
		if (*tmp1 == ' ' || *tmp1 == '\t') {
			*tmp1 = '\0';
			tmp1--;
		}
		else break;
	}
	char *tmp = s;
	while (*tmp) {
		k = strspn(tmp, " \t");
		if (k > 1) {
			tmp1 = tmp;
			while (*tmp1) {
				*tmp1 = *(tmp1 + k - 1);
				tmp1++;
			}
		}
		else tmp++;
	}
	return s;
}

char *delTabs(char *s) {
	char *tmp = s;
	while (*tmp) {
		if (*tmp == '\t') *tmp = ' ';
		tmp++;
	}
	return s;
}

char *reorg(char *s) {
	if (!(s)) return s;
	if (!(*(s))) return s;
	s = trim(s);
	char fl = *s;
	char ll = *(skipWord(s) - 1);
	char *tmp = s;
	int i = 0, j; 
	while (*(tmp)) {
		if (*(tmp) == fl && *(skipWord(tmp) - 1) == ll) {
			i += strcspn(tmp, " \t");
			tmp = skipWord(tmp);
			i += strspn(tmp, " \t");
			tmp = skipSpace(tmp);
		}
		else {
			int k = strcspn(tmp, " \t");
			char *tmp1 = tmp;
			for (j = i - 1; j < strlen(s); j++) {
				*tmp1 = *(tmp1 + k + 1);
				tmp1++;
			}
			*tmp1 = '\0';
		}
	}
	s = trim(s);
	s = delTabs(s);
	return s;
}

int main() {
	char *s = NULL;
	while (1) {
		puts("Enter string:");
		if (!(s = getstr())) break;
		printf("Source string: \"%s\"\n", s);
		s = reorg(s);
		printf("Result string: \"%s\"\n", s);
		free(s);
	}
	return 0;
}
