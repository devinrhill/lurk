#pragma once

#include "Task.hpp"

namespace lvk {

class TaskSceneRoot: public Task {
public:
	TaskSceneRoot() {
		setName("TaskSceneRoot");
	}
};

} // namespace lvk
