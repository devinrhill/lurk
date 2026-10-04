// Devin Hill 2026

#pragma once

#include "Node.hpp"

namespace lvk {

namespace nutl {

typedef bool (*NODE_LISTUP_FUNC_PTR)(Node*);
typedef bool (*NODE_COMPARE_FUNC_PTR)(Node*, Node*);

int countUpNode(Node* tree) {
	int num = 0;
	for (Node* node2 = tree; node2 != nullptr; node2 = node2->next(tree))
	{
		num++;
	}
	return num;
}

int listUpNode(Node** out, Node* tree, int max = -1, NODE_LISTUP_FUNC_PTR func = nullptr) {
	int num = 0;
	Node* node2 = tree;
	while (node2 != nullptr && (max == -1 || num < max))
	{
		if (func == nullptr || func(node2))
		{
			out[num] = node2;
			num++;
		}
		node2 = node2->next(tree);
	}
	return num;
}

int listUpNodeRev(Node** out, Node* tree, int max = -1, NODE_LISTUP_FUNC_PTR func = nullptr) {
	int num = listUpNode(out, tree, max, func);
	if (num >= 2)
	{
		for (int i = 0; i < num / 2; i++)
		{
			Node* node2 = out[num - i - 1];
			out[num - i - 1] = out[i];
			out[i] = node2;
		}
	}
	return num;
}

int listUpNodeSort(Node** out, Node* tree, int max = -1, NODE_COMPARE_FUNC_PTR cFunc = nullptr, NODE_LISTUP_FUNC_PTR func = nullptr) {
	if (cFunc != nullptr)
	{
		int num = listUpNode(out, tree, max, func);
		int num2 = num;
		int num3 = 1;
		while (num2 > 1 || 0 < num3)
		{
			num2 = num2 * 10 / 13;
			if (num2 < 1)
			{
				num2 = 1;
			}
			else if (num2 == 9 || num2 == 10)
			{
				num2 = 11;
			}
			num3 = 0;
			for (int i = 0; i < num - num2; i++)
			{
				if (cFunc(out[i], out[i + num2]))
				{
					Node* node2 = out[i + num2];
					out[i + num2] = out[i];
					out[i] = node2;
					num3 = 1;
				}
			}
		}
		return num;
	}
	return 0;
}

/*
Node* searchKind(int kind, Node* node, Node* root) {
	while (node != nullptr && node->kind != (uint)kind) {
		node = node->next(root);
	}
	return node;
}
*/

void viewTree(Node* root) {
	int num = 0;
	Node* node2 = root;
	printf("Node Tree---------------------------");
	while (node2 != nullptr)
	{
		if (0 < num)
		{
			for (int i = 0; i < num; i++)
			{
				printf("  ");
			}
			printf("+");
		}
		printf(" %s", node2->name);
		if (node2->child != nullptr)
		{
			node2 = node2->child;
			num++;
		}
		else if (num > 0 && node2->nextSibling != nullptr)
		{
			node2 = node2->nextSibling;
		}
		else if (num > 0)
		{
			do
			{
				node2 = node2->parent;
				num--;
				if (num <= 0)
				{
					node2 = nullptr;
					break;
				}
				if (node2 != nullptr && node2->nextSibling != nullptr)
				{
					node2 = node2->nextSibling;
					break;
				}
			}
			while (node2 != nullptr);
		}
		else
		{
			node2 = nullptr;
		}
	}
	printf("------------------------------------");
}

} // namespace nutl

} // namespace lvk
