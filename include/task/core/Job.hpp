// Devin Hill 2026

#pragma once

namespace lvk {

struct Job {
	void (*init)();
	void (*close)();
};

} // namespace lvk
