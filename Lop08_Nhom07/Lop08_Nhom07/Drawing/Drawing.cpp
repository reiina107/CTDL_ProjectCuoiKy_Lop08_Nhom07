#include "graphics.h"
#include <dos.h>
#include <stdio.h>
#include <sstream>

int addressXYCir[100][4];
int temp = 0;

struct NODE
{
	int LTAG, RTAG;
	int INFO;
	int checkApproved = 0;
	NODE* LLINK, * RLINK;
};

struct DATA {
	NODE* node;
	DATA* next;
};

struct STACK {
	DATA* top;
	void init() {
		top = nullptr;
	}
	bool isEmpty() {
		return top == nullptr;
	}
	bool insert(DATA* data) {
		if (top == nullptr) {
			top = data;
			top->next = nullptr;
			return true;
		}
		data->next = top;
		top = data;
		return true;
	}
	bool insertNode(NODE* N) {
		DATA* data = (DATA*)malloc(sizeof(DATA));
		data->node = N;
		return this->insert(data);
	}
	DATA* deleteStack() {
		if (this->isEmpty()) return nullptr;
		DATA* result = (DATA*)malloc(sizeof(DATA));
		result->node = top->node;
		DATA* new_top = top->next;
		delete(top);
		top = new_top;
		return result;
	}
};

void ChangeColorNode(NODE* node, int a)
{
	int redFlag = 0;
	for (int i = 0; i < temp; i++)
		if (addressXYCir[i][2] == node->INFO)
		{
			redFlag = i;
			break;
		}
	// Color: a = 0 -> white, a = 1 -> yellow, a = 2 -> green
	if (a == 0)
		circle(addressXYCir[redFlag][0], addressXYCir[redFlag][1], 40, 255, 255, 255);
	if (a == 1)
		circle(addressXYCir[redFlag][0], addressXYCir[redFlag][1], 40, 255, 255, 0);
	if (a == 2)
		circle(addressXYCir[redFlag][0], addressXYCir[redFlag][1], 40, 0, 255, 0);
	stringstream ss;
	ss << addressXYCir[redFlag][2];
	string s = ss.str();
	char arr[10] = "0";
	for (int i = 0; i < s.length(); i++)
		arr[i] = s[i];
	arr[s.length()] = '\0';
	drawText(addressXYCir[redFlag][0] - 15, addressXYCir[redFlag][1] - 20, arr);
	Sleep(500);
}

void recordResult(NODE* node, int x)
{
	int redFlag = 0;
	for (int i = 0; i < temp; i++)
		if (addressXYCir[i][2] == node->INFO)
		{
			redFlag = i;
			break;
		}
	stringstream ss;
	ss << addressXYCir[redFlag][2];
	string s = ss.str();
	char arr[10] = "0";
	for (int i = 0; i < s.length(); i++)
		arr[i] = s[i];
	arr[s.length()] = '\0';
	drawText(x, 456, arr);
}

bool CheckNodeApproved(NODE* node)
{
	if (node->checkApproved == 1)
		return true;
	return false;
}

struct Threaded_BinaryTree {
	NODE* root;
	void init() {
		root = nullptr;
	}

	//Inorder 1 3 4 6 7 8 10 13 14
	NODE* leftNode() {
		NODE* P = root;
		while (P->LTAG == 0) {
			ChangeColorNode(P, 0);
			P = P->LLINK;
			ChangeColorNode(P, 1);
		}
		return P;
	}
	NODE* nextNodeInorder(NODE* P) {
		NODE* N = P->RLINK;
		int redFlag = 0;
		ChangeColorNode(N, 1);

		if (P->RTAG == 1) return N;
		while (N->LTAG == 0) {
			ChangeColorNode(N, 0);
			N = N->LLINK;
			ChangeColorNode(N, 1);
		}
		return N;
	}
	void inorder() {
		NODE* P = leftNode();
		int redFlag = 0;
		int x = 332;
		while (P != root) {
			ChangeColorNode(P, 2);
			recordResult(P, x);
			x = x + 50;
			P = nextNodeInorder(P);
		}
		ChangeColorNode(P, 2);
	}

