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

TEST(Zero, maxEdgeN) {
	ASSERT_EQ(maxEdgeN(0), 0);
}

TEST(One, maxEdgeN) {
	ASSERT_EQ(maxEdgeN(1), 1);
}

TEST(Two, maxEdheN) {
	ASSERT_EQ(maxEdgeN(2), 4);
}

TEST(Negative, maxEdgeN) {
	ASSERT_EQ(maxEdgeN(-655), 0);
}

TEST(Positive, maxEdgeN) {
	ASSERT_EQ(maxEdgeN(291), 84681);
}

TEST(Negative, generate) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(generate(&g, -15, 68), INCORRECT_VERTICES_NUMBER);
}

TEST(NegativeEdgeNumber, generate) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(generate(&g, 17, -96), INCORRECT_EDGES_NUMBER);
}

TEST(tooManyEdges, generate) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(generate(&g, 26, 700), TOO_MANY_EDGES);
}

TEST(lonelyVertex, generate) {
	Graph g = { 0, 0, NULL };
	generate(&g, 1, 0);
	ASSERT_EQ(g.v, 1);
	ASSERT_EQ(g.e, 0);
	ASSERT_TRUE(g.adjlist);
	delGraph(&g);
}

TEST(emptyGraph, generate) {
	Graph g = { 0, 0, NULL };
	generate(&g, 0, 0);
	ASSERT_EQ(g.v, 0);
	ASSERT_EQ(g.e, 0);
	ASSERT_FALSE(g.adjlist);
	delGraph(&g);
}

TEST(lonelyEdge, generate) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(generate(&g, 0, 1), INCORRECT_VERTICES_NUMBER);
}

TEST(graph100, generate) {
	Graph g = { 0, 0, NULL };
	generate(&g, 100, 100);
	ASSERT_EQ(g.v, 100);
	ASSERT_EQ(g.e, 100);
	ASSERT_TRUE(g.adjlist);
	delGraph(&g);
}

TEST(randomGraph, generate) {
	Graph g = { 0, 0, NULL };
	srand(time(NULL));
	int v = rand()/100;
	int e = maxEdgeN(v)/10;
	generate(&g, v, e);
	ASSERT_EQ(g.v, v);
	ASSERT_EQ(g.e, e);
	ASSERT_TRUE(g.adjlist);
	delGraph(&g);
}

TEST(emptyGraph, vertexIDSearch) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(vertexIDSearch(&g, 12), NOT_FOUND);
}

TEST(lonelyVertex, lonelyVertexIDSearch) {
	Graph g = { 0, 0, NULL };
	vertexInsert(&g, 17, 65.44, 7);
	ASSERT_EQ(vertexIDSearch(&g, 7), 0);
	delGraph(&g);
}

TEST(lonelyVertex, absentVertexIDSearch) {
	Graph g = { 0, 0, NULL };
	vertexInsert(&g, 17, 65.44, 7);
	ASSERT_EQ(vertexIDSearch(&g, 14), NOT_FOUND);
	delGraph(&g);
}

TEST(graph100, vertexIDSearch) {
	Graph g = { 0, 0, NULL };
	generate(&g, 100, 100);
	ASSERT_EQ(vertexIDSearch(&g, 67), 67);
	delGraph(&g);
}

TEST(emptyGraph, cordVertexRemove) {
	Graph g = { 0, 0, NULL };
	double x = 17.7965;
	double y = 4;
	ASSERT_EQ(vertexRemove(&g, &x, &y, NULL), VERTEX_NOT_FOUND);
}

TEST(undefinedVertex, vertexRemove) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(vertexRemove(&g, NULL, NULL, NULL), UNDEFINED_VERTEX);
}

TEST(emptyGraph, IDVertexRemove) {
	Graph g = { 0, 0, NULL };
	int id = 16;
	ASSERT_EQ(vertexRemove(&g, NULL, NULL, &id), VERTEX_NOT_FOUND);
}

TEST(lonelyVertex, cordVertexRemove) {
	Graph g = { 0, 0, NULL };
	double x = 17.99;
	double y = 86;
	int id = 72;
	vertexInsert(&g, x, y, id);
	vertexRemove(&g, &x, &y, NULL);
	ASSERT_EQ(g.v, 0);
	ASSERT_FALSE(g.adjlist);
}

TEST(lonelyVertex, IDVertexRemove) {
	Graph g = { 0, 0, NULL };
	double x = 17.99;
	double y = 86;
	int id = 72;
	vertexInsert(&g, x, y, id);
	vertexRemove(&g, NULL, NULL, &id);
	ASSERT_EQ(g.v, 0);
	ASSERT_FALSE(g.adjlist);
}

TEST(lonelyVertex, IDVertexRemoveAbsent) {
	Graph g = { 0, 0, NULL };
	double x = 17.99;
	double y = 86;
	int id = 72;
	vertexInsert(&g, x, y, 86);
	ASSERT_EQ(vertexRemove(&g, NULL, NULL, &id), VERTEX_NOT_FOUND);
	ASSERT_EQ(g.v, 1);
	ASSERT_TRUE(g.adjlist);
	delGraph(&g);
}

