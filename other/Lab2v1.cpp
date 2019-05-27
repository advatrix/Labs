#include "pch.h"
#include <stdio.h>
#include <malloc.h>

int gi() {
	int t, r;
	do {
		r = scanf_s("%d", &t);
		if (r == 1) return t;
		printf("Input is incorrect. You should enter an integer. Try again.\n");
		scanf_s("%*[^\n]");
	} while (1);
}

int gp() {
	int t;
	do {
		t = gi();
		if (t > 0) return t;
		printf("Input is incorrect. You should enter a positive integer. Try again\n");
	} while (1);
}

int main() {
	int m, **a, *b, *n;
	printf("Enter m (the number of rows): ");
	do {
		m = gp();
		a = (int**)malloc(m * sizeof(int*));
		b = (int*)malloc(m * sizeof(int));
		n = (int*)malloc(m * sizeof(int));
		if (a == NULL || b == NULL || n == NULL) {
			printf("Value is too big");
			free(a);
			free(b);
			free(n);
		}
		else break;
	} while (1);
	for (int i = 0; i < m; i++) {
		while (1) {
			printf("Enter n[%d] (the number of elements in the row %d): ", i + 1, i + 1);
			n[i] = gp();
			a[i] = (int*)malloc(n[i] * sizeof(int));
			if (a[i] == NULL) {
				printf("Value is too big");
				free(a[i]);
			}
			else break;
		}
		int sum = 0;
		for (int j = 0; j < n[i]; j++) {
			printf("Enter a[%i][%i]: ", i + 1, j + 1);
			a[i][j] = gi();
			sum += a[i][j];
		}
		double avrg = sum / n[i];
		b[i] = 0;
		for (int j = 0; j < n[i]; j++) if (a[i][j] > avrg) b[i] += a[i][j];
	}
	printf("\n\nOriginal array:\n");
	for (int i = 0; i < m; i++, printf("\n")) for (int j = 0; j < n[i]; j++) printf("%d\t", a[i][j]);
	printf("\nVector {bi}:\n");
	for (int i = 0; i < m; i++) printf("%d\t", b[i]);
	for (int i = 0; i < m; i++) free(a[i]);
	free(a);
	free(n);
	free(b);
	return 0;
}