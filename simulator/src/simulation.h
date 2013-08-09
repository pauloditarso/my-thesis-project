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
#include <time.h>

# define DAY_TIME 1440 // maybe there is a better name
# define DEADLINE 840 // a deadline of 14 hours
//# define TASK_AVG_TIME 30 // 30 minutes
//# define TASK_SDV_TIME 3  // 3 minutes
//# define GRID_AVG_TIME 30
//# define GRID_SDV_TIME 3
//# define JOB_AVG_LENGTH 600 // 10 hours
//# define JOB_SDV_LENGTH 60 // 1 hours

unsigned short int optFlag;
unsigned long int simulationTime;
unsigned int taskAvgTime, taskSdvTime, gridAvgUptime;
float gridQoSFactor, reservedUsagePrice, ondemandUsagePrice, reservationPricePerDay, reservationPrice;
float ondemandPriceFactor, inhouseFactor; //, workloadFactor;
unsigned short int jobSize, simSeed, numberOfLocalMachines, numberOfReservedMachines, numberOfOnDemandMachines; // TAZER balance E numberOfGridMachines???
unsigned long int gridMachinesID, scheduleID;
enum {CONSTANT, LINEAR, STEP} utilityFunction;
float aggregatedUtility, aggregatedCost, aggregatedProfit;
//unsigned long int balanceCredit;
//char *localMachinesTrace;
//char *workloadTasksTrace;
//char *workloadJobsTrace;

typedef struct task {
	unsigned int taskID, jobID, jobSize, runtime;
	unsigned long int arrivalTime;
	enum {QUEUED, SCHEDULED, STARTED, FINNISHED} status;
	unsigned int numberOfSubmissions;
	struct task *nextTask;
} task;

typedef struct job {
	unsigned int jobID, jobSize, longestTask;
	unsigned long int arrivalTime, finnishTime, deadline;
//	enum {JOBQUEUED, JOBSTARTED, JOBFINNISHED} status;
//	unsigned long int utility;
	float maxUtility, utility, cost;
	struct job *nextJob;
} job;

typedef struct machine {
	unsigned int machineID;
	enum {LOCAL, GRID, RESERVED, ONDEMAND, SPOT} source;
//	unsigned short int capacity; // future implementation
	enum {IDLE, RUNNING, DONATING} status;
	unsigned long int arrivalTime, departureTime;
	float usagePrice, reservationPrice;
	struct machine *nextMachine;
} machine;

typedef struct machineOptSet {
	unsigned int machineID;
	enum {LOCALSET, GRIDSET, RESERVEDSET, ONDEMANDSET, SPOTSET} source;
	unsigned long int timeLeft, upTime;
	struct machineOptSet *nextMachineOptSet;
} machineOptSet;

typedef struct schedule {
	unsigned long int scheduleID, scheduleTime;
	unsigned int taskID, jobID, runtime, machineID;
	enum {MACHLOCAL, MACHGRID, MACHRESERVED, MACHONDEMAND, MACHSPOT} source;
	struct schedule *nextSchedule;
} schedule;

typedef struct scheduleQueue {
	unsigned long int targetFinnishtime;
	schedule *scheduleList;
	enum {UNFINNISHED, FAILED, SUCCESSFUL} status;
//	unsigned long int utility;
	float utility, cost, profit;
	struct scheduleQueue *previousSchedule;
} scheduleQueue;

typedef struct event {
	unsigned long int eventNumber;
	enum {SIMSTARTED, MACHARRIVAL, MACHDEPARTURE, GRIDDONATING, GRIDPREEMPTED, TASKARRIVAL, TASKSCHEDULE, TASKPREEMPTED,
	TASKFINNISHED, JOBARRIVAL, JOBSTARTED, JOBFINNISHED, ALLOCATIONPLANNING, SIMFINNISHED} eventID;
	unsigned long int time;
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
	unsigned int taskID, jobID, runtime;
	unsigned long int startTime, finnishTime;
	enum {ACCOUNTUNFINNISHED, ACCOUNTFINNISHED} status;
	float cost;
	struct taskAccountInfo *nextTaskAccountInfo;
} taskAccountInfo;

typedef struct jobAccountInfo {
	unsigned int jobAccountID;
	unsigned int jobID;
	unsigned long int startTime, finnishTime;
	struct jobAccountInfo *nextJobAccountInfo;
} jobAccountInfo;

typedef struct gridAccountInfo {
	unsigned int gridAccountID;
	unsigned int machineID;
	enum {GRIDLOCAL, GRIDGRID, GRIDRESERVED, GRIDONDEMAND, GRIDSPOT} source;
	unsigned long int startTime, finnishTime;
	struct gridAccountInfo *nextGridAccountInfo;
} gridAccountInfo;

typedef struct balanceAccountInfo {
	unsigned long int balanceAccountID;
	unsigned long int time;
	unsigned long int consumed;
	unsigned long int value;
	struct balanceAccountInfo *nextBalanceAccountInfo;
} balanceAccountInfo;

event *ptrThisEvent;
balanceAccountInfo *ptrLastBalance;

void passou();

void IncrementBalance(balanceAccountInfo *ptrBalanceAccountInfo, unsigned long int time, unsigned long int value);

void DecrementBalance(balanceAccountInfo *ptrBalanceAccountInfo, unsigned long int time, unsigned long int value);

unsigned int GetBalance(balanceAccountInfo **ptrPtrBalanceAccountInfo, unsigned long int time);

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

void JobFinnished(event *ptrCurrentEvent, jobAccountInfo *ptrJobAccountInfo, job *ptrJobList, task **ptrPtrTaskList);

void JobFinnishedOpt(event *ptrCurrentEvent, jobAccountInfo *ptrJobAccountInfo, job *ptrJobList, task **ptrPtrTaskList, task **ptrPtrOrderedTaskList);

void EventHandler(event *ptrCurrentEvent, event **ptrPtrEventList, machine **ptrPtrMachineList, task **ptrPtrTaskList, taskAccountInfo **ptrPtrTaskAccountInfoList,
		gridAccountInfo *ptrGridInfoList, job *ptrJobList, jobAccountInfo *ptrJobAccountInfo, balanceAccountInfo **ptrPtrBalanceAccountInfo, task **ptrPtrOrderedTaskList,
		schedule *ptrScheduleList);

void AllocationPlanning(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList, balanceAccountInfo **ptrPtrBalanceAccountInfo);

void AllocationPlanningOpt(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrOrderedTaskList, job *ptrJobList,
		balanceAccountInfo **ptrPtrBalanceAccountInfo);

double Randn(double mu, double sigma);

double Uniform(double a, double b);

#endif /* SIMULATION_H_ */