TEST(lonelyVertex, cordVertexRemoveAbsent) {
	Graph g = { 0, 0, NULL };
	double x = 17.99;
	double y = 86;
	int id = 72;
	vertexInsert(&g, 19, 66, id);
	ASSERT_EQ(vertexRemove(&g, &x, &y, NULL), VERTEX_NOT_FOUND);
	ASSERT_EQ(g.v, 1);
	ASSERT_TRUE(g.adjlist);
	delGraph(&g);
}

TEST(GraphNullPtr, edgeSearch) {
	ASSERT_TRUE(edgeSearch(NULL, 15, 66) == NULL);
}

TEST(lonelyVertex, edgeSearch) {
	Graph g = { 0, 0, NULL };
	vertexInsert(&g, 19, 64, 5);
	ASSERT_TRUE(edgeSearch(&g, 5, 0) == NULL);
}

TEST(graph1, edgeSearch) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	AdjList* tmp = edgeSearch(&g, 2, 5);
	ASSERT_EQ(tmp->vertex->point->id, 5);
	delGraph(&g);
}

TEST(graph1, AbsentSid1EdgeSearch) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	ASSERT_TRUE(edgeSearch(&g, 1, 19) == NULL);
	delGraph(&g);
}

TEST(graph1, AbsentSid2EdgeSearch) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	ASSERT_TRUE(edgeSearch(&g, 1, 8) == NULL);
	delGraph(&g);
}

TEST(graph1, AbsentFid1EdgeSearch) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	ASSERT_TRUE(edgeSearch(&g, 9, 1) == NULL);
	delGraph(&g);
}


TEST(graph1, AbsentFid2EdgeSearch) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	ASSERT_TRUE(edgeSearch(&g, 2, 1) == NULL);
	delGraph(&g);
}

TEST(graph100, IDVertexRemove) {
	Graph g = { 0, 0, NULL };
	generate(&g, 100, 100);
	int id = 17;
	vertexRemove(&g, NULL, NULL, &id);
	ASSERT_EQ(g.v, 99);
	ASSERT_EQ(g.adjlist[17].vertex->point->id, 99);
	delGraph(&g);
}

TEST(emptyGraph, edgeInsert) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(edgeInsert(&g, 0, 0), SECOND_NOT_FOUND);
}

TEST(lonelyVertex, edgeInsert1) {
	Graph g = { 0, 0, NULL };
	vertexInsert(&g, 15, 16, 4);
	ASSERT_EQ(edgeInsert(&g, 4, 0), SECOND_NOT_FOUND);
}

TEST(lonelyVertex, edgeInsert2) {
	Graph g = { 0, 0, NULL };
	vertexInsert(&g, 19, 64, 8);
	ASSERT_EQ(edgeInsert(&g, 0, 8), FIRST_NOT_FOUND);
}

TEST(graph1, AbsentFidEdgeInsert) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	ASSERT_EQ(edgeInsert(&g, 90, 2), FIRST_NOT_FOUND);
	delGraph(&g);
}

TEST(graph1, AbsentSidEdgeInsert) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	ASSERT_EQ(edgeInsert(&g, 2, 44), SECOND_NOT_FOUND);
	delGraph(&g);
}

TEST(graph1, DuplicateEdgeInsert) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	ASSERT_EQ(edgeInsert(&g, 2, 6), EDGE_DUPLICATE);
	delGraph(&g);
}

TEST(graph1, EdgeInsert) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	edgeInsert(&g, 1, 4);
	ASSERT_EQ(g.e, 15);
	ASSERT_TRUE(g.adjlist[0].next->vertex->point->id == 4);
	delGraph(&g);
}

TEST(graph1, LoopInsert) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	edgeInsert(&g, 1, 1);
	ASSERT_EQ(g.e, 15);
	ASSERT_TRUE(g.adjlist[0].next->vertex->point->id == 1);
	delGraph(&g);
}

TEST(emptyGraph, vertexCordSearch) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(vertexCordSearch(&g, 0, 0), NOT_FOUND);
}

TEST(graph1, absentXYVertexCordSearch) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	ASSERT_EQ(vertexCordSearch(&g, 9.5, 9.6), NOT_FOUND);
	delGraph(&g);
}

TEST(graph1, absentYVertexCordSearch) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	ASSERT_EQ(vertexCordSearch(&g, 71, 78), NOT_FOUND);
	delGraph(&g);
}

TEST(graph1, absentXVertexCordSearch) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	ASSERT_EQ(vertexCordSearch(&g, 78, 12), NOT_FOUND);
	delGraph(&g);
}

