#ifndef TASK_HPP
#define TASK_HPP

#include <cmath>
#include <cstdio>
#include "../Middle.hpp"
#include "Node.hpp"

class Task : public Node {
public:
	enum Flags: uint {
		PRE_UPDATE = 1<<0, // main update phases
		UPDATE = 1<<1,
		POST_UPDATE = 1<<2,
		UPDATE_ALL = PRE_UPDATE | UPDATE | POST_UPDATE,

		PRE_DRAW = 1<<3, // main draw phases, avoids priority sorting each frame
		DRAW = 1<<4,
		POST_DRAW = 1<<5,
		DRAW_ALL = PRE_DRAW | DRAW | POST_DRAW,

		FORCE = 1<<6, // force phase execution,

		// if a parent task owns a child task with the EXTERNAL flag set,
		// the parent task must explicitly call required child phases
		EXTERNAL = 1<<7,
	};

	enum DrawFlags: uint {
		DRAW_PRE_3D = 1<<0, // required hints denoting when to call during frame generation
		DRAW_3D = 1<<1,
		DRAW_2D = 1<<2,
		DRAW_POST_2D = 1<<3,
		DRAW_HINT_ALL = DRAW_PRE_3D | DRAW_3D | DRAW_2D | DRAW_POST_2D,
	};

	enum Phase {
		PRE = 0,
		MAIN = 1,
		POST = 2
	};

	static constexpr uint PRIO_DEFAULT = 0x80000000;

	uint status = 0;
	uint drawPriority = PRIO_DEFAULT;
	bool autoFree = false;

	uint flags;
	uint drawFlags[3];

	Task(): Node() {
		setName("Task");

		flags = 0;
		autoFree = false;
		for(uint i = 0; i < 3; i++) {
			drawFlags[i] = 0;
		}
	}

	~Task() {}

	virtual bool preUpdate(Task* param) { return true; }

	virtual bool update(Task* param) { return true; }

	virtual bool postUpdate(Task* param) { return true; }

	virtual void preDraw(int status, Task* param) {}

	virtual void draw(int status, Task* param) {}

	virtual void postDraw(int status, Task* param) {}

	void pause(int flags) {
		this->flags &= ~(flags);
	}

	void resume(int flags) {
		this->flags |= flags;
	}

	/* old
	static void preUpdateAll(Task *root, Task* param = nullptr, bool freeFlag = true) {
		Task *task3;
		Task *task2 = task3 = root;
		while (task3 != nullptr) {
			if(!(task3->flags & EXTERNAL)) {
  				if ((task3->flags & Flags::PRE_UPDATE && !task3->preUpdate(param) &&
					freeFlag) || task3->flags & FORCE) {
		    		task3->close();
		    		task3 = ((task3 == root) ? nullptr : ((Task *)task2->next(root)));
		    		break;
				}
			}

	    	task2 = task3;
	    	task3 = (Task *)task3->next(root);
		}
	}
	*/

	static void preUpdateAll(Task* root, Task* param = nullptr, bool freeFlag = true) {
		Task* task3;
		Task* task2 = (task3 = root);
		while(task3 != nullptr) {
			if((!(task3->flags & EXTERNAL) && task3->flags & PRE_UPDATE) || task3->flags & FORCE) {
				if(!task3->preUpdate(param) && freeFlag) {
					task3->close();
					task3 = ((task3 == root) ? nullptr : ((Task*)task2->next(root)));
					continue;
				}
			}
			
			task2 = task3;
			task3 = (Task*)task3->next(root);
		}
	}

	/* old
	static void updateAll(Task *root, Task* param = nullptr, bool freeFlag = true) {
		Task *task3;
		Task *task2 = task3 = root;
		while (task3 != nullptr) {
			if(!(task3->flags & EXTERNAL)) {
				if ((task3->flags & Flags::UPDATE && !task3->update(param) && freeFlag) || task3->flags & FORCE) {
					task3->close();
					task3 = ((task3 == root) ? nullptr : ((Task *)task2->next(root)));
					break;
				}
			}

			task2 = task3;
			task3 = (Task *)task3->next(root);
		}
	}
	*/

	static void updateAll(Task* root, Task* param = nullptr, bool freeFlag = true) {
		Task* task3;
		Task* task2 = (task3 = root);
		while(task3 != nullptr) {
			if((!(task3->flags & EXTERNAL) && task3->flags & UPDATE) || task3->flags & FORCE) {
				if(!task3->update(param) && freeFlag) {
					task3->close();
					task3 = ((task3 == root) ? nullptr : ((Task*)task2->next(root)));
					continue;
				}
			}
			
			task2 = task3;
			task3 = (Task*)task3->next(root);
		}
	}

	/* old
	static void postUpdateAll(Task *root, Task* param = nullptr, bool freeFlag = true) {
		Task *task3;
		Task *task2 = task3 = root;
		while (task3 != nullptr) {
			if(!(task3->flags & EXTERNAL)) {
				if ((task3->flags & Flags::POST_UPDATE && !task3->postUpdate(param) &&
						freeFlag) || task3->flags & FORCE) {
					task3->close();
					task3 = ((task3 == root) ? nullptr : ((Task *)task2->next(root)));
					break;
				}
			}

			task2 = task3;
			task3 = (Task *)task3->next(root);
		}
	}
	*/

