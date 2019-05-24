#include "pch.h"
#include <tchar.h>
#include "source.cpp"

TEST(EmptyTable, Insert) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	ASSERT_EQ(insert(&t, 12, 16, s), 0);
	ASSERT_EQ(t.n, 1);
	ASSERT_EQ(t.ks1[0].k, 12);
	ASSERT_EQ(t.ks2[0].k, 16);
	ASSERT_EQ(t.ks1[0].elem->info, s);
	free(s);
}

TEST(EmptyTable, FindKey1) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	int key = 12;
	olditem answer = search(&t, &key, NULL);
	int a = (answer.info == NULL) ? 1 : 0;
	ASSERT_EQ(a, 1);
}

TEST(EmptyTable, FindKey2) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	int key = 12;
	olditem answer = search(&t, NULL, &key);
	int a = (answer.info == NULL) ? 1 : 0;
	ASSERT_EQ(a, 1);
}

TEST(EmptyTable, FindBoth) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	int key1 = 12;
	int key2 = 17;
	olditem answer = search(&t, &key1, &key2);
	int a = (answer.info == NULL) ? 1 : 0;
	ASSERT_EQ(a, 1);
}

TEST(EmptyTable, Delete) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	ASSERT_EQ(rmv(&t, 1, 5), 1);
	ASSERT_EQ(rmv(&t, 2, 11), 1);
}

TEST(Table1, Search1) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	insert(&t, 12, 16, s);
	int key = 12;
	olditem answer = search(&t, &key, NULL);
	ASSERT_EQ(answer.info, s);
	free(s);
}

TEST(Table1, Search1Absent) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	item elem = { 12, 16, s };
	insert(&t, 12, 16, s);
	int key = 148;
	olditem answer = search(&t, &key, NULL);
	int a = (answer.info == NULL) ? 1 : 0;
	ASSERT_EQ(a, 1);
	free(s);
}

TEST(Table1, Search2) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	item elem = { 12, 16, s };
	insert(&t, 12, 16, s);
	int key = 16;
	olditem answer = search(&t, NULL, &key);
	ASSERT_EQ(answer.info, s);
	free(s);
}

TEST(Table1, Search2Absent) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	item elem = { 12, 16, s };
	insert(&t, 12, 16, s);
	int key = 263;
	olditem answer = search(&t, NULL, &key);
	int a = (answer.info == NULL) ? 1 : 0;
	ASSERT_EQ(a, 1);
	free(s);
}

TEST(Table1, SearchBoth) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	insert(&t, 12, 16, s);
	int key1 = 12;
	int key2 = 16;
	olditem answer = search(&t, &key1, &key2);
	ASSERT_EQ(answer.info, s);
	free(s);
}

TEST(Table1, SearchBothAbsent) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	insert(&t, 12, 16, s);
	int key1 = 291;
	int key2 = 717;
	olditem answer = search(&t, &key1, &key2);
	int a = (answer.info == NULL) ? 1 : 0;
	ASSERT_EQ(a, 1);
	free(s);
}

TEST(Table1, SearchBothAbsent1) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	insert(&t, 12, 16, s);
	int key1 = 291;
	int key2 = 16;
	olditem answer = search(&t, &key1, &key2);
	int a = (answer.info == NULL) ? 1 : 0;
	ASSERT_EQ(a, 1);
	free(s);
}

TEST(Table1, SearchBothAbsent2) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	insert(&t, 16, 12, s);
	int key1 = 12;
	int key2 = 987;
	olditem answer = search(&t, &key1, &key2);
	int a = (answer.info == NULL) ? 1 : 0;
	ASSERT_EQ(a, 1);
	free(s);
}

TEST(Table1, Insert1AbsentBoth) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	insert(&t, 12, 16, s);
	char buf2[] = "ofni";
	char *r = (char*)malloc(5);
	*r = '\0';
	strcat(r, buf);
	insert(&t, 17, 97, r);
	ASSERT_EQ(t.ks1[1].elem->info, r);
	ASSERT_EQ(t.ks2[1].elem->info, r);
	ASSERT_EQ(t.n, 2);
	free(s);
	free(r);
}

TEST(Table1, InsertDuplicate1) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	insert(&t, 12, 16, s);
	char buf2[] = "ofni";
	char *r = (char*)malloc(5);
	*r = '\0';
	strcat(r, buf);
	ASSERT_EQ(insert(&t, 12, 97, r), 1);
	ASSERT_EQ(t.n, 1);
	free(s);
	free(r);
}

TEST(Table1, InsertDuplicate2) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	insert(&t, 12, 16, s);
	char buf2[] = "ofni";
	char *r = (char*)malloc(5);
	*r = '\0';
	strcat(r, buf);
	ASSERT_EQ(insert(&t, 21, 16, r), 2);
	ASSERT_EQ(t.n, 1);
	free(s);
	free(r);
}


TEST(Table5, InsertLow2) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	for (int i = 0; i < 5; i++) insert(&t, i+4, i*2+3, s);
	insert(&t, 17, 1, s);
	ASSERT_EQ(t.ks2[0].k, 1);
	free(s);
}


TEST(Table5, InsertHigh2) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	for (int i = 0; i < 5; i++) insert(&t, i+4, i+1, s);
	insert(&t, 38, 147, s);
	ASSERT_EQ(t.ks2[t.n - 1].k, 147);
	free(s);
}

TEST(Table5, Delete1) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	for (int i = 0; i < 5; i++) insert(&t, i+4, i+1, s);
	ASSERT_EQ(rmv(&t, 1, 8), 0);
	ASSERT_EQ(t.n, 4);
	ASSERT_EQ(search1(&t, 8), -1);
	free(s);
}

TEST(Table5, Delete2) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	for (int i = 0; i < 5; i++) insert(&t, i + 4, i + 1, s);
	ASSERT_EQ(rmv(&t, 2, 4), 0);
	ASSERT_EQ(t.n, 4);
	ASSERT_EQ(search2(&t, 4), -1);
	free(s);
}

TEST(FullTable, Insert) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	for (int i = 0; i < SIZE; i++) insert(&t, i + 4, i + 1, s);
	ASSERT_EQ(insert(&t, 64, 75, s), 3);
	free(s);
}

TEST(TableNullPtr, Insert) {
	table *t = NULL;
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	ASSERT_EQ(insert(t, 5, 116, s), 4);
	free(s);
}

TEST(TableNullPtr, Search) {
	table *t = NULL;
	int key1 = 16;
	int key2 = 48;
	int a = (search(t, &key1, &key2).info == NULL) ? 1 : 0;
	ASSERT_EQ(a, 1);
	ASSERT_EQ(search1(t, key1), -1);
	ASSERT_EQ(search1(t, key2), -1);
}

TEST(TableNullPtr, Delete) {
	table*t = NULL;
	ASSERT_EQ(del(t), 1);
}


TEST(InfoNullPtr, Insert) {
	keyItem ks1[SIZE], ks2[SIZE];
	table t = { 0, ks1, ks2 };
	char buf[] = "info";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	for (int i = 0; i < 5; i++) insert(&t, i + 1, i + 5, s);
	ASSERT_EQ(insert(&t, 97, 151, NULL), 0);
	ASSERT_EQ(t.ks2[t.n - 1].k, 151);
	ASSERT_EQ(t.n, 6);
	free(s);
}

int _tmain(int argc, _TCHAR* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