TEST(graph1, VertexCordSearch) {
	Graph g = { 0, 0, NULL };
	char* fname = makeStr("file");
	load(&g, fname);
	ASSERT_EQ(vertexCordSearch(&g, 71, 12), 2);
	delGraph(&g);
}

TEST(emptyGraph, vertexInsert) {
	Graph g = { 0, 0, NULL };
	vertexInsert(&g, 17, 23, 55);
	ASSERT_EQ(g.v, 1);
	ASSERT_TRUE(g.adjlist);
	ASSERT_EQ(g.adjlist[0].vertex->point->id, 55);
	ASSERT_EQ(g.adjlist[0].vertex->point->x, 17);
	ASSERT_EQ(g.adjlist[0].vertex->point->y, 23);
}

TEST(graph1, duplicatePointVertexInsert) {
	Graph g = { 0, 0, NULL };
	load(&g, makeStr("file"));
	ASSERT_EQ(vertexInsert(&g, 71, 12, 99), DUPLICATE_POINT);
	delGraph(&g);
}

TEST(graph1, duplicateIDVertexInsert) {
	Graph g = { 0, 0, NULL };
	load(&g, makeStr("file"));
	ASSERT_EQ(vertexInsert(&g, 72, 13, 3), DUPLICATE_ID);
	delGraph(&g);
}

TEST(graph1, vertexInsert) {
	Graph g = { 0, 0, NULL };
	load(&g, makeStr("file"));
	vertexInsert(&g, 72, 13, 20);
	ASSERT_EQ(g.v, 9);
	ASSERT_EQ(g.adjlist[8].vertex->point->id, 20);
	ASSERT_EQ(g.adjlist[8].vertex->point->x, 72);
	ASSERT_EQ(g.adjlist[8].vertex->point->y, 13);
	delGraph(&g);
}

TEST(GraphNullPtr, pointInsert) {
	ASSERT_EQ(pointInsert(NULL, NULL), GRAPH_NULLPTR);
}

TEST(PointNullPtr, pointInsert) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(pointInsert(&g, NULL), POINT_NULLPTR);
}

TEST(emptyGraph, pointInsert) {
	Graph g = { 0, 0, NULL };
	Point *p = (Point*)calloc(1, sizeof(Point));
	pointInsert(&g, p);
	ASSERT_TRUE(g.adjlist);
	ASSERT_TRUE(g.adjlist[0].vertex->point == p);
	delGraph(&g);
}

TEST(graph1, pointInsert) {
	Graph g = { 0, 0, NULL };
	Point *p = (Point*)calloc(1, sizeof(Point));
	load(&g, makeStr("file"));
	pointInsert(&g, p);
	ASSERT_TRUE(g.adjlist[8].vertex->point == p);
	delGraph(&g);
}

TEST(GraphNullPtr, save) {
	ASSERT_EQ(save(NULL, NULL), GRAPH_NULLPTR);
}

TEST(FileNameNullPtr, save) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(save(&g, NULL), FNAME_NULLPTR);
}

TEST(graph100, save) {
	Graph g = { 0, 0, NULL };
	generate(&g, 100, 100);
	save(&g, makeStr("tmp"));
	Graph n = { 0, 0, NULL };
	load(&n, makeStr("tmp"));
	for (int i = 0; i < 100; i++) {
		ASSERT_EQ(g.adjlist[i].vertex->point->id, n.adjlist[i].vertex->point->id);
		ASSERT_EQ(g.adjlist[i].vertex->point->x, n.adjlist[i].vertex->point->x);
		ASSERT_EQ(g.adjlist[i].vertex->point->y, n.adjlist[i].vertex->point->y);
	}
	delGraph(&g);
	delGraph(&n);
}

TEST(GraphNullPtr, load) {
	ASSERT_EQ(load(NULL, NULL), GRAPH_NULLPTR);
}

TEST(FileNameNullPtr, load) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(load(&g, NULL), FNAME_NULLPTR);
}

TEST(graph100, load) {
	Graph g = { 0, 0, NULL };
	load(&g, makeStr("tmp"));
	ASSERT_EQ(g.e, 100);
	ASSERT_EQ(g.v, 100);
	delGraph(&g);
}

TEST(graph1, load) {
	Graph g = { 0, 0, NULL };
	load(&g, makeStr("file"));
	ASSERT_EQ(g.e, 14);
	ASSERT_EQ(g.v, 8);
	ASSERT_EQ(g.adjlist[0].next->vertex->point->id, 2);
	delGraph(&g);
}

TEST(newFile, load) {
	Graph g = { 0, 0, NULL };
	ASSERT_EQ(load(&g, makeStr("phantom")), LOADING_FAILURE);
}

TEST(emptyGraph, load) {
	Graph g = { 0, 0, NULL };
	load(&g, makeStr("empty"));
	ASSERT_EQ(g.v, 0);
	ASSERT_EQ(g.e, 0);
	ASSERT_TRUE(g.adjlist == NULL);
}
 
int _tmain(int argc, _TCHAR* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}