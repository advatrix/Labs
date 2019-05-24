#include "pch.h"
#include <stdio.h>
#include <malloc.h>
#pragma warning(disable:4996)

typedef struct item {
	char c;
	struct item *next;
} item;

int getList(item **);
void putList(const char *, item *);
item *deleteList(item *);
item *delSpace(item *);
item *skipWord(item *);
item *delWord(item *);

int getList(item **pptr) {
	char buf[81], *str;
	item head = { '*', NULL };
	item *last = &head;
	int n, rc = 1;
	do {
		n = scanf("%80[^\n]", buf);
		if (n < 0) {
			deleteList(head.next);
			head.next = NULL;
			rc = 0;
			continue;
		}
		if (n > 0) {
			for (str = buf; *str != '\0'; ++str) {
				last->next = (item*)malloc(sizeof(item));
				last = last->next;
				last->c = *str;
			}
			last->next = NULL;
		}
		else scanf("%*c");
	} while (n > 0);
	*pptr = head.next;
	return rc;
}

void putList(const char *msg, item *ptr) {
	printf("%s: \"", msg);
	for (; ptr; ptr = ptr->next) {
		printf("%c", ptr->c);
	}
	printf("\"\n");
}

item *deleteList(item *ptr) {
	item *tmp = ptr;
	while (ptr) {
		tmp = ptr;
		ptr = ptr->next;
		free(tmp);
	}
	return ptr;
}
/*
item *reorg(item *p) {
	item head = { '\0', p },
		*last = &head,
		*prev = NULL,
		*tmp = NULL;
	int f = 0;
	while (last && (last->next = delSpace(last->next))) {
		while (last->next && (last->next->c == '(')) {
			f++;
			last->next->c = ' ';
			tmp = last->next;
			last->next = last->next->next;
			free(tmp);
		}
		while (last->next && (last->next->c == ')')) {
			f--;
			last->next->c = ' ';
			tmp = last->next;
			last->next = last->next->next;
			free(tmp);
		}
		if ((f > 0) && last->next) {
			prev = skipWord(last->next);
			last = prev->next;
			if (last) {
				if (last->c == ')') f--;
			}
		}
		else last->next = delWord(last->next);
	}
	if (last && prev) {
		prev->next = NULL;
		free(last);
	}
	tmp = &head;
	tmp = delSpace(tmp);
	while (tmp) {
		if ((tmp->c == ')') || (tmp->c == '(') || (tmp->c == '\t'))  tmp->c = ' ';
		if ((tmp->next) && (tmp->c == ' ') && (tmp->next->c == ' ')) tmp = delSpace(tmp);
		if (tmp) tmp = tmp->next;
	}
	while (head.next && head.next->c == ' ') {
		tmp = head.next;
		head.next = head.next->next;
		free(tmp);
	}
	return head.next;
}

*/
item *delSpace(item *p) {
	item *tmp = NULL;
	while (p && (p->c == ' ' || p->c == '\t')) {
		tmp = p;
		p = p->next;
		free(tmp);
	}
	return p;
}
/*
item *skipWord(item *p) {
	if (p) while (p->next && p->next->c != ' ' && p->next->c != '\t' && p->next->c !=')' && p->next->c != '(') p = p->next;
	return p;
}

item *delWord(item *p) {
	item *tmp;
	while (p && p->c != ' ' && p->c != '\t' && p->c != ')' && p->c != '(') {
		tmp = p;
		p = p->next;
		free(tmp);
	}
	return p;
}
*/

item *reorg(item *p) {
	item head = { '\0', p },
		*prev = &head,
		*tmp = head.next,
		*t = NULL;
	int f = 0, sp = 0;
	while (tmp) {
		if (tmp->c == '(') {
			f++;
			tmp->c = ' ';
		}
		else if (tmp->c == ')') {
			f--;
			tmp->c = ' ';
		}
		else if (tmp->c == '\t') tmp->c = ' ';
		if (f > 0) {
			tmp = tmp->next;
			prev = prev->next;
		}
		else {
			t = tmp;
			tmp = tmp->next;
			prev->next = tmp;
			free(t);
		}
	}
	prev = &head;
	tmp = head.next;
	while (tmp && (tmp->c == ' ')) {
		t = tmp;
		tmp = tmp->next;
		prev->next = tmp;
		free(t);
	}
	while (tmp && tmp->next) {
		if (tmp->c == ' ') sp++;
		else sp = 0;
		if (sp > 1) {
			t = tmp;
			tmp = tmp->next;
			prev->next = tmp;
			free(t);
			sp = 1;
		}
		else {
			tmp = tmp->next;
			prev = prev->next;
		}
	}
	while (tmp && tmp->next) {
		if (tmp->c == ' ') {
			t = tmp;
			tmp = tmp->next;
			prev->next = tmp;
			free(t);
		}
		else {
			tmp = tmp->next;
			prev = prev->next;
		}
	}
	if (tmp && tmp->c == ' ') {
		prev->next = NULL;
		t = tmp;
		free(tmp);
		if (prev->c == ' ') {
			tmp = head.next;
			while (tmp->next != prev) tmp = tmp->next;
			tmp->next = NULL;
			free(prev);
		}
	}
	return head.next;
}

int main() {
	item *st;
	do {
		puts("Enter string: ");
		if (!(getList(&st))) break;
		putList("Source string", st);
		st = reorg(st);
		putList("Result string", st);
		st = deleteList(st);
	} while (1);
	puts("Program finished");
	return 0;
}