	//Preorder 8 3 1 6 4 7 10 14 13
	NODE* nextNodePreorder(NODE* P) {
		NODE* N = P->LLINK;
		// Check Node Approved
		if (!CheckNodeApproved(N))
			ChangeColorNode(N, 1);
		if (P->LTAG == 0) return N;
		// Check Node Approved
		if (!CheckNodeApproved(N))
			ChangeColorNode(N, 0);
		N = P->RLINK;
		ChangeColorNode(N, 1);
		return N;
	}
	void preorder() {
		NODE* P = root->LLINK;
		ChangeColorNode(P, 1);
		int x = 332;
		while (P != root) {
			P->checkApproved = 1;
			ChangeColorNode(P, 2);
			recordResult(P, x);
			x = x + 50;
			P = nextNodePreorder(P);
		}
		ChangeColorNode(P, 2);
	}

	//Postorder 1 4 7 6 3 13 14 10 8
	NODE* findFatherNode(NODE* P) {
		STACK S;
		S.init();
		NODE* N = root->LLINK; 
		S.insertNode(N);
		while (S.isEmpty() == 0) {
			N = S.deleteStack()->node;
			if ((N->LTAG != 1 && N->LLINK == P)
				|| (N->RTAG != 1 && N->RLINK == P))
				return N;
			if (N->LTAG != 1)
				S.insertNode(N->LLINK);
			if (N->RTAG != 1)
				S.insertNode(N->RLINK);
		}
	}
	NODE* nextNodePostorder(NODE* P) {
		if (P->RTAG == 1)
		{
			ChangeColorNode(P->RLINK, 1);
			return P->RLINK;
		}
		if (P == root->LLINK)
			return root;
		NODE* N = findFatherNode(P);
		// Check Node Approved
		if (!N->checkApproved)
			ChangeColorNode(N, 1);
		if (N->RLINK == P)
		{
			ChangeColorNode(N, 1);
			return N;
		}
		else {
			N = N->RLINK;
			// Check Node Approved
			if (!N->checkApproved)
				ChangeColorNode(N, 1);
			while (N->LTAG != 1 || N->RTAG != 1) {
				while (N->LTAG == 0) {
					N = N->LLINK;
					// Check Node Approved
					if (!N->checkApproved)
						ChangeColorNode(N, 1);
				}
				if (N->RTAG != 1)
				{
					N = N->RLINK;
					// Check Node Approved
					if (!N->checkApproved)
						ChangeColorNode(N, 1);
				}
			}
			ChangeColorNode(N, 1);
			return N;
		}
	}
	void postorder() {
		NODE* P = leftNode();
		int x = 332;
		while (P != root) {
			P->checkApproved = 1;
			ChangeColorNode(P, 2);
			recordResult(P, x);
			x = x + 50;
			P = nextNodePostorder(P);
		}
	}

	// Insert node
	void InsertNode_Inorder(NODE* node, NODE* tree) {
		if (tree == nullptr)
			tree = node;
		else
		{
			NODE* P = tree;
			ChangeColorNode(P, 1);
			while (true) {
				if (P->INFO > node->INFO && P->LLINK == nullptr)
				{
					P->LLINK = node; break;
				}
				else if (P->INFO < node->INFO && P->RLINK == nullptr)
				{
					P->RLINK = node; break;
				}
				else if (P->INFO > node->INFO)
				{
					ChangeColorNode(P, 0);
					P = P->LLINK;
					ChangeColorNode(P, 1);
				}
				else if (P->INFO < node->INFO)
				{
					ChangeColorNode(P, 0);
					P = P->RLINK;
					ChangeColorNode(P, 1);
				}
				else if (P->INFO == node->INFO)
				{
					drawText(450, 15, "Node already in the tree");
					Sleep(2000);
					break;
				}
			}
		}
	}

