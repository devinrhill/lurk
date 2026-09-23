#ifndef NODE_HPP
#define NODE_HPP

#include <cstdio>
#include <cstring>
#include "../Middle.hpp"

class Node {
public:
	static constexpr uint NAME_LEN = 0x40;

	char name[NAME_LEN];
	Node* parent;
	Node* child;
	Node* nextSibling;
	Node* prevSibling;

	Node() {
		name[0] = 0;
		parent = nullptr;
		child = nullptr;
		nextSibling = nullptr;
		prevSibling = nullptr;
		setName("Node");
	}

	~Node() {}

	void setName(const char *name) {
		memset(this->name, 0, NAME_LEN);
		memcpy(this->name, name, NAME_LEN);
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

		return true;
	}

	bool connect(Node *node, int nPos = 0) {
		if (node == nullptr) {
			return false;
		}
		node->parent = this;
		if (child == nullptr) {
			child = node;
			node->prevSibling = nullptr;
			node->nextSibling = nullptr;
		} else if (nPos != 0 && nPos == 1) {
			Node *node2 = child;
			child = node;
			node->prevSibling = nullptr;
			node->nextSibling = node2;
			node2->prevSibling = node;
		} else {
			Node *node2 = child;
			while (node2->nextSibling != nullptr) {
				node2 = node2->nextSibling;
			}
			node->prevSibling = node2;
			node->nextSibling = nullptr;
			node2->nextSibling = node;
		}

		return true;
	}

	void close() {
		Node *node2;
		do {
			node2 = child;
			if (node2 != nullptr) {
				while (node2->nextSibling != nullptr) {
					node2 = node2->nextSibling;
				}
				node2->close();
			}
		} while (node2 != nullptr);

		disconnect();
	}

	Node *next(Node *root) {
		if (child != nullptr) {
			return child;
		}
		if (nextSibling != nullptr) {
			return nextSibling;
		}
		Node *node2 = parent;
		while (node2 != nullptr && node2 != root) {
			if (node2->nextSibling != nullptr) {
				return node2->nextSibling;
			}
			node2 = node2->parent;
		}

		return nullptr;
	}

	void print(Node *root, int depth) {
		if (depth >= 0) {
			for (int i = 0; i < depth; i++) {
				printf("	 ");
			}
		} else {
			for (int i = 0; i < -depth; i++) {
				printf("^");
			}
		}
		printf("L %s\n", root->name);

		if (root->child != nullptr) {
			print((Node *)root->child, depth + 1);
		}
		if (root->nextSibling != nullptr) {
			print((Node *)root->nextSibling, depth);
		}
	}

	Node* searchName(Node* root, const char* name) {
		Node* node3;
		Node* node2 = node3 = root;
		while(node3 != nullptr) {
			if(!strcmp(node3->name, name)) {
				return node3;
			}

			node2 = node3;
			node3 = node3->next(root);
		}

		return node3;
	}

	static Node* searchComp(Node* root, Node* comp) {
		Node* node3;
		Node* node2 = node3 = root;
		while(node3 != nullptr) {
			if(!memcmp((byte*)node3, (byte*)comp, sizeof(Node))) {
				return node3;
			}

			node2 = node3;
			node3 = node3->next(root);
		}

		return nullptr;
	}

	uint getChildCount(Node* root) {
		uint count = 0;

		Node* node3;
		Node* node2 = node3 = root;
		while(node3 != nullptr) {
			count++;

			node2 = node3;
			node3 = node3->next(root);
		}

		return count;
	}
};

#endif // NODE_HPP
