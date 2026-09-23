#ifndef JOB_HANDLER_HPP
#define JOB_HANDLER_HPP

#include "Job.hpp"
#include <cstdint>
#include <cstdlib>
#include "../Middle.hpp"
#include "../util/Util.hpp"

struct JobHandler {
	bool force;
	uint newJob;
	uint curJob;
	uint oldJob;
	uint mask;
	uint shift;
	uint jobCount;
	Job* jobs;

	JobHandler() {
		force = false;
		newJob = UINT32_MAX;
		curJob = UINT32_MAX;
		oldJob = UINT32_MAX;
		mask = UINT32_MAX;
		shift = 0;
		jobCount = 0;
		jobs = nullptr;
	}

	~JobHandler() {
		if(jobs != nullptr) {
			free(jobs);
		}
	}

	void setConfig(uint mask, uint shift, uint jobCount, Job* jobs) {
		this->mask = mask;
		this->shift = shift;
		this->jobCount = jobCount;
		this->jobs = (Job*)zalloc(jobCount * sizeof(Job));
		memcpy(this->jobs, jobs, jobCount * sizeof(Job));
	}

	void setNewJob(uint newJob) {
		this->newJob = newJob;
	}

	void setNewJobForce(uint newJob) {
		this->newJob = newJob;
		this->force = true;
	}

	uint getCurJob() {
		return this->curJob;
	}

	uint getOldJob() {
		return this->oldJob;
	}

	uint getJobIndex(uint num) {
		return (num >> (int)this->shift) & this->mask;
	}

	uint getJobIndexAbove(uint num) {
		return (num >> (int)this->shift) & ~this->mask;
	}

	bool isJobChanged(uint num0, uint num1) {
		return num0 >> (int)this->shift != num1 >> (int)this->shift;
	}

	bool isJobChangedAbove(uint num0, uint num1) {
		return getJobIndexAbove(num0) != getJobIndexAbove(num1);
	}

	bool execProc() {
		if(jobs != nullptr && (force || isJobChanged(newJob, curJob))) {
			force = false;
			if(curJob != -1) {
				Job job = jobs[getJobIndex(curJob)];
				if(job.close != nullptr) {
					job.close();
				}
			}
			if(isJobChangedAbove(newJob, curJob) && curJob != -1) {
				newJob = -1;
			}
			oldJob = curJob;
			curJob = newJob;
			if(newJob != -1) {
				Job job = jobs[getJobIndex(newJob)];
				if(job.init != nullptr) {
					job.init();
				}
			}
		}

		return curJob != -1;
	}
};

#endif // JOB_HANDLER_HPP