	// Search in binary search tree
	void SearchInBinarySearchTree(NODE* node)
	{
		NODE* P = root->LLINK;
		bool check = false;
		ChangeColorNode(P, 1);
		while (true)
		{
			if (node->INFO < P->INFO)
			{
				if (P->LTAG == 1)
					break;
				ChangeColorNode(P, 0);
				P = P->LLINK;
				ChangeColorNode(P, 1);
			}
			else if (node->INFO > P->INFO)
			{
				if (P->RTAG == 1)
					break;
				ChangeColorNode(P, 0);
				P = P->RLINK;
				ChangeColorNode(P, 1);
			}
			else if (node->INFO == P->INFO)
			{
				ChangeColorNode(P, 2);
				check = true;
				break;
			}
			else if (P->LLINK == root || P->RLINK == root || P->INFO > P->LLINK->INFO)
			{
				break;
			}
		}
		if (check == false)
			drawText(450, 15, "The number you are looking for does not exist");
		else
			drawText(450, 15, "The number you are looking for is displayed with a green background");
		Sleep(10000);
	}
};

Threaded_BinaryTree* build_ThreadedBinaryTree_Inorder() {
	//Inorder 1 3 4 6 7 8 10 13 14
	Threaded_BinaryTree* tree = (Threaded_BinaryTree*)malloc(sizeof(Threaded_BinaryTree));
	tree->init();
	NODE* node_8, * node_3, * node_10, * node_1, * node_6, * node_4, * node_7, * node_14, * node_13;

	node_8 = (NODE*)malloc(sizeof(NODE)); node_3 = (NODE*)malloc(sizeof(NODE));
	node_1 = (NODE*)malloc(sizeof(NODE)); node_10 = (NODE*)malloc(sizeof(NODE));
	node_6 = (NODE*)malloc(sizeof(NODE)); node_4 = (NODE*)malloc(sizeof(NODE));
	node_7 = (NODE*)malloc(sizeof(NODE)); node_14 = (NODE*)malloc(sizeof(NODE));
	node_13 = (NODE*)malloc(sizeof(NODE));

	node_8->INFO = 8; node_3->INFO = 3;
	node_1->INFO = 1; node_10->INFO = 10;
	node_6->INFO = 6; node_4->INFO = 4;
	node_7->INFO = 7; node_14->INFO = 14;
	node_13->INFO = 13;

	tree->root = (NODE*)malloc(sizeof(NODE));

	node_8->LTAG = 0, node_8->RTAG = 0;
	node_8->LLINK = node_3, node_8->RLINK = node_10;

	node_3->LTAG = 0, node_3->RTAG = 0;
	node_3->LLINK = node_1, node_3->RLINK = node_6;

	node_1->LTAG = 1, node_1->RTAG = 1;
	node_1->LLINK = tree->root, node_1->RLINK = node_3;

	node_6->LTAG = 0, node_6->RTAG = 0;
	node_6->LLINK = node_4, node_6->RLINK = node_7;

	node_4->LTAG = 1, node_4->RTAG = 1;
	node_4->LLINK = node_3, node_4->RLINK = node_6;

	node_7->LTAG = 1, node_7->RTAG = 1;
	node_7->LLINK = node_6, node_7->RLINK = node_8;

	node_10->LTAG = 1, node_10->RTAG = 0;
	node_10->LLINK = node_8, node_10->RLINK = node_14;

	node_14->LTAG = 0, node_14->RTAG = 1;
	node_14->LLINK = node_13, node_14->RLINK = tree->root;

	node_13->LTAG = 1, node_13->RTAG = 1;
	node_13->LLINK = node_10, node_13->RLINK = node_14;

	tree->root->LTAG = 0; tree->root->RTAG = 1;
	tree->root->LLINK = node_8; tree->root->RLINK = tree->root;

	return tree;
}

