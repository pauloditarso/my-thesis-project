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

//#define TIME 108000
# define TASK_AVG_TIME 1800
# define TASK_SDV_TIME 300

unsigned short int optFlag;
unsigned int simulationTime;
float gridQoSFactor;
unsigned int gridMachinesID;


typedef struct task {
	unsigned int taskID, arrivalTime, jobID, jobSize, runtime;
	enum {QUEUED, STARTED, FINNISHED} status;
	float utilityFunction;
	struct task *nextTask;
} task;

typedef struct job {
	unsigned int jobID, jobSize, arrivalTime, finnishTime, longestTask, deadline;
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

typedef struct schedule {
	unsigned int taskID, jobID;
	unsigned int machineID;
	enum {MACHLOCAL, MACHGRID, MACHCLOUD} source;
} schedule;

typedef struct event {
	unsigned int eventNumber;
	enum {SIMSTARTED, MACHARRIVAL, MACHDEPARTURE, GRIDDONATING, GRIDPREEMPTED, TASKARRIVAL, TASKSCHEDULE, TASKPREEMPTED,
	TASKFINNISHED, JOBARRIVAL, JOBSTARTED, JOBFINNISHED, ALLOCATIONPLANNING, SIMFINNISHED} eventID;
	unsigned int time;
	union {
		task taskInfo;
		job jobInfo;
		machine machineInfo;
		schedule scheduleInfo;
		short int flag;
	};
	struct event *nextEvent;
} event;

typedef struct taskAccountInfo {
	unsigned int taskAccountID;
	unsigned int machineID;
	enum {LOCALMACH, GRIDMACH, CLOUDMACH} source;
	unsigned int taskID, jobID;
	unsigned int runtime, startTime, finnishTime;
	enum {ACCOUNTUNFINNISHED, ACCOUNTFINNISHED} status;
	struct taskAccountInfo *nextTaskAccountInfo;
} taskAccountInfo;

typedef struct jobAccountInfo {
	unsigned int jobAccountID;
	unsigned int jobID;
	unsigned int startTime, finnishTime;
	struct jobAccountInfo *nextJobAccountInfo;
} jobAccountInfo;

typedef struct gridAccountInfo {
	unsigned int gridAccountID;
	unsigned int machineID;
	enum {GRIDLOCAL, GRIDGRID, GRIDCLOUD} source;
	unsigned int startTime, finnishTime;
	struct gridAccountInfo *nextGridAccountInfo;
} gridAccountInfo;

typedef struct balanceAccountInfo {
	unsigned int balanceAccountID;
	unsigned int time;
	unsigned int value;
	struct balanceAccountInfo *nextBalanceAccountInfo;
} balanceAccountInfo;

void passou();

void IncrementBalance(balanceAccountInfo *ptrBalanceAccountInfo, unsigned int time, unsigned int value);

void DecrementBalance(balanceAccountInfo *ptrBalanceAccountInfo, unsigned int time, unsigned int value);

unsigned int GetBalance(balanceAccountInfo *ptrBalanceAccountInfo, unsigned int time);

void InsertEvent(event *ptrEventList, event *ptrNewEvent);

void InsertAfterEvent(event *ptrEventList, event *ptrNewEvent, event *ptrTargetEvent);

void RemoveEvent(event **ptrPtrEventList, event *ptrOldEvent);

void FillEmptyEventList(event *ptrEventList, unsigned short int numberMachinesP3);

void MachineArrival(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList);

void MachineDeparture(event *ptrCurrentEvent, event *ptrEventList, machine **ptrPtrMachineList, taskAccountInfo *ptrTaskAccountInfoList);

void GridDonating(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, gridAccountInfo *ptrGridInfoList);

void GridPreempted(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, gridAccountInfo *ptrGridInfoList,
		balanceAccountInfo *ptrBalanceAccountInfo);

void InsertGridAccountList(event *ptrCurrentEvent, machine *ptrAuxMachine, gridAccountInfo *ptrGridInfoList);

void TaskArrival(event *ptrCurrentEvent, event *ptrEventList, task *ptrTaskList, machine *ptrMachineList);

void TaskFinnished(event *ptrCurrentEvenet, event *ptrEventList, task *ptrTaskList, taskAccountInfo *ptrTaskAccountInfoList, machine *ptrMachineList,
		balanceAccountInfo *ptrBalanceAccountInfo);

void InsertTaskAccountList(event *ptrCurrentEvent, machine *ptrAuxMachine,  task *ptrAuxTask, taskAccountInfo *ptrTaskAccountInfoList);

void InsertJobAccountList(event *ptrCurrentEvent, jobAccountInfo *ptrJobAccountInfoList);

void RemoveTaskAccountList(taskAccountInfo **ptrPtrTaskAccountInfoList, taskAccountInfo *ptrOldTaskAccount);

void TaskSchedule(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList, taskAccountInfo *ptrTaskAccountInfoList);

void TaskUnSchedule(event *ptrCurrentEvent, event **ptrPtrEventList, machine *ptrMachineList, task *ptrTaskList, taskAccountInfo **ptrPtrTaskAccountInfoList,
		balanceAccountInfo *ptrBalanceAccountInfo);

void JobArrival(event *ptrCurrentEvent, event *ptrEventList, job *ptrJobList, task *ptrTaskList, machine *ptrMachineList, balanceAccountInfo *ptrBalanceAccountInfo);

void JobStarted(event *ptrCurrentEvent, jobAccountInfo *ptrJobAccountInfo, job *ptrJobList);

void JobFinnished(event *ptrCurrentEvent, jobAccountInfo *ptrJobAccountInfo, job *ptrJobList);

void EventHandler(event *ptrCurrentEvent, event **ptrPtrEventList, machine **ptrPtrMachineList, task *ptrTaskList, taskAccountInfo **ptrPtrTaskAccountInfoList,
		gridAccountInfo *ptrGridInfoList, job *ptrJobList, jobAccountInfo *ptrJobAccountInfo, balanceAccountInfo *ptrBalanceAccountInfo);

void AllocationPlanning(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList, balanceAccountInfo *ptrBalanceAccountInfo);

#endif /* SIMULATION_H_ */
