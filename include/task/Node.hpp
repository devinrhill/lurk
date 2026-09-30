#pragma once

#include <cstring>
#include <cstdio>
#include "../Middle.hpp"

class Node {
public:
	static constexpr uint NAME_LEN = 0x40;

	char name[NAME_LEN];

	Node* parent;
	Node* child;
	Node* nextSibling;
	Node* prevSibling;

	Node()
		: name{},
		  parent(nullptr),
		  child(nullptr),
		  nextSibling(nullptr),
		  prevSibling(nullptr) {
		setName("Node");
	}

	~Node() {}

	void setName(const char* newName) {
		if (newName == nullptr) {
			name[0] = '\0';
			return;
		}

		std::strncpy(name, newName, NAME_LEN - 1);
		name[NAME_LEN - 1] = '\0';
	}

	bool disconnect() {
		if (child != nullptr) {
			return false;
		}

		if (parent != nullptr && parent->child == this) {
			parent->child = nextSibling;
		}

		if (prevSibling != nullptr) {
			prevSibling->nextSibling = nextSibling;
		}

		if (nextSibling != nullptr) {
			nextSibling->prevSibling = prevSibling;
		}

		parent = nullptr;
		prevSibling = nullptr;
		nextSibling = nullptr;

		return true;
	}

	bool connect(Node* node, bool insertBegin = false) {
		if (node == nullptr || node == this) {
			return false;
		}

		if (node->parent != nullptr ||
			node->prevSibling != nullptr ||
			node->nextSibling != nullptr) {
			return false;
		}

		for (Node* p = this; p != nullptr; p = p->parent) {
			if (p == node) {
				return false;
			}
		}

		node->parent = this;

		if (child == nullptr) {
			child = node;
			node->prevSibling = nullptr;
			node->nextSibling = nullptr;
			return true;
		}

		if (insertBegin) {
			Node* first = child;

			node->prevSibling = nullptr;
			node->nextSibling = first;

			first->prevSibling = node;
			child = node;

			return true;
		} else {
			Node* last = child;
			while (last->nextSibling != nullptr) {
				last = last->nextSibling;
			}

			last->nextSibling = node;
			node->prevSibling = last;
			node->nextSibling = nullptr;
		}

		return true;
	}

	void close() {
		while (child != nullptr) {
			Node* node = child;

			while (node->nextSibling != nullptr) {
				node = node->nextSibling;
			}

			node->close();
		}

		disconnect();
	}

	Node* next(Node* root) {
		if (root == nullptr) {
			return nullptr;
		}

		if (child != nullptr) {
			return child;
		}

		if (nextSibling != nullptr) {
			return nextSibling;
		}

		Node* node = parent;

		while (node != nullptr && node != root) {
			if (node->nextSibling != nullptr) {
				return node->nextSibling;
			}

			node = node->parent;
		}

		return nullptr;
	}

	void print(Node* root, int depth = 0) {
		if (root == nullptr) {
			return;
		}

		if (depth >= 0) {
			for (int i = 0; i < depth; ++i) {
				std::printf("\t");
			}
		} else {
			for (int i = 0; i < -depth; ++i) {
				std::printf("^");
			}
		}

		std::printf("L %s\n", root->name);

		if (root->child != nullptr) {
			print(root->child, depth + 1);
		}

		if (root->nextSibling != nullptr) {
			print(root->nextSibling, depth);
		}
	}

	Node* searchName(Node* root, const char* searchName) {
		if (root == nullptr || searchName == nullptr) {
			return nullptr;
		}

		Node* node = root;

		while (node != nullptr) {
			if (std::strcmp(node->name, searchName) == 0) {
				return node;
			}

			node = node->next(root);
		}

		return nullptr;
	}

	static Node* searchComp(Node* root, const Node* comp) {
		if (root == nullptr || comp == nullptr) {
			return nullptr;
		}

		Node* node = root;

		while (node != nullptr) {
			if (std::strcmp(node->name, comp->name) == 0) {
				return node;
			}

			node = node->next(root);
		}

		return nullptr;
	}

	uint getChildCount(Node* root) {
		if (root == nullptr) {
			return 0;
		}

		uint count = 0;

		for (Node* node = root->child;
			 node != nullptr;
			 node = node->nextSibling) {
			++count;
		}

		return count;
	}
};
