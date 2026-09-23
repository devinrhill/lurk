#ifndef JOB_HPP
#define JOB_HPP

struct Job {
	void (*init)();
	void (*close)();
};

#endif // JOB_HPP