Threaded_BinaryTree* build_ThreadedBinaryTree_Preorder() {
	//Preorder 8 3 1 6 4 7 10 14 13
	Threaded_BinaryTree* tree = (Threaded_BinaryTree*)malloc(sizeof(Threaded_BinaryTree));
	tree->init();
	NODE* node_8, * node_3, * node_10, * node_1, * node_6, * node_4, * node_7, * node_14, * node_13;

	node_8 = (NODE*)malloc(sizeof(NODE)); node_3 = (NODE*)malloc(sizeof(NODE));
	node_1 = (NODE*)malloc(sizeof(NODE)); node_10 = (NODE*)malloc(sizeof(NODE));
	node_6 = (NODE*)malloc(sizeof(NODE)); node_4 = (NODE*)malloc(sizeof(NODE));
	node_7 = (NODE*)malloc(sizeof(NODE)); node_14 = (NODE*)malloc(sizeof(NODE));
	node_13 = (NODE*)malloc(sizeof(NODE));

	node_8->INFO = 8; node_3->INFO = 3;
	node_1->INFO = 1; node_10->INFO = 10;
	node_6->INFO = 6; node_4->INFO = 4;
	node_7->INFO = 7; node_14->INFO = 14;
	node_13->INFO = 13;

	tree->root = (NODE*)malloc(sizeof(NODE));

	node_8->LTAG = 0, node_8->RTAG = 0;
	node_8->LLINK = node_3, node_8->RLINK = node_10;

	node_3->LTAG = 0, node_3->RTAG = 0;
	node_3->LLINK = node_1, node_3->RLINK = node_6;

	node_1->LTAG = 1, node_1->RTAG = 1;
	node_1->LLINK = node_3, node_1->RLINK = node_6;

	node_6->LTAG = 0, node_6->RTAG = 0;
	node_6->LLINK = node_4, node_6->RLINK = node_7;

	node_4->LTAG = 1, node_4->RTAG = 1;
	node_4->LLINK = node_6, node_4->RLINK = node_7;

	node_7->LTAG = 1, node_7->RTAG = 1;
	node_7->LLINK = node_4, node_7->RLINK = node_10;

	node_10->LTAG = 1, node_10->RTAG = 0;
	node_10->LLINK = node_7, node_10->RLINK = node_14;

	node_14->LTAG = 0, node_14->RTAG = 1;
	node_14->LLINK = node_13, node_14->RLINK = node_13;

	node_13->LTAG = 1, node_13->RTAG = 1;
	node_13->LLINK = node_14, node_13->RLINK = tree->root;

	tree->root->LTAG = 0; tree->root->RTAG = 1;
	tree->root->LLINK = node_8; tree->root->RLINK = tree->root;

	return tree;
}

Threaded_BinaryTree* build_ThreadedBinaryTree_Postorder() {
	//Postorder 1 4 7 6 3 13 14 10 8
	Threaded_BinaryTree* tree = (Threaded_BinaryTree*)malloc(sizeof(Threaded_BinaryTree));
	tree->init();
	NODE* node_8, * node_3, * node_10, * node_1, * node_6, * node_4, * node_7, * node_14, * node_13;

	node_8 = (NODE*)malloc(sizeof(NODE)); node_3 = (NODE*)malloc(sizeof(NODE));
	node_1 = (NODE*)malloc(sizeof(NODE)); node_10 = (NODE*)malloc(sizeof(NODE));
	node_6 = (NODE*)malloc(sizeof(NODE)); node_4 = (NODE*)malloc(sizeof(NODE));
	node_7 = (NODE*)malloc(sizeof(NODE)); node_14 = (NODE*)malloc(sizeof(NODE));
	node_13 = (NODE*)malloc(sizeof(NODE));

	node_8->INFO = 8; node_3->INFO = 3;
	node_1->INFO = 1; node_10->INFO = 10;
	node_6->INFO = 6; node_4->INFO = 4;
	node_7->INFO = 7; node_14->INFO = 14;
	node_13->INFO = 13;

	tree->root = (NODE*)malloc(sizeof(NODE));

	node_8->LTAG = 0, node_8->RTAG = 0;
	node_8->LLINK = node_3, node_8->RLINK = node_10;

	node_3->LTAG = 0, node_3->RTAG = 0;
	node_3->LLINK = node_1, node_3->RLINK = node_6;

	node_1->LTAG = 1, node_1->RTAG = 1;
	node_1->LLINK = tree->root, node_1->RLINK = node_4;

	node_6->LTAG = 0, node_6->RTAG = 0;
	node_6->LLINK = node_4, node_6->RLINK = node_7;

	node_4->LTAG = 1, node_4->RTAG = 1;
	node_4->LLINK = node_1, node_4->RLINK = node_7;

	node_7->LTAG = 1, node_7->RTAG = 1;
	node_7->LLINK = node_4, node_7->RLINK = node_6;

	node_10->LTAG = 1, node_10->RTAG = 0;
	node_10->LLINK = node_14, node_10->RLINK = node_14;

	node_14->LTAG = 0, node_14->RTAG = 1;
	node_14->LLINK = node_13, node_14->RLINK = node_10;

	node_13->LTAG = 1, node_13->RTAG = 1;
	node_13->LLINK = node_3, node_13->RLINK = node_14;

	tree->root->LTAG = 0; tree->root->RTAG = 1;
	tree->root->LLINK = node_8; tree->root->RLINK = tree->root;

	return tree;
}

