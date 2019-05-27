/*
Дана последовательность из n натуральных чисел
Для каждого числа из исходной последовательности подсчитать количество и сумму цифр в записи числа
Сформировать две новых последовательности, состоящие соответственно из количества и суммы цифр в записи чисел,
стоящих на нечётных местах исходной последовательности.
Упорядочить полученные последовательности по возрастанию.
Вывести исходную и полученную последовательности на экран
*/

#include "pch.h"
#include <stdio.h>
#include <malloc.h>

int dgtcnt(int a) {
	int cnt = 0;
	while (a) {
		a /= 10;
		cnt++;
	}
	return cnt;
}

int dgtsum(int a) {
	int b, sum = 0;
	while (a) {
		b = a % 10;
		a /= 10;
		sum += b;
	}
	return sum;
}

int sort(int *a, int n) {
	int i;
	for (int j = 1; j < n; j++) {
		int key = a[j];
		i = j - 1;
		while (i > 0 && a[i] > key) {
			a[i + 1] = a[i];
			i--;
		}
		a[i + 1] = key;
	}
	return 0;
}

int printarr(int *a, int n) {
	for (int i = 0; i < n; i++) {
		printf("%d ", a[i]);
	}
	printf("\n");
	return 0;
}

int gp() {
	int t;
	while (1) {
		scanf_s("%d", &t);
		if (t > 0)
			break;
		printf("Wrong input: you should enter a positive integer. Try again!\n");
		scanf_s("%*c");
		scanf_s("%*[^\n]");
		scanf_s("%*s");
	}
	return t;
}

int main() {
	int n, *a, m = 1, q; 
	printf("Enter n: ");
	n = gp();
	a = (int*)malloc(n * sizeof(int));
	for (int i = 0; i < n; i++) {
		printf("Enter number %d: ", i + 1);
		a[i] = gp();
	}
	while (m) {
		printf("What to do? \n 0 - exit \n 1 - dgtcnt and sum \n 2 - create two sequences \n");
		scanf_s("%d", &m);
		switch (m) {
		case 1:
			for (int i = 0; i < n; i++)
				printf("number %i is %i, has %i digits total of %i \n",i+1, a[i], dgtcnt(a[i]), dgtsum(a[i]));
			break;
		case 2:
			int k;
			if (n % 2) {
				k = n / 2 + 1;
			}
			else {
				k = n / 2;
			}
			int *sum;
			sum = (int*)malloc(k * sizeof(int));
			int *cnt;
			cnt = (int*)malloc(k * sizeof(int));
			for (int i = 0; i < k; i++) {
				cnt[i] = dgtcnt(a[i * 2]);
				sum[i] = dgtsum(a[i * 2]);
			}
			sort(cnt, k);
			sort(sum, k);
			printf("Original sequence:\n");
			printarr(a, n);
			printf("Sorted sequence of digits counts of numbers in odd places:\n");
			printarr(sum, k);
			printf("Sorted sequence of digits sums of numbers in odd places:\n");
			printarr(cnt, k);
			free(sum);
			free(cnt);
			break;
		default:
			if (m) 
				printf("I don\'t understand you. Try again\n");
		}
	}
	free(a);
}
