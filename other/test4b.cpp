#include "pch.h"
#include "..\Lab4bv2\Lab4bv2.cpp"
#include <tchar.h>


TEST(FileNameNullPtr, load) {
	char *fname = NULL;
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	ASSERT_EQ(load(&t, fname), 0);
}

TEST(FileNameNullPtr, create) {
	char *fname = NULL;
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	ASSERT_EQ(create(&t, fname, 10), 0);
}

TEST(NewFile, load) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file.dat");
	ASSERT_EQ(load(&t, fname), 0);
	free(fname);
}

TEST(NewFile, createAnother) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file.dat");
	ASSERT_EQ(create(&t, fname, 15), 1);
	save(&t);
	delTable(&t);
}

TEST(NewFile, create) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("newfile.dat");
	ASSERT_EQ(create(&t, fname, 15), 1);
	ASSERT_STREQ(t.fname, "newfile.dat");
	delTable(&t);
}

TEST(EmptyFile, rmv) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file.dat");
	load(&t, fname);
	ASSERT_EQ(rmv(&t, 1, 14), 1);
	delTable(&t);
}

TEST(EmptyFile, insert) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	create(&t, fname, 15);
	//	load(&t, fname);
	char *info = makeStr("info");
	item* elem = (item*)malloc(sizeof(item));
	fseek(t.fd, 0, SEEK_END);
	*elem = { 11, t.n,  83, ftell(t.fd), (int)strlen(info) + 1 };
	insert(&t, elem);
	fwrite(info, sizeof(char), t.ks1[(t.n) - 1].elem->len, t.fd);
	free(info);
	ASSERT_EQ(t.n, 1);
	ASSERT_EQ(t.SZ, 15);
	ASSERT_EQ(t.ks1[0].k, 11);
	save(&t);
	delTable(&t);
}

TEST(File1, properties1) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	ASSERT_EQ(t.SZ, 15);
	ASSERT_EQ(t.n, 1);
	ASSERT_EQ(t.ks1[0].k, 11);
	save(&t);
	delTable(&t);
}

TEST(File1, fill) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	for (int i = 0; i < t.SZ; i++) {
		char *info = makeStr("info");
		item* elem = (item*)malloc(sizeof(item));
		fseek(t.fd, 0, SEEK_END);
		*elem = { i, t.n,  i + 6, ftell(t.fd), (int)strlen(info) + 1 };
		insert(&t, elem);
		fwrite(info, sizeof(char), t.ks1[(t.n) - 1].elem->len, t.fd);
		free(info);
	}
	ASSERT_EQ(t.n, t.SZ);
	ASSERT_EQ(t.n, 15);
	save(&t);
	delTable(&t);
}

TEST(File1, FindKey1) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	int key1 = 7;
	fitem answer = search(&t, &key1, NULL);
	ASSERT_EQ(answer.key1, 7);
	ASSERT_EQ(answer.key2, 13);
	save(&t);
	delTable(&t);
}

TEST(File1, FindKey2) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	int key2 = 8;
	fitem answer = search(&t, NULL, &key2);
	ASSERT_EQ(answer.key1, 2);
	ASSERT_EQ(answer.key2, 8);
	save(&t);
	delTable(&t);
}

TEST(File1, FindKeyBoth) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	int key1 = 2;
	int key2 = 8;
	fitem answer = search(&t, &key1, &key2);
	ASSERT_EQ(answer.key1, 2);
	ASSERT_EQ(answer.key2, 8);
	save(&t);
	delTable(&t);
}

TEST(File1, FindAbsent1) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	int key1 = 95;
	fitem answer = search(&t, &key1, NULL);
	int a = answer.info ? 1 : 0;
	ASSERT_EQ(a, 0);
	save(&t);
	delTable(&t);
}

TEST(File1, FindAbsent2) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	int key2 = 287;
	fitem answer = search(&t, NULL, &key2);
	int a = answer.info ? 1 : 0;
	ASSERT_EQ(a, 0);
	save(&t);
	delTable(&t);
}

TEST(File1, FindAbsentBoth) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	int key1 = 114, key2 = 995;
	fitem answer = search(&t, &key1, &key2);
	int a = answer.info ? 1 : 0;
	ASSERT_EQ(a, 0);
	save(&t);

	delTable(&t);
}

TEST(File1, Overflow) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	char *info = makeStr("info");
	item* elem = (item*)malloc(sizeof(item));
	fseek(t.fd, 0, SEEK_END);
	*elem = { 44, t.n,  87, ftell(t.fd), (int)strlen(info) + 1 };
	ASSERT_EQ(insert(&t, elem), 3);
	free(info);
	save(&t);

	delTable(&t);
}

TEST(File1, Delete1) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	ASSERT_EQ(rmv(&t, 1, 5), 0);
	ASSERT_EQ(t.n, 14);
	save(&t);
	delTable(&t);
}

TEST(File1, FindDeleted) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	int key1 = 5;
	fitem answer = search(&t, &key1, NULL);
	int a = answer.info ? 1 : 0;
	ASSERT_EQ(a, 0);
	ASSERT_EQ(t.n, 14);

	save(&t);
	delTable(&t);
}

TEST(File1, checkpoint1) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	ASSERT_EQ(t.n, 14);
	save(&t);
	delTable(&t);
}

