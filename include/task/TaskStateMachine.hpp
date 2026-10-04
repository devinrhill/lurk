// Devin Hill 2026

#pragma once

#include "../Types.hpp"
#include "Task.hpp"

namespace lvk {

class TaskStateMachine: public Task {
public:
    enum Step {
        ENTER = 1,
        STEP = 2,
        EXIT = 3
    };

    struct State {
		static constexpr uint WORK_COUNT = 8;

        uint curState; // detail user-defined major state
        uint newState; // the next major state
        uint oldState; // the previous major state
        uint step; // minor state, Step enum
        uint* work; // per major state scratch space, 8 uints
        
		State() {
    		work = new uint[WORK_COUNT];
		}

		~State() {
    		delete[] work;
		}

		void clear() {
			curState = 0;
			oldState = 0;
			newState = 0;
			step = 0;
			clearWork();
		}

		void clearWork() {
			for(uint i = 0; i < STATE_COUNT; i++) {
				work[i] = 0;
			}
		}


    };

	static constexpr uint STATE_COUNT = 8;
    State states[STATE_COUNT];

	TaskStateMachine() {
    	setName("TaskStateMachine");
    	for(uint i = 0; i < STATE_COUNT; i++)
    		states[i] = {};
	}

	~TaskStateMachine() {
	}
};

} // namespace lvk
