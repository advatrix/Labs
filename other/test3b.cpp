#include "pch.h"
#include <tchar.h>
#include "../Lab3bv1/Lab3bv1.cpp"


int strToList(char *str, item **ptr) {
	item head = { '*', NULL };
	item *last = &head;
	for (; *str != '\0'; ++str) {
		last->next = (item*)malloc(sizeof(item));
		last = last->next;
		last->c = *str;
	}
	last->next = NULL;
	*ptr = head.next;
	return 0;
}

int compareLists(item *a, item *b) {
	while (a && b) {
		if (a->c != b->c) return 0;
		a = a->next;
		b = b->next;
	}
	if (!a && !b) return 1;
	else return 0;
 }


TEST(testCase_0, test_Reorg) {
	char buf[4] = "(1)";
	char *s = (char*)malloc(4);
	*s = '\0';
	strcat(s, buf);
	item *ptr;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	char res[2] = "1";
	char *r = (char*)malloc(2);
	*r = '\0';
	strcat(r, res);
	item *rtp;
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(s);
	free(r);
}

TEST(testCase_1, test_Reorg) {
	item *ptr = NULL;
	ASSERT_EQ(NULL, reorg(ptr));
}

TEST(testCase_2, test_Reorg) {
	char buf[] = "(((((((((((((((((((";
	char *s = (char*)malloc(20);
	*s = '\0';
	strcat(s, buf);
	item *ptr;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	char *r = (char*)malloc(1);
	*r = '\0';
	item *rtp;
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(s);
	free(r);
}

TEST(testCase_3, test_Reorg) {
	char buf[] = "\t\t\t\t      \t\t\t\t  \t";
	char *s = (char*)malloc(18);
	*s = '\0';
	strcat(s, buf);
	item *ptr;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	char *r = (char*)malloc(1);
	*r = '\0';
	item *rtp;
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(s);
	free(r);
}

TEST(testCase_4, test_Reorg) {
	char buf[] = " ( (\t ) ) \t";
	char *s = (char*)malloc(12);
	*s = '\0';
	strcat(s, buf);
	item *ptr;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	char *r = (char*)malloc(1);
	*r = '\0';
	item *rtp;
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(s);
	free(r);
}

TEST(testCase_5, test_Reorg) {
	char *s = (char*)malloc(1);
	*s = '\0';
	item *ptr, *rtp;
	strToList(s, &ptr);
	strToList(s, &rtp);
	ptr = reorg(ptr);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(s);
}

TEST(testCase_6, test_Reorg) {
	char buf[] = "one two (three) four five (six) seven eight (nine ten eleven) twelve";
	char *s = (char*)malloc(69);
	*s = '\0';
	strcat(s, buf);
	char res[] = "three six nine ten eleven";
	char *r = (char*)malloc(26);
	*r = '\0';
	strcat(r, res);
	item *ptr, *rtp;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(s);
	free(r);
}

TEST(testCase_7, test_Reorg) {
	char buf[] = "(one two three four five)";
	char *s = (char*)malloc(26);
	*s = '\0';
	strcat(s, buf);
	char res[] = "one two three four five";
	char *r = (char*)malloc(24);
	*r = '\0';
	strcat(r, res);
	item *ptr, *rtp;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(r);
	free(s);
}

TEST(testCase_8, test_Reorg) {
	char buf[] = "(one two three four five";
	char *s = (char*)malloc(25);
	*s = '\0';
	strcat(s, buf);
	char res[] = "one two three four five";
	char *r = (char*)malloc(24);
	*r = '\0';
	strcat(r, res);
	item *ptr, *rtp;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(s);
	free(r);
}

TEST(testCase_9, test_Reorg) {
	char buf[] = "(one (two) ((three four five)";
	char *s = (char*)malloc(30);
	*s = '\0';
	strcat(s, buf);
	char res[] = "one two three four five";
	char *r = (char*)malloc(24);
	*r = '\0';
	strcat(r, res);
	item *ptr, *rtp;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(s);
	free(r);
}

TEST(testCase_10, test_Reorg) {
	char buf[] = "((((one) two three) four five) six seven) eight) nine) ten) eleven) twelve";
	char *s = (char*)malloc(77);
	*s = '\0';
	strcat(s, buf);
	char res[] = "one two three four five six seven";
	char *r = (char*)malloc(34);
	*r = '\0';
	strcat(r, res);
	item *ptr, *rtp;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(r);
	free(s);
}

TEST(testCase_11, test_Reorg) {
	char buf[] = "one ( two )  three";
	char *s = (char*)malloc(19);
	*s = '\0';
	strcat(s, buf);
	char res[] = "two";
	char *r = (char*)malloc(4);
	*r = '\0';
	strcat(r, res);
	item *ptr, *rtp;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(s);
	free(r);
}

TEST(testCase_12, test_Reorg) {
	char buf[] = "one two three";
	char *s = (char*)malloc(14);
	*s = '\0';
	strcat(s, buf);
	char *r = (char*)malloc(1);
	*r = '\0';
	item *ptr, *rtp;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(s);
	free(r);
}

TEST(testCase_13, test_Reorg) {
	char buf[] = "one (two three four five (six seven eight) nine ten eleven (twelve)) thirteen fourteen fifteen sixteen (seventeen) eighteen (nineteen twenty";
	char *s = (char*)malloc(141);
	*s = '\0';
	strcat(s, buf);
	char res[] = "two three four five six seven eight nine ten eleven twelve seventeen nineteen twenty";
	char *r = (char*)malloc(85);
	*r = '\0';
	strcat(r, res);
	item *ptr, *rtp;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(s);
	free(r);
}

TEST(testCase_14, test_Reorg) {
	char buf[] = "lorem(ipsum)dolor sit amet";
	char *s = (char*)malloc(27);
	*s = '\0';
	strcat(s, buf);
	char res[] = "ipsum";
	char *r = (char*)malloc(6);
	*r = '\0';
	strcat(r, res);
	item *ptr, *rtp;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(s);
	free(r);
}

TEST(testCase_15, test_Reorg) {
	char buf[] = "lorem((ipsum)dolor)sit amet";
	char *s = (char*)malloc(28);
	*s = '\0';
	strcat(s, buf);
	char res[] = "ipsum dolor";
	char *r = (char*)malloc(12);
	*r = '\0';
	strcat(r, res);
	item *ptr, *rtp;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(s);
	free(r);
}

TEST(testCase_16, test_Reorg) {
	char buf[] = "(1(2(3)4)5)6)7)8";
	char *s = (char*)malloc(17);
	*s = '\0';
	strcat(s, buf);
	char res[] = "1 2 3 4 5";
	char *r = (char*)malloc(10);
	*r = '\0';
	strcat(r, res);
	item *ptr, *rtp;
	strToList(s, &ptr);
	ptr = reorg(ptr);
	strToList(r, &rtp);
	ASSERT_EQ(1, compareLists(ptr, rtp));
	free(r);
	free(s);
}

int _tmain(int argc, _TCHAR* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
