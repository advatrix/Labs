#include "pch.h"
#include <tchar.h>
#include "C:\Users\troja\source\repos\Lab3av4\Lab3av4\googletest-master\googletest\include\gtest\gtest.h"
#include <malloc.h>
#include <string.h>
#pragma warning(disable:4996)
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
	while (*tmp1) {
		//s[j] = s[j + k + 1];
		*tmp1 = *(tmp1 + k + 1);
		tmp1++;
	}
	tmp1 = s + strlen(s) - 1;
	//for (int j = strlen(s) - 1; ; j--) {
//		if (s[j] == ' ' || s[j] == '\t') s[j] = '\0';
//		else break;
//	}
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
			//	for (int j = i; j < strlen(s); j++)s[j] = s[j + k - 1];
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

TEST(testCase_1, test_Reorg) {
	char buf[29] = "\t kot \t kit \t tok \t\t 0 kat \t";
	char *s = (char*)malloc(29);
	*s = '\0';
	strcat(s, buf);
	char res[12] = "kot kit kat";
	char *r = (char*)malloc(12);
	*r = '\0';
	strcat(r, res);
	ASSERT_EQ(*(reorg(s)), *res);
	free(s);
	free(r);
}

TEST(testCase_2, test_Reorg) {
	char buf[37] = "             \t \t \t         \t\t\t\t\t   \t";
	char *s = (char*)malloc(37);
	*s = '\0';
	strcat(s, buf);
	char r = '\0';
	ASSERT_EQ(*(reorg(s)), r);
	free(s);
}

TEST(testCase_3, test_Reorg) {
	char buf[5] = "test";
	char *s = (char*)malloc(5);
	*s = '\0';
	strcat(s, buf);
	char *r = (char*)malloc(5);
	*r = '\0';
	strcat(r, buf);
	ASSERT_EQ(*(reorg(s)), *r);
	free(s);
	free(r);
}

TEST(testCase_4, test_Reorg) {
	char buf[35] = "\t\t  \t27 \t 217 \t\t72 \t 712 \t00000\t  ";
	char *s = (char*)malloc(35);
	*s = '\0';
	strcat(s, buf);
	char res[7] = "27 217";
	char *r = (char*)malloc(7);
	*r = '\0';
	strcat(r, res);
	ASSERT_EQ(*(reorg(s)), *r);
	free(s);
	free(r);
}

TEST(testCase_5, test_Reorg) {
	char buf[27] = "lorem ipsum dolor sit amet";
	char *s = (char*)malloc(27);
	*s = '\0';
	strcat(s, buf);
	char res[6] = "lorem";
	char *r = (char*)malloc(6);
	*r = '\0';
	strcat(r, res);
	ASSERT_EQ(*(reorg(s)), *r);
	free(s);
	free(r);
}


TEST(testCase_6, test_Reorg) {
	char buf[141] = "\t\taaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa\t\t\t\t\taaaaaaa      qqqqqqqq    wsaawqsdmfdmb    aaaaa   aaa aaaa              aaaaaaa\t";
	char *s = (char*)malloc(141);
	*s = '\0';
	strcat(s, buf);
	char res[85] = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa aaaaaaa aaaaa aaa aaaa aaaaaaa";
	char *r = (char*)malloc(85);
	*r = '\0';
	strcat(r, res);
	ASSERT_EQ(*(reorg(s)), *r);
	free(s);
	free(r);
}

TEST(testCase_7, test_Reorg) {
	char buf[1] = "";
	char *s = (char*)malloc(1);
	strcat(s, buf);
	char *r = (char*)malloc(1);
	strcat(r, buf);
	ASSERT_EQ(*(reorg(s)), *r);
}

TEST(testCase_8, test_Reorg) {
	char *s = NULL;
	char *r = NULL;
	ASSERT_EQ((reorg(s)), r);
}

int _tmain(int argc, _TCHAR* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}