#pragma once

#include "TaskApp.hpp"
#include "TaskEvent.hpp"

class TaskAppEvent: public TaskApp, public TaskEvent {
public:
	TaskAppEvent(): TaskApp(), TaskEvent() {
    	TaskApp::setName("TaskAppEvent");
	}

	~TaskAppEvent() {
	}
};