	static void postUpdateAll(Task* root, Task* param = nullptr, bool freeFlag = true) {
		Task* task3;
		Task* task2 = (task3 = root);
		while(task3 != nullptr) {
			if((!(task3->flags & EXTERNAL) && task3->flags & POST_UPDATE) || task3->flags & FORCE) {
				if(!task3->postUpdate(param) && freeFlag) {
					task3->close();
					task3 = ((task3 == root) ? nullptr : ((Task*)task2->next(root)));
					continue;
				}
			}
			
			task2 = task3;
			task3 = (Task*)task3->next(root);
		}
	}

	static void preDrawAll(Task *root, int status, Task* param, int drawHint) {
		Task *task3;
		Task *task2 = task3 = root;
		while (task3 != nullptr) {
			if(!(task3->flags & EXTERNAL)) {
				if (((task3->flags & PRE_DRAW &&
					task3->drawFlags[0] & drawHint) ||
					task3->flags & FORCE)) {
					task3->preDraw(status, param);
				}
			}

			task2 = task3;
			task3 = (Task *)task3->next(root);
		}
	}

	static void drawAll(Task *root, int status, Task* param, int drawHint) {
		Task *task3;
		Task *task2 = task3 = root;
		while (task3 != nullptr) {
			if(!(task3->flags & EXTERNAL)) {
				if (((task3->flags & DRAW &&
					task3->drawFlags[1] & drawHint) ||
					task3->flags & FORCE)) {
					task3->draw(status, param);
				}
			}

			task2 = task3;
			task3 = (Task *)task3->next(root);
		}
	}

	static void postDrawAll(Task *root, int status, Task* param, int drawHint) {
		Task *task3;
		Task *task2 = task3 = root;
		while (task3 != nullptr) {
			if(!(task3->flags & EXTERNAL)) {
				if (((task3->flags & POST_DRAW &&
					task3->drawFlags[2] & drawHint) ||
					task3->flags & FORCE)) {
					task3->postDraw(status, param);
				}
			}
			
			task2 = task3;
			task3 = (Task *)task3->next(root);
		}
	}

	static void procUpdateAll(Task *root, Task* param = nullptr, bool freeFlag = true) {
		preUpdateAll(root, param, freeFlag);
		updateAll(root, param, freeFlag);
		postUpdateAll(root, param, freeFlag);
	}

	static void procDrawAll(Task *root, int status, Task* param, int drawHint) {
		preDrawAll(root, status, param, drawHint);
		drawAll(root, status, param, drawHint);
		postDrawAll(root, status, param, drawHint);
	}

	static void print(Task *root, int depth = 0, int iteration = 0) {
		if(depth == 0) {
			printf("|  Phases  |  Order  |  Tree view\n");
			printf("|__123123__|__1234___|__________________________________________________________________\n");
		}

		printf("|  ");

		struct TmpFlagOut {
			int flag;
			char ch;
		};
		TmpFlagOut tmps[10] = {
			{.flag=PRE_UPDATE, .ch='u'},
			{.flag=UPDATE, .ch='u'},
			{.flag=POST_UPDATE, .ch='u'},
			{.flag=PRE_DRAW, .ch='D'},
			{.flag=DRAW, .ch='D'},
			{.flag=POST_DRAW, .ch='D'},
			{.flag=DRAW_PRE_3D, .ch='1'},
			{.flag=DRAW_3D, .ch='2'},
			{.flag=DRAW_2D, .ch='3'},
			{.flag=DRAW_POST_2D, .ch='4'}
		};

		for(int i = 0; i < 6; i++) {
			if(root->flags & tmps[i].flag) {
				fputc(tmps[i].ch, stdout);
			} else {
				fputc('.', stdout);
			}
		}

		printf("  |  ");

		for(int i = 6; i <= 10; i++) {
			if(root->flags & tmps[i].flag) {
				fputc(tmps[i].ch, stdout);
			} else {
				fputc('.', stdout);
			}
		}

		printf("   |  ");

		if (depth >= 0) {
			for (int i = 0; i < depth; i++) {
				printf("    ");
			}
		} else {
			for (int i = 0; i < -depth; i++) {
				printf("^");
			}
		}
		if (root->parent != nullptr) {
			printf("L ");
		}
		printf(" %s     ", root->name);

		int extent =
				(abs(depth) * 2) + strlen(root->name) + (root->parent != nullptr ? 2 : 0);
		for (int i = 0; i < 20 - extent; i++) {
			printf(" ");
		}

		printf("\n");

		if (root->child != nullptr) {
			print((Task *)root->child, depth + 1, iteration + 1);
		}
		if (root->nextSibling != nullptr) {
			print((Task *)root->nextSibling, depth, iteration + 1);
		}
	}
};

#endif // TASK_HPP
