/*
координаты вещественые числа

вывод графа в виде списков смежности

1. Написать функцию считывания вещественных чисел (getDouble (double* t));
2. Создание графа:
- загрузка из файла
- задать вручную
- рандомная генерация


3. процесс работы

вывод возможностей загрузки графа
1. Из файла
	- структура файла???
2. Сгенерировать
	какие входные данные нужны для генерации?
	- (макс) число вершин
	- рёбра????
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

*/
#include "pch.h"
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#pragma warning(disable:4996)



struct Point {
	double x;
	double y;
	int name;
};

struct AdjList {
	Point data;
	Point* next;
};

struct Graph {
	AdjList* adjlist;
	int topcount;
	int edgecount;
};

const char* loadmsgs[] = { "0. Quit", "1. Load graph", "2. Generate graph", "3. Generate graph" };
const char* menu[] = { "0. Quit", "1. Add vertex", "2. Add edge", "3. Delete vertex", "4. Decompose graph", "5. Show adjacency lists", "6. Save graph", "7. Timing", "8. Graph properties" };

const int NLoadMsgs = sizeof(loadmsgs) / sizeof(loadmsgs[0]);

int main() {
	Graph g = { NULL, 0, 0 };
	int rc;
	while (rc = dialog(loadmsgs, NLoadMsgs)) if (!lfptr[rc](&t)) break;
}