// Lab1v1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

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
			printf("Sorted sequence of odd placed numbers digits counts:\n");
			printarr(sum, k);
			printf("Sorted sequence of odd placed numbers digits sums:\n");
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

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