NODE* build_UnThreadedBinaryTree()
{
	NODE* node1 = (NODE*)malloc(sizeof(NODE));
	NODE* node3 = (NODE*)malloc(sizeof(NODE));
	NODE* node4 = (NODE*)malloc(sizeof(NODE));
	NODE* node6 = (NODE*)malloc(sizeof(NODE));
	NODE* node7 = (NODE*)malloc(sizeof(NODE));
	NODE* node8 = (NODE*)malloc(sizeof(NODE));
	NODE* node10 = (NODE*)malloc(sizeof(NODE));
	NODE* node13 = (NODE*)malloc(sizeof(NODE));
	NODE* node14 = (NODE*)malloc(sizeof(NODE));

	node8->LLINK = node3; node8->RLINK = node10;
	node3->LLINK = node1; node3->RLINK = node6;
	node10->LLINK = nullptr; node10->RLINK = node14;
	node1->LLINK = nullptr; node1->RLINK = nullptr;
	node6->LLINK = node4; node6->RLINK = node7;
	node14->LLINK = node13; node14->RLINK = nullptr;
	node4->LLINK = nullptr; node4->RLINK = nullptr;
	node7->LLINK = nullptr; node7->RLINK = nullptr;
	node13->LLINK = nullptr; node13->RLINK = nullptr;

	node1->INFO = 1;
	node3->INFO = 3;
	node4->INFO = 4;
	node6->INFO = 6;
	node7->INFO = 7;
	node8->INFO = 8;
	node10->INFO = 10;
	node13->INFO = 13;
	node14->INFO = 14;

	return node8;
}

NODE* build_UnThreadedBinarySearchTree()
{
	NODE* node1 = (NODE*)malloc(sizeof(NODE));
	NODE* node2 = (NODE*)malloc(sizeof(NODE));
	NODE* node3 = (NODE*)malloc(sizeof(NODE));
	NODE* node4 = (NODE*)malloc(sizeof(NODE));
	NODE* node5 = (NODE*)malloc(sizeof(NODE));
	NODE* node7 = (NODE*)malloc(sizeof(NODE));
	NODE* node9 = (NODE*)malloc(sizeof(NODE));
	NODE* node10 = (NODE*)malloc(sizeof(NODE));
	NODE* node12 = (NODE*)malloc(sizeof(NODE));
	NODE* node15 = (NODE*)malloc(sizeof(NODE));

	node7->LLINK = node3; node7->RLINK = node10;
	node3->LLINK = node2; node3->RLINK = node5;
	node10->LLINK = node9; node10->RLINK = node12;
	node2->LLINK = node1; node2->RLINK = nullptr;
	node5->LLINK = node4; node5->RLINK = nullptr;
	node9->LLINK = nullptr; node9->RLINK = nullptr;
	node12->LLINK = nullptr; node12->RLINK = node15;
	node1->LLINK = nullptr; node1->RLINK = nullptr;
	node4->LLINK = nullptr; node4->RLINK = nullptr;
	node15->LLINK = nullptr; node15->RLINK = nullptr;

	node1->INFO = 1;
	node2->INFO = 2;
	node3->INFO = 3;
	node4->INFO = 4;
	node5->INFO = 5;
	node7->INFO = 7;
	node9->INFO = 9;
	node10->INFO = 10;
	node12->INFO = 12;
	node15->INFO = 15;

	return node7;
}

