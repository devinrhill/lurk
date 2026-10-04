// Devin Hill 2026

#pragma once

#include "Job.hpp"
#include "JobHandler.hpp"
#include "Task.hpp"
#include "gui/TaskGui.hpp"

namespace lvk {

class TaskSysCore {
public:
    JobHandler jb;

    Task ROOT;
    Task sysRoot;
    Task usrRoot;
    Task guiRoot;
    Task* updateParam;
    Task* drawParam;
    int drawStatus;
    void* mainApp;

	TaskSysCore() {
    	ROOT.setName("ROOT");
    	//ROOT.drawPriority = 0;
    	sysRoot.setName("Sys Root");
    	//sysRoot.drawPriority = 0;
    	usrRoot.setName("Usr Root");
    	//usrRoot.drawPriority = 0x10000;
    	guiRoot.setName("Gui Root");

    	ROOT.connect(&sysRoot);
    	ROOT.connect(&usrRoot);
    	ROOT.connect(&guiRoot);
    	//taskSortBuf = new Task*[512];
    	//memset(taskSortBuf, 0, sizeof(Task*) * 512);
    	updateParam = nullptr;
    	drawParam = nullptr;
    	drawStatus = 0;
    	//allowDrawFlags = Task::DrawClass::ALL;
    	//needSort = true;
    	//sorted = 0;
	}

	~TaskSysCore() {
    	//delete[] taskSortBuf
	}

	void setJobEnv(uint mask, uint shift, uint tableLen, Job* table) {
    	jb.setConfig(mask, shift, tableLen, table);
	}

	uint setJob(uint prm) {
    	uint result = jb.getCurJob();
    	jb.setNewJob(prm);
    	return result;
	}

	uint setJobForce(uint prm) {
    	uint result = jb.getCurJob();
    	jb.setNewJobForce(prm);
    	return result;
	}

	void registMainTask(Task* tsk) {
    	usrRoot.connect(tsk);
	}

	void unregistMainTask(Task* tsk) {
    	Task* t = (Task*)Node::searchComp((Node*)&ROOT, (Node*)tsk);
    	if(t != nullptr) {
        	t->close();
    	}
	}

	void registSysTask(Task* tsk) {
    	sysRoot.connect(tsk);
	}

	void unregistSysTask(Task* tsk) {
    	if(sysRoot.child != nullptr) {
        	sysRoot.child->close();
    	}
	}

	void registGuiTask(TaskGui* tsk) {
    	guiRoot.connect(tsk);
	}

	void unregistGuiTask(TaskGui* tsk) {
    	if(guiRoot.child != nullptr) {
        	guiRoot.child->close();
    	}
	}

	void* registMainApp(void* app) {
    	mainApp = app;
    	updateParam = (Task*)mainApp;
    	drawParam = (Task*)mainApp;

    	usrRoot.connect((Task*)mainApp);

    	return mainApp;
	}

	void unregistMainApp() {
    	((Task*)mainApp)->close();
	}

	void process() {
    	jobProc();
    	taskProc();
	}

	void jobProc() {
    	jb.execProc();
	}

	void taskProc() {
    	taskExecAll();
	}

	void taskExecAll() {
    	Task* root = &ROOT;
    	Task::preUpdateAll(root, updateParam);
    	Task::updateAll(root, updateParam);
    	Task::postUpdateAll(root, updateParam);
	}

	void taskDrawAll() {
    	Task::procDrawAll(&ROOT, drawStatus, drawParam, Task::DrawFlags::DRAW_HINT_ALL);
	}

	void taskDrawPre3D() {
    	Task::procDrawAll(&ROOT, drawStatus, drawParam, Task::DrawFlags::DRAW_PRE_3D);
	}

	void taskDraw3D() {
    	Task::procDrawAll(&ROOT, drawStatus, drawParam, Task::DrawFlags::DRAW_3D);
	}

	void taskDraw2D() {
    	Task::procDrawAll(&ROOT, drawStatus, drawParam, Task::DrawFlags::DRAW_2D);
	}

	void taskDrawPost2D() {
    	Task::procDrawAll(&ROOT, drawStatus, drawParam, Task::DrawFlags::DRAW_POST_2D);
	}
};

inline TaskSysCore TaskCore;

} // namespace lvk
