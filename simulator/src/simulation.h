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
#include <math.h>

# define DAY_TIME 1440 // maybe there is a better name
# define TASK_AVG_TIME 30 // 30 minutes
# define TASK_SDV_TIME 3  // 3 minutes
# define GRID_AVG_TIME 30
# define GRID_SDV_TIME 3
# define JOB_AVG_LENGTH 600 // 10 hours
# define JOB_SDV_LENGTH 120 // 2 hours

unsigned short int optFlag;
unsigned int simulationTime;
float gridQoSFactor, reservedUsagePrice, ondemandUsagePrice, reservationPricePerDay, reservationPrice;
unsigned short int simSeed, numberOfLocalMachines, numberOfReservedMachines, numberOfOnDemandMachines; // TAZER balance E numberOfGridMachines???
unsigned int gridMachinesID, scheduleID;
enum {CONSTANT, LINEAR, STEP} utilityFunction;
//char *localMachinesTrace;
//char *workloadTasksTrace;
//char *workloadJobsTrace;

typedef struct task {
	unsigned int taskID, jobID, jobSize, arrivalTime, runtime;
	enum {QUEUED, SCHEDULED, STARTED, FINNISHED} status;
	unsigned short int numberOfSubmissions;
	struct task *nextTask;
} task;

typedef struct job {
	unsigned int jobID, jobSize, arrivalTime, finnishTime, longestTask, deadline, maxUtility;
//	enum {JOBQUEUED, JOBSTARTED, JOBFINNISHED} status;
	int utility;
	float cost;
	struct job *nextJob;
} job;

typedef struct machine {
	unsigned int machineID;
	enum {LOCAL, GRID, RESERVED, ONDEMAND, SPOT} source;
//	unsigned short int capacity; // future implementation
	enum {IDLE, RUNNING, DONATING} status;
	unsigned int arrivalTime, departureTime;
	float usagePrice, reservationPrice;
	struct machine *nextMachine;
} machine;

typedef struct machineOptSet {
	unsigned int machineID;
	enum {LOCALSET, GRIDSET, RESERVEDSET, ONDEMANDSET, SPOTSET} source;
	unsigned int timeLeft, upTime;
	struct machineOptSet *nextMachineOptSet;
} machineOptSet;

typedef struct schedule {
	unsigned int scheduleID, scheduleTime, taskID, jobID, runtime, machineID;
	enum {MACHLOCAL, MACHGRID, MACHRESERVED, MACHONDEMAND, MACHSPOT} source;
	struct schedule *nextSchedule;
} schedule;

typedef struct scheduleQueue {
	unsigned int targetFinnishtime;
	schedule *scheduleList;
	enum {UNFINNISHED, FAILED, SUCCESSFUL} status;
	float profit;
	struct scheduleQueue *previousSchedule;
} scheduleQueue;

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
	enum {LOCALMACH, GRIDMACH, RESERVEDMACH, ONDEMANDMACH, SPOTMACH} source;
	unsigned int taskID, jobID;
	unsigned int runtime, startTime, finnishTime;
	enum {ACCOUNTUNFINNISHED, ACCOUNTFINNISHED} status;
	float cost;
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
	enum {GRIDLOCAL, GRIDGRID, GRIDRESERVED, GRIDONDEMAND, GRIDSPOT} source;
	unsigned int startTime, finnishTime;
	struct gridAccountInfo *nextGridAccountInfo;
} gridAccountInfo;

typedef struct balanceAccountInfo {
	unsigned int balanceAccountID;
	unsigned int time;
	unsigned int consumed;
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

void FillEmptyEventList(event *ptrEventList);

void MachineArrival(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList);

void MachineDeparture(event *ptrCurrentEvent, event *ptrEventList, machine **ptrPtrMachineList, taskAccountInfo *ptrTaskAccountInfoList);

void GridDonating(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, gridAccountInfo *ptrGridInfoList);

void GridPreempted(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, gridAccountInfo *ptrGridInfoList,
		balanceAccountInfo *ptrBalanceAccountInfo);

void InsertGridAccountList(event *ptrCurrentEvent, machine *ptrAuxMachine, gridAccountInfo *ptrGridInfoList);

void TaskArrival(event *ptrCurrentEvent, event *ptrEventList, task *ptrTaskList, machine *ptrMachineList, task **ptrPtrOrderedTaskList);

void TaskFinnished(event *ptrCurrentEvenet, event **ptrPtrEventList, task *ptrTaskList, taskAccountInfo *ptrTaskAccountInfoList, machine *ptrMachineList,
		balanceAccountInfo *ptrBalanceAccountInfo, job *ptrJobList);

void TaskFinnishedOpt(event *ptrCurrentEvenet, event **ptrPtrEventList, task *ptrTaskList, taskAccountInfo *ptrTaskAccountInfoList, machine *ptrMachineList,
		balanceAccountInfo *ptrBalanceAccountInfo, job *ptrJobList);

void InsertTaskAccountList(event *ptrCurrentEvent, machine *ptrAuxMachine,  task *ptrAuxTask, taskAccountInfo *ptrTaskAccountInfoList);

void InsertJobAccountList(event *ptrCurrentEvent, jobAccountInfo *ptrJobAccountInfoList);

void RemoveTaskAccountList(taskAccountInfo **ptrPtrTaskAccountInfoList, taskAccountInfo *ptrOldTaskAccount);

void TaskSchedule(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList, taskAccountInfo *ptrTaskAccountInfoList,
		schedule *ptrScheduleList);

void TaskUnSchedule(event *ptrCurrentEvent, event **ptrPtrEventList, machine *ptrMachineList, task *ptrTaskList, taskAccountInfo **ptrPtrTaskAccountInfoList,
		balanceAccountInfo *ptrBalanceAccountInfo, job *ptrJobList);

void JobArrival(event *ptrCurrentEvent, event *ptrEventList, job *ptrJobList, task *ptrTaskList, machine *ptrMachineList, balanceAccountInfo *ptrBalanceAccountInfo);

void JobStarted(event *ptrCurrentEvent, jobAccountInfo *ptrJobAccountInfo, job *ptrJobList);

void JobFinnished(event *ptrCurrentEvent, jobAccountInfo *ptrJobAccountInfo, job *ptrJobList);

void EventHandler(event *ptrCurrentEvent, event **ptrPtrEventList, machine **ptrPtrMachineList, task *ptrTaskList, taskAccountInfo **ptrPtrTaskAccountInfoList,
		gridAccountInfo *ptrGridInfoList, job *ptrJobList, jobAccountInfo *ptrJobAccountInfo, balanceAccountInfo *ptrBalanceAccountInfo, task **ptrPtrOrderedTaskList,
		schedule *ptrScheduleList);

void AllocationPlanning(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList, balanceAccountInfo *ptrBalanceAccountInfo);

void AllocationPlanningOpt(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrOrderedTaskList, balanceAccountInfo *ptrBalanceAccountInfo);

double Randn(double mu, double sigma);

#endif /* SIMULATION_H_ */