Threaded_BinaryTree* build_ThreadedBinarySearchTree_Inorder()
{
	// Inorder 1 2 3 4 5 7 9 10 12 15
	Threaded_BinaryTree* tree = (Threaded_BinaryTree*)malloc(sizeof(Threaded_BinaryTree));
	tree->init();
	NODE* node1, * node2, * node3, * node4, * node5, * node7, * node9, * node10, * node12, * node15;

	node1 = (NODE*)malloc(sizeof(NODE)); node2 = (NODE*)malloc(sizeof(NODE));
	node3 = (NODE*)malloc(sizeof(NODE)); node4 = (NODE*)malloc(sizeof(NODE));
	node5 = (NODE*)malloc(sizeof(NODE)); node7 = (NODE*)malloc(sizeof(NODE));
	node9 = (NODE*)malloc(sizeof(NODE)); node10 = (NODE*)malloc(sizeof(NODE));
	node12 = (NODE*)malloc(sizeof(NODE)); node15 = (NODE*)malloc(sizeof(NODE));

	node1->INFO = 1; node2->INFO = 2;
	node3->INFO = 3; node4->INFO = 4;
	node5->INFO = 5; node7->INFO = 7;
	node9->INFO = 9; node10->INFO = 10;
	node12->INFO = 12; node15->INFO = 15;

	tree->root = (NODE*)malloc(sizeof(NODE));

	node1->LLINK = tree->root; node1->RLINK = node2;
	node1->LTAG = 1; node1->RTAG = 1;
	node2->LLINK = node1; node2->RLINK = node3;
	node2->LTAG = 0; node2->RTAG = 1;
	node3->LLINK = node2; node3->RLINK = node5;
	node3->LTAG = 0; node3->RTAG = 0;
	node4->LLINK = node3; node4->RLINK = node5;
	node4->LTAG = 1; node4->RTAG = 1;
	node5->LLINK = node4; node5->RLINK = node7;
	node5->LTAG = 0; node5->RTAG = 1;
	node7->LLINK = node3; node7->RLINK = node10;
	node7->LTAG = 0; node7->RTAG = 0;
	node9->LLINK = node7; node9->RLINK = node10;
	node9->LTAG = 1; node9->RTAG = 1;
	node10->LLINK = node9; node10->RLINK = node12;
	node10->LTAG = 0; node10->RTAG = 0;
	node12->LLINK = node10; node12->RLINK = node15;
	node12->LTAG = 1; node12->RTAG = 0;
	node15->LLINK = node12; node15->RLINK = tree->root;
	node15->LTAG = 1; node15->RTAG = 1;

	tree->root->LTAG = 0; tree->root->RTAG = 1;
	tree->root->LLINK = node7; tree->root->RLINK = tree->root;
	return tree;
}

void DrawNodeTop(int a, int b, NODE* root, int w, int l, int x) {
	Sleep(x);
	circle(a, b, 40, 255, 255, 255);
	addressXYCir[temp][0] = a;
	addressXYCir[temp][1] = b;
	addressXYCir[temp++][2] = root->INFO;
	stringstream ss;
	ss << root->INFO;
	string s1 = ss.str();
	char arr[10] = "0";
	for (int i = 0; i < s1.length(); i++)
		arr[i] = s1[i];
	arr[s1.length()] = '\0';
	drawText(a - 15, b - 20, arr);
	line(a, b + 20, a + 110, b + 75);
	line(a, b + 20, a - 110, b + 75);
}

void DrawBranch(int a, int  b, NODE* root, int  w, int l, int wline, int x)
{
	Sleep(x);
	// draw the left branch
	if (root->LLINK != nullptr)
	{
		line(a, b + 20, a - 110 + wline, b + 75);
		circle(a - w, b + l, 40, 255, 255, 255);
		addressXYCir[temp][0] = a - w;
		addressXYCir[temp][1] = b + l;
		addressXYCir[temp++][2] = root->LLINK->INFO;
		stringstream ss;
		ss << root->LLINK->INFO;
		string s1 = ss.str();
		char arr[10] = "0";
		for (int i = 0; i < s1.length(); i++)
			arr[i] = s1[i];
		arr[s1.length()] = '\0';
		drawText(a - w - 15, b + l - 20, arr);
		DrawBranch(a - w, b + l, root->LLINK, w - 30, l, wline + 23, x);
		Sleep(x);
	}
	// draw the right branch
	if (root->RLINK != nullptr)
	{
		line(a, b + 20, a + 110 - wline, b + 75);
		circle(a + w, b + l, 40, 255, 255, 255);
		addressXYCir[temp][0] = a + w;
		addressXYCir[temp][1] = b + l;
		addressXYCir[temp++][2] = root->RLINK->INFO;
		stringstream ss;
		ss << root->RLINK->INFO;
		string s1 = ss.str();
		char arr[10] = "0";
		for (int i = 0; i < s1.length(); i++)
			arr[i] = s1[i];
		arr[s1.length()] = '\0';
		drawText(a + w - 15, b + l - 20, arr);
		DrawBranch(a + w, b + l, root->RLINK, w - 30, l, wline + 23, x);
		Sleep(x);
	}
}

