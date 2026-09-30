#pragma once

#include <map>
#include <vector>
#include "Task.hpp"
#include "../BumpAllocator.hpp"

struct EventServer {
	BumpAllocator store;

	EventServer() {
		store.init(256 * (sizeof(int) + sizeof(void*)));
	}

	int* accessCode(int port) {
		return (int*)(port * (sizeof(int) + sizeof(void*)));
	}

	void* accessUser(int port) {
		return (void*)((port * (sizeof(int) + sizeof(void*))) + sizeof(int));
	}
};

class TaskEvent: public Task {
public:
	EventServer* server; // i barely know'er
	
	void broadcast(int port, int code, int flags) {
		//

		*server->accessCode(port) = code;
	}
};
