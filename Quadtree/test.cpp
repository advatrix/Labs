#include "pch.h"
#include "../Lab5v1/Lab5v1.cpp"
#include <tchar.h>

TEST(TreeNullPtr, Create) {
	ASSERT_EQ(createTree(NULL, NULL, 0, 0, 0, 0, 0), 1);
}

TEST(TreeNullPtr, dLoad) {
	ASSERT_EQ(dload(NULL), 2);
}

TEST(TreeNullPtr, load) {
	ASSERT_EQ(load(NULL, NULL), 1);
}

TEST(TreeNullPtr, CreateFile) {
	ASSERT_EQ(createFile(NULL, NULL), 1);
}

TEST(TreeNullPtr, dinsert) {
	ASSERT_EQ(dinsert(NULL), 0);
}

TEST(TreeNullPtr, insert) {
	ASSERT_EQ(insert(NULL, NULL), 1);
}

TEST(TreeNullPtr, search) {
	ASSERT_EQ(search(NULL, NULL, NULL, 0, 0), 2);
}

TEST(TreeNullPtr, delTree) {
	ASSERT_EQ(delTree(NULL), 1);
}

TEST(TreeNullPtr, save) {
	ASSERT_EQ(save(NULL), 2);
}

TEST(TreeNullPtr, dsave) {
	ASSERT_EQ(dsave(NULL), 0);
}

TEST(TreeNullPtr, dremove) {
	ASSERT_EQ(dremove(NULL), 2);
}

TEST(TreeNullPtr, rmv) {
	ASSERT_EQ(rmv(NULL, 0, 0), 2);
}

TEST(TreeNullPtr, dsearch) {
	ASSERT_EQ(dsearch(NULL), 1);
}

TEST(TreeNullPtr, dshow) {
	ASSERT_EQ(dshow(NULL), 1);
}

TEST(TreeNullPtr, showTree) {
	ASSERT_EQ(showTree(NULL), 2);
}

TEST(TreeNullPtr, showXDirectOrder) {
	ASSERT_EQ(showXDirectOrder(NULL), 2);
}

TEST(TreeNullPtr, properties) {
	ASSERT_EQ(properties(NULL), 1);
}

TEST(TreeNullPtr, showRange) {
	ASSERT_EQ(showRange(NULL), 2);
}

TEST(TreeNullPtr, timing) {
	ASSERT_EQ(timing(NULL), 2);
}

TEST(NLessThan1, createTree) {
	QTree t = { 0, NULL, NULL };
	char* fname = makeStr("testfile");
	ASSERT_EQ(createTree(&t, fname, -19, 25, -174, 48, -7), 3);
	free(fname);
}

TEST(NullFilename, load) {
	QTree t = { 0, NULL, NULL };
	ASSERT_EQ(load(&t, NULL), 2);
}

TEST(NullFilename, createFile) {
	QTree t = { 0, NULL, NULL };
	ASSERT_EQ(createFile(&t, NULL), 2);
}

TEST(NullParent, divide) {
	ASSERT_EQ(divide(NULL), 1);
}

TEST(NullElem, insert) {
	QTree t = { 0, NULL, NULL };
	ASSERT_EQ(insert(&t, NULL), 2);
}

TEST(NullNode, search) {
	QTree t = { 0, NULL, NULL };
	ASSERT_EQ(search(&t, NULL, NULL, 0, 0), 3);
}

TEST(OutsidePoint, quadrant) {
	ASSERT_EQ(quadrant(9000, 100500, -9000, 8000, 12000, 96000), -1);
}

TEST(CenterPoint, quadrant) {
	ASSERT_EQ(quadrant(0, 0, -1, 1, -1, 1), 0);
}

TEST(NEPoint, quadrant) {
	ASSERT_EQ(quadrant(1, 1, -2, 2, -2, 2), 0);
}

TEST(NEEdgePoint, quadrant) {
	ASSERT_EQ(quadrant(5, 5, -5, 5, -5, 5), 0);
}

TEST(SEPoint, quadrant) {
	ASSERT_EQ(quadrant(1, -1, -2, 2, -2, 2), 1);
}

TEST(SEEdgePoint, quadrant) {
	ASSERT_EQ(quadrant(5, -5, -5, 5, -5, 5), 1);
}