void DrawResult(int a, int b)
{
	int wline = 0;
	for (int i = 0; i < temp; i++)
	{
		rectangle(a - 225 + wline, b + 350, a - 175 + wline, b + 400);
		setcolor(4);
		wline = wline + 50;
	}
};

void DrawBinaryTree(int a, int b, NODE* root, int w, int l, int wline, int x)
{
	DrawNodeTop(a, b, root, w, l, x);
	DrawBranch(a, b, root, w, l, wline, x);
	Sleep(500);
}


int main() {

	initgraph();

	NODE* tree = build_UnThreadedBinaryTree();
	int a = 550, b = 100, w = 120, l = 80, wline = 0, x;

	do {
		temp = 0;
		cout << "1. Browsing inorder" << endl;
		cout << "2. Browsing postorder" << endl;
		cout << "3. Browsing preorder" << endl;
		cout << "4. Insert node" << endl;
		cout << "5. Search in binary search tree" << endl;
		cout << "0. Exit" << endl;
		cout << "Choose a program: ";
		cin >> x;
		if (x == 1)
		{
			system("cls");
			// Draw Binary Tree
			DrawBinaryTree(a, b, tree, w, l, wline, 0);
			DrawResult(a, b);
			// Browsing Inorder
			Threaded_BinaryTree* tree;
			tree = build_ThreadedBinaryTree_Inorder();
			tree->inorder();
		}
		else if (x == 2)
		{
			system("cls");
			// Draw Binary Tree
			DrawBinaryTree(a, b, tree, w, l, wline, 0);
			DrawResult(a, b);
			// Browsing Postorder
			Threaded_BinaryTree* tree;
			tree = build_ThreadedBinaryTree_Postorder();
			tree->postorder();
		}
		else if (x == 3)
		{
			system("cls");
			// Draw Binary Tree
			DrawBinaryTree(a, b, tree, w, l, wline, 0);
			DrawResult(a, b);
			// Browsing Preorder
			Threaded_BinaryTree* tree;
			tree = build_ThreadedBinaryTree_Preorder();
			tree->preorder();
		}
		else if (x == 4)
		{
			system("cls");
			NODE* SearchTree = build_UnThreadedBinarySearchTree();
			DrawBinaryTree(a, b, SearchTree, w, l, wline, 0);

			Threaded_BinaryTree* tree;
			tree = build_ThreadedBinarySearchTree_Inorder();

			int k;
			cout << "Enter the number to insert: ";
			cin >> k;

			NODE* nodeInsert = (NODE*)malloc(sizeof(NODE));
			nodeInsert->LLINK = nullptr; nodeInsert->RLINK = nullptr;
			nodeInsert->INFO = k;
			tree->InsertNode_Inorder(nodeInsert, SearchTree);
			system("cls");
			DrawBinaryTree(a, b, SearchTree, w, l, wline, 0);
			ChangeColorNode(nodeInsert, 2);
		}
		else if (x == 5)
		{
			system("cls");
			NODE* SearchTree = build_UnThreadedBinarySearchTree();
			DrawBinaryTree(a, b, SearchTree, w, l, wline, 0);

			Threaded_BinaryTree* tree;
			tree = build_ThreadedBinarySearchTree_Inorder();

			int k;
			cout << "Enter the number to search: ";
			cin >> k;

			NODE* nodeSearch = (NODE*)malloc(sizeof(NODE));
			nodeSearch->INFO = k;
			tree->SearchInBinarySearchTree(nodeSearch);
		}
		else if (x == 0)
			break;
		else
			cout << "You entered wrong, please re-enter\n";
	} while (true);

	return 0;
}