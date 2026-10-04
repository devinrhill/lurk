// Devin Hill 2026

#pragma once

#include "../../core/GameSysCore.hpp"
#include "TaskSysCore.hpp"
#include "TaskState.hpp"

namespace lvk {

class TaskApp: public TaskState {
public:
    struct Stopwatch {
        long start;
        long total;
        bool isRunning;
    };

    bool isPaused;
    Stopwatch* stopwatches;

	TaskApp(): TaskState() {
    	setName("TaskApp");

    	flags = 0;
    	stopwatches = new Stopwatch[8];
    	isPaused = false;
    	for(uint i = 0; i < 8; i++) {
        	stopwatchReset(i);
    	}
    	TaskCore.mainApp = this;
    	TaskCore.updateParam = this;
    	TaskCore.drawParam = this;
	}

	~TaskApp() {
    	delete[] stopwatches;
    	TaskCore.mainApp = nullptr;
    	TaskCore.updateParam = nullptr;
    	TaskCore.drawParam = nullptr;
	}

	void stopwatchReset(uint n) {
    	stopwatches[n].isRunning = false;
    	stopwatches[n].start = 0;
    	stopwatches[n].total = 0;
	}

	void stopwatchStart(uint n) {
    	if(stopwatches[n].isRunning) {
        	stopwatches[n].start = stopwatches[n].total = GameCore.ticks;
    	} else {
        	long ticks = GameCore.ticks;
        	stopwatches[n].start = ticks - (stopwatches[n].total - stopwatches[n].start);
        	stopwatches[n].total = ticks;
    	}
    	stopwatches[n].isRunning = true;
	}

	void stopwatchStop(uint n) {
    	if(stopwatches[n].isRunning) {
        	stopwatches[n].total = GameCore.ticks;
    	}
    	stopwatches[n].isRunning = false;
	}

	float stopwatchCheck(uint n) {
    	if(stopwatches[n].isRunning) {
        	return ((GameCore.ticks- stopwatches[n].start));
    	}

    	return ((GameCore.ticks - stopwatches[n].total));
	}

	bool stopwatchState(uint n) {
    	return stopwatches[n].isRunning;
	}

	void init() {}
	void close() {}
};

} // namespace lvk
