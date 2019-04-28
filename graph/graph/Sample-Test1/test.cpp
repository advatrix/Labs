#include "pch.h"
#include "..\Graph\Graph.cpp"
#include <tchar.h>

char* makeStr(const char buf[]) {
	char *s = (char*)malloc(strlen(buf) + 1);
	*s = '\0';
	strcat(s, buf);
	return s;
}

TEST(GraphNullPtr, delGraph) {
	ASSERT_EQ(delGraph(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, killChildren) {
	ASSERT_EQ(killChildren(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, displayTree) {
	ASSERT_EQ(displayTree(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, decompose) {
	ASSERT_EQ(decompose(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, dgenerate) {
	ASSERT_EQ(dgenerate(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, properties) {
	ASSERT_EQ(properties(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, timing) {
	ASSERT_EQ(timing(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, display) {
	ASSERT_EQ(display(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, getCords) {
	ASSERT_EQ(getCords(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, getID) {
	ASSERT_EQ(getId(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, dvertexRemove) {
	ASSERT_EQ(dvertexRemove(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, dedgeInsert) {
	ASSERT_EQ(dedgeInsert(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, dcreate) {
	ASSERT_EQ(dcreate(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, dvertexInsert) {
	ASSERT_EQ(dvertexInsert(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, dload) {
	ASSERT_EQ(dload(NULL), GRAPH_NULLPTR);
}

TEST(GraphNullPtr, dsave) {
	ASSERT_EQ(dsave(NULL), GRAPH_NULLPTR);
}

TEST(StackNullPtr, push) {
	AdjList* tmp = (AdjList*)calloc(1, sizeof(AdjList));
	ASSERT_EQ(push(NULL, tmp), STACK_NULLPTR);
	free(tmp);
}

TEST(AdjListNullPtr, push) {
	Stack* tmp = (Stack*)calloc(1, sizeof(Stack));
	ASSERT_EQ(push(&tmp, NULL), ADJLIST_NULLPTR);
	free(tmp);
}

TEST(emptyStack, push) {
	Stack* s = (Stack*)calloc(1, sizeof(Stack));
	AdjList* a = (AdjList*)calloc(1, sizeof(AdjList));
	Vertex* v = (Vertex*)calloc(1, sizeof(Vertex));
	a->vertex = v;
	push(&s, a);
	ASSERT_TRUE(s->next->ind == a->vertex->ind);
	free(a);
	free(v);
	free(s->next);
	free(s);
}

TEST(graph1, transpose) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	Graph t = transpose(&g);
	ASSERT_EQ(g.e, t.e);
	ASSERT_EQ(g.v, t.v);
	ASSERT_EQ(g.adjlist[0].vertex->point->id, t.adjlist[0].vertex->point->id);
	ASSERT_EQ(t.adjlist[0].next->vertex->point->id, 5);
	delGraph(&t);
	delGraph(&g);
}

TEST(emptyGraph, transpose) {
	Graph g = { 0, 0, NULL };
	Graph t = transpose(&g);
	ASSERT_EQ(t.e, 0);
	ASSERT_EQ(t.v, 0);
	ASSERT_TRUE(g.adjlist == NULL);
	delGraph(&t);
	delGraph(&g);
}

TEST(GraphNullPtr, dfsVisit) {
	ASSERT_EQ(dfsVisit(NULL, NULL, NULL, NULL), GRAPH_NULLPTR);
}

TEST(AdjListNullPtr, dfsVisit) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(dfsVisit(&g, NULL, NULL, NULL), ADJLIST_NULLPTR);
}

TEST(TimeNullPtr, dfsVisit) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	ASSERT_EQ(dfsVisit(&g, &g.adjlist[0], NULL, NULL), TIME_NULLPTR);
	delGraph(&g);
}

TEST(StackNullPtr, dfsVisit) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	int time = 0;
	ASSERT_EQ(dfsVisit(&g, &g.adjlist[0], NULL, &time), STACK_NULLPTR);
	delGraph(&g);
}

TEST(GraphNullPtr, dfs) {
	ASSERT_TRUE(dfs(NULL, NULL) == NULL);
}

TEST(StackNullPtr, dfs) {
	Graph g = { 0, 0, NULL };
	ASSERT_TRUE(dfs(&g, NULL) == NULL);
}

TEST(ParentNullPtr, insertChild) {
	ASSERT_EQ(insertChild(NULL, NULL), PARENT_NULLPTR);
}

TEST(ChildNullPtr, insertChild) {
	Vertex* p = (Vertex*)calloc(1, sizeof(Vertex));
	ASSERT_EQ(insertChild(p, NULL), CHILD_NULLPTR);
	free(p);
}

TEST(lonelyParent, insertChild) {
	Vertex* p = (Vertex*)calloc(1, sizeof(Vertex));
	Vertex* c = (Vertex*)calloc(1, sizeof(Vertex));
	insertChild(p, c);
	ASSERT_EQ(p->nChilds, 1);
	ASSERT_TRUE(p->child);
	ASSERT_TRUE(p->child[0] == c);
	free(p);
	free(c);
}

TEST(normalParent, insertChild) {
	Vertex* p = (Vertex*)calloc(1, sizeof(Vertex));
	Vertex* c1 = (Vertex*)calloc(1, sizeof(Vertex));
	Vertex* c2 = (Vertex*)calloc(1, sizeof(Vertex));
	Vertex* c3 = (Vertex*)calloc(1, sizeof(Vertex));
	Vertex* c4 = (Vertex*)calloc(1, sizeof(Vertex));
	Vertex* c5 = (Vertex*)calloc(1, sizeof(Vertex));
	insertChild(p, c1);
	insertChild(p, c2);
	insertChild(p, c3);
	insertChild(p, c4);
	insertChild(p, c5);
	ASSERT_EQ(p->nChilds, 5);
	ASSERT_TRUE(p->child);
	ASSERT_TRUE(p->child[0] == c1);
	ASSERT_TRUE(p->child[1] == c2);
	ASSERT_TRUE(p->child[2] == c3);
	ASSERT_TRUE(p->child[3] == c4);
	ASSERT_TRUE(p->child[4] == c5);
	free(p);
	free(c1);
	free(c2);
	free(c3);
	free(c4);
	free(c5);
}

TEST(GraphNullPtr, dfstVisit) {
	ASSERT_EQ(dfstVisit(NULL, NULL), GRAPH_NULLPTR);
}

TEST(AdjListNullPtr, dfstVisit) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(dfstVisit(&g, NULL), ADJLIST_NULLPTR);
}

TEST(lonelyVertex, dfstVisit) {
	Graph g = { 0, 0, NULL };
	vertexInsert(&g, 0, 0, 0);
	dfstVisit(&g, g.adjlist);
	ASSERT_EQ(g.adjlist[0].vertex->color, BLACK);
	delGraph(&g);
}

TEST(GraphNullPtr, dfst) {
	ASSERT_EQ(dfst(NULL, NULL), GRAPH_NULLPTR);
}

TEST(StackNullPtr, dfst) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(dfst(&g, NULL), STACK_NULLPTR);
}

TEST(normalParent, killChildren) {
	Graph g = { 0, 0, NULL };
	for (int i = 0; i < 10; i++) vertexInsert(&g, i, i, i);
	for (int i = 1; i < 10; i++) insertChild(g.adjlist[0].vertex, g.adjlist[i].vertex);
	killChildren(&g);
	ASSERT_EQ(g.adjlist[0].vertex->nChilds, 0);
	ASSERT_FALSE(g.adjlist[0].vertex->child);
	delGraph(&g);
}

TEST(vertexNullPtr, displayNode) {
	ASSERT_EQ(displayNode(NULL), UNDEFINED_VERTEX);
}



int _tmain(int argc, _TCHAR* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}