TEST(File1, Delete2) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	ASSERT_EQ(t.n, 14);
	ASSERT_EQ(rmv(&t, 2, 9), 0);
	ASSERT_EQ(t.n, 13);
	int key1 = 3, key2 = 9;
	fitem answer = search(&t, &key1, NULL);
	int a = answer.info ? 1 : 0;
	ASSERT_EQ(a, 0);
	answer = search(&t, NULL, &key2);
	a = answer.info ? 1 : 0;
	ASSERT_EQ(a, 0);
	answer = search(&t, &key1, &key2);
	a = answer.info ? 1 : 0;
	ASSERT_EQ(a, 0);
	save(&t);
	delTable(&t);
}

TEST(File1, checkpoint2) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	ASSERT_EQ(t.n, 13);
	save(&t);
	delTable(&t);
}

TEST(File1, InsertDuplicate1) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	ASSERT_EQ(t.n, 13);
	char *info = makeStr("info");
	item* elem = (item*)malloc(sizeof(item));
	*elem = { 7, t.n, 74, ftell(t.fd), (int)strlen(info) + 1 };
	ASSERT_EQ(insert(&t, elem), 1);
	ASSERT_EQ(t.n, 13);
	free(info);
	save(&t);
	delTable(&t);
}

TEST(File1, InsertDuplicate2) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	ASSERT_EQ(t.n, 13);
	char *info = makeStr("newInfo");
	item* elem = (item*)malloc(sizeof(item));
	*elem = { 94, t.n, 14, ftell(t.fd), (int)strlen(info) + 1 };
	ASSERT_EQ(insert(&t, elem), 2);
	ASSERT_EQ(t.n, 13);
	free(info);
	save(&t);
	delTable(&t);
}

TEST(File1, InsertDuplicateBoth) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	ASSERT_EQ(t.n, 13);
	char *info = makeStr("newInfo2");
	item* elem = (item*)malloc(sizeof(item));
	*elem = { 1, t.n, 7, ftell(t.fd), (int)strlen(info) + 1 };
	ASSERT_EQ(insert(&t, elem), 1);
	ASSERT_EQ(t.n, 13);
	free(info);
	save(&t);
	delTable(&t);
}

TEST(File1, InsertLow) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	ASSERT_EQ(t.n, 13);
	char *info = makeStr("info");
	fseek(t.fd, 0, SEEK_END);
	item* elem = (item*)malloc(sizeof(item));
	*elem = { 155, t.n, -19, ftell(t.fd), (int)strlen(info) + 1 };
	ASSERT_EQ(insert(&t, elem), 0);
	fwrite(info, sizeof(char), t.ks1[(t.n) - 1].elem->len, t.fd);
	free(info);
	ASSERT_EQ(t.n, 14);
	ASSERT_EQ(t.ks2[0].k, -19);
	save(&t);
	delTable(&t);
}

TEST(File1, InsertHigh) {
	table t = { 0, 0, NULL, NULL, NULL, NULL };
	char *fname = makeStr("file1.dat");
	load(&t, fname);
	ASSERT_EQ(t.n, 14);
	char *info = makeStr("INFO");
	fseek(t.fd, 0, SEEK_END);
	item* elem = (item*)malloc(sizeof(item));
	*elem = { 156, t.n, 291, ftell(t.fd), (int)strlen(info) + 1 };
	ASSERT_EQ(insert(&t, elem), 0);
	fwrite(info, sizeof(char), t.ks1[t.n - 1].elem->len, t.fd);
	free(info);
	ASSERT_EQ(t.n, 15);
	ASSERT_EQ(t.ks2[t.n - 1].k, 291);
	save(&t);
	delTable(&t);
}

TEST(TableNullPtr, properties) {
	ASSERT_EQ(properties(NULL), 0);
}

TEST(TableNullPtr, delTable) {
	ASSERT_EQ(delTable(NULL), 0);
}

TEST(TableNullPtr, save) {
	ASSERT_EQ(save(NULL), 0);
}

TEST(TableNullPtr, load) {
	char *fname = makeStr("file.dat");
	ASSERT_EQ(load(NULL, fname), 0);
	free(fname);
}

TEST(TableNullPtr, rmv) {
	ASSERT_EQ(rmv(NULL, 5, 19), 2);
}

TEST(TableNullPtr, show) {
	ASSERT_EQ(show(NULL), 0);
}

TEST(TableNullPtr, del) {
	ASSERT_EQ(del(NULL), 1);
}

TEST(TableNullPtr, search) {
	ASSERT_EQ(search(NULL, NULL, NULL).key1, 0);
	ASSERT_EQ(search(NULL, NULL, NULL).key2, 0);
	int a = search(NULL, NULL, NULL).info ? 1 : 0;
	ASSERT_EQ(a, 0);
}

TEST(TableNullPtr, find) {
	ASSERT_EQ(find(NULL), 0);
}

TEST(TableNullPtr, search1) {
	ASSERT_EQ(search1(NULL, 21), -1);
}

TEST(TableNullPtr, search2) {
	ASSERT_EQ(search2(NULL, 65), -1);
}

TEST(TableNullPtr, insert2) {
	ASSERT_EQ(insert2(NULL, NULL), -2);
}

TEST(TableNullPtr, insert) {
	ASSERT_EQ(insert(NULL, NULL), 4);
}

TEST(TableNullPtr, add) {
	ASSERT_EQ(add(NULL), 0);
}

int _tmain(int argc, _TCHAR* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