TEST(SWPoint, quadrant) {
	ASSERT_EQ(quadrant(-1, -1, -2, 2, -2, 2), 2);
}

TEST(SWEdgePoint, quadrant) {
	ASSERT_EQ(quadrant(-5, -5, -5, 5, -5, 5), 2);
}

TEST(NWPoint, quadrant) {
	ASSERT_EQ(quadrant(-1, 1, -2, 2, -2, 2), 3);
}

TEST(NWEdgePoint, quadrant) {
	ASSERT_EQ(quadrant(-5, 5, -5, 5, -5, 5), 3);
}

TEST(NullElem, linsert) {
	ASSERT_EQ(linsert(NULL, NULL), 2);
}

TEST(NullHead, linsert) {
	char* info = makeStr("text");
	elem e = { 5, 17, info };
	ASSERT_EQ(linsert(&e, NULL), 3);
	free(info);
}

TEST(NullNode, traverse) {
	ASSERT_TRUE(traverse(NULL, NULL, NULL) == NULL);
}

TEST(NullElem, inRange) {
	ASSERT_EQ(inRange(NULL, 154, 974, -930, 255), 1);
}

TEST(NullNode, treeView) {
	ASSERT_EQ(treeView(NULL, 54), 1);
}

TEST(NullNode, delNode) {
	ASSERT_EQ(delNode(NULL, 2), 1);
}

TEST(EmptyTree, properties) {
	QTree t = { 0, NULL, NULL };
	char* fname = makeStr("testfile.dat");
	createTree(&t, fname, -10000, 10000, -10000, 10000, 4);
	ASSERT_EQ(t.N, 4);
	ASSERT_EQ(t.n, 0);
	ASSERT_EQ(t.root->busy, 0);
	ASSERT_TRUE(t.root->child == NULL);
	ASSERT_TRUE(t.root->itemshead.data == NULL);
	ASSERT_EQ(t.root->xmax, 10000);
	ASSERT_EQ(t.root->xmin, -10000);
	ASSERT_EQ(t.root->ymin, -10000);
	ASSERT_EQ(t.root->ymax, 10000);
	fclose(t.fd);
	delTree(&t);
}

TEST(EmptyTree, search) {
	QTree t = { 0, NULL, NULL };
	char* fname = makeStr("testfile.dat");
	createTree(&t, fname, -10000, 10000, -10000, 10000, 4);
	itemList* ans;
	Node* r = t.root;
	search(&t, &r, &ans, 15, 52);
	ASSERT_TRUE(ans == NULL);
	fclose(t.fd);
	delTree(&t);

}

TEST(EmptyTree, remove) {
	QTree t = { 0, NULL, NULL };
	char* fname = makeStr("testfile.dat");
	createTree(&t, fname, -10000, 10000, -10000, 10000, 4);
	ASSERT_EQ(rmv(&t, 47, 17), 1);
	fclose(t.fd);
	delTree(&t);
}

TEST(EmptyTree, insert) {
	QTree t = { 0, NULL, NULL };
	char* fname = makeStr("testfile.dat");
	createTree(&t, fname, -10000, 10000, -10000, 10000, 4);
	elem* e = (elem*)malloc(sizeof(elem));
	char* info = makeStr("text");
	e->x = 12;
	e->y = 7;
	e->info = info;
	insert(&t, e);
	ASSERT_EQ(t.n, 1);
	ASSERT_EQ(t.root->busy, 1);
	ASSERT_TRUE(t.root->child == NULL);
	ASSERT_FALSE(t.root->itemshead.next == NULL);
	fclose(t.fd);
	delTree(&t);
}

TEST(OneTree, remove) {
	QTree t = { 0, NULL, NULL };
	char* fname = makeStr("testfile.dat");
	createTree(&t, fname, -10000, 10000, -10000, 10000, 4);
	rmv(&t, 12, 7);
	ASSERT_EQ(t.n, 0);
	ASSERT_EQ(t.root->busy, 0);
	ASSERT_TRUE(t.root->child == NULL);
	ASSERT_TRUE(t.root->itemshead.data == NULL);
	fclose(t.fd);
	delTree(&t);
}



int _tmain(int argc, _TCHAR* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}