/*
 * simulation.h
 *
 *  Created on: Aug 14, 2011
 *      Author: PauloDitarso
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <stdio.h>
#include <stdlib.h>

#define TIME 108000

typedef struct task {
	unsigned int taskID, arrivalTime, jobID, jobSize, runtime;
	enum {QUEUED, STARTED, FINNISHED} status;
	float utilityFunction;
	struct task *nextTask;
} task;

typedef struct job {
	unsigned int jobID, arrivalTime, finnishTime, longestTask, deadline;
	struct job *nextJob;
} job;

typedef struct machine {
	unsigned int machineID;
	enum {LOCAL, GRID, CLOUD} source;
//	unsigned short int capacity; // future implementation
	enum {IDLE, RUNNING, DONATING} status;
	unsigned int arrivalTime, departureTime;
	float usagePrice, reservationPrice;
	struct machine *nextMachine;
} machine;

typedef struct event {
	unsigned int eventNumber;
	enum {SIMSTARTED, MACHARRIVAL, MACHDEPARTURE, GRIDDONATING, GRIDPREEMPTED, TASKARRIVAL, TASKSCHEDULE, TASKPREEMPTED,
	TASKFINNISHED, JOBARRIVAL, JOBSTARTED, JOBFINNISHED, SIMFINNISHED} eventID;
	unsigned int time;
	union {
		task taskInfo;
		job jobInfo;
		machine machineInfo;
		short int flag;
	};
	struct event *nextEvent;
} event;

typedef struct taskAccountInfo {
	unsigned int taskAccountID;
	unsigned int machineID;
	enum {LOCALMACH, GRIDMACH, CLOUDMACH} source;
	unsigned int taskID, jobID;
	unsigned int startTime, finnishTime;
	enum {ACCOUNTUNFINNISHED, ACCOUNTFINNISHED} status;
	struct taskAccountInfo *nextTaskAccountInfo;
} taskAccountInfo;

typedef struct jobAccountInfo {
	unsigned int jobAccountID;
//	unsigned int machineID;
//	enum {JOBLOCALMACH, JOBGRIDMACH, JOBCLOUDMACH} source;
//	unsigned int taskID, jobID;
	unsigned int startTime, finnishTime;
	struct jobAccountInfo *nextJobAccountInfo;
} jobAccountInfo;

typedef struct gridAccountInfo {
	unsigned int gridAccountID;
	unsigned int machineID;
	enum {GRIDLOCAL, GRIDGRID, GRIDCLOUD} source;
	unsigned int startTime, finnishTime;
//	unsigned int girdAccumulatedBalance;
	struct gridAccountInfo *nextGridAccountInfo;
} gridAccountInfo;

//machine *ReturnCorrectMachine(event *ptrCurrentEvent, machine *ptrMachineList);

void InsertEvent(event *ptrEventList, event *ptrNewEvent);

void InsertAfterEvent(event *ptrEventList, event *ptrNewEvent, event *ptrTargetEvent);

void RemoveEvent(event **ptrPtrEventList, event *ptrOldEvent);

void FillEmptyEventList(event *ptrEventList);

void MachineArrival(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList);

void MachineDeparture(event *ptrCurrentEvent, event *ptrEventList, machine **ptrPtrMachineList);

void GridDonating(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, gridAccountInfo *ptrGridInfoList);

void GridPreempted(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, gridAccountInfo *ptrGridInfoList);

void InsertGridAccountList(event *ptrCurrentEvent, machine *ptrAuxMachine, gridAccountInfo *ptrGridInfoList);

void TaskArrival(event *ptrCurrentEvent, event *ptrEventList, task *ptrTaskList);

void TaskFinnished(event *ptrCurrentEvenet, event *ptrEventList, task *ptrTaskList, taskAccountInfo *ptrTaskAccountInfoList, machine *ptrMachineList);

void InsertTaskAccountList(event *ptrCurrentEvent, machine *ptrAuxMachine,  task *ptrAuxTask, taskAccountInfo *ptrTaskAccountInfoList);

void InsertJobAccountList(event *ptrCurrentEvent, job *ptrAuxJob, jobAccountInfo *ptrJobAccountInfoList);

void RemoveTaskAccountList(taskAccountInfo **ptrPtrTaskAccountInfoList, taskAccountInfo *ptrOldTaskAccount);

void TaskSchedule(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList, taskAccountInfo *ptrTaskAccountInfoList);

void TaskUnSchedule(event *ptrCurrentEvent, event **ptrPtrEventList, machine *ptrMachineList, task *ptrTaskList, taskAccountInfo **ptrPtrTaskAccountInfoList);

void JobArrival(event *ptrCurrentEvent, event *ptrEventList, job *ptrJobList);

void JobStarted(event *ptrCurrentEvent, event *ptrEventList, job *ptrJobList);

void JobFinnished(event *ptrCurrentEvent, event *ptrEventList, job *ptrJobList);

void EventHandler(event *ptrCurrentEvent, event **ptrPtrEventList, machine **ptrPtrMachineList, task *ptrTaskList, taskAccountInfo **ptrPtrTaskAccountInfoList,
		gridAccountInfo *ptrGridInfoList, job *ptrJobList);

#endif /* SIMULATION_H_ */
