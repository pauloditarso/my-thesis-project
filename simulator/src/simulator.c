/*
 ============================================================================
 Name        : simulator.c
 Author      : Paulo Ditarso
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "simulation.h"

int main(int argc, char *argv[]) {

	event *eventList, *ptrAuxList, *ptrLastNode, *ptrAuxAux;
	machine *machineList, *ptrAuxMachine;
	task *taskList, *ptrAuxTask;
	job *jobList, *ptrAuxJob;
	taskAccountInfo *taskAccountInfoList, *ptrAuxTaskAccountInfo;
	gridAccountInfo *gridInfoList, *ptrAuxGridInfo;
	jobAccountInfo *jobAccountInfoList, *ptrAuxJobAccountInfo;
	balanceAccountInfo *balanceAccountInfoList, *ptrAuxBalanceAccountInfo;

	unsigned short int numberMachinesP3 = atoi(argv[1]);

	// starting a new event list
	if ( (eventList = malloc(sizeof(event))) ) {
		// start simulation event
		eventList->eventNumber = 0; eventList->eventID = 0; eventList->time = 0; eventList->flag = -1; eventList->nextEvent = NULL;
	} else printf("ERROR: merdou o malloc!!!\n");

	// finish simulation event
	if ( (ptrLastNode = malloc(sizeof(event))) ) {
		ptrLastNode->eventNumber = 1; ptrLastNode->eventID = SIMFINNISHED; ptrLastNode->time = 108001; ptrLastNode->flag = -1; ptrLastNode->nextEvent = NULL;
	} else printf("ERROR: merdou o malloc!!!\n");

	eventList->nextEvent = ptrLastNode;

	// filling the event list with local and grid (???) machines, and workload jobs and tasks
	FillEmptyEventList(eventList, numberMachinesP3);

//	ptrAuxAux = eventList;
//	int count1 = 0;
//	while(ptrAuxAux) {
//		count1++;
//		if (ptrAuxAux->eventID == 1 || ptrAuxAux->eventID == 2) {
//			printf("eventID %d time %d machineID %d source %d status %d AT %d DT %d UP %f RP %f\n", ptrAuxAux->eventID, ptrAuxAux->time, ptrAuxAux->machineInfo.machineID,
//					ptrAuxAux->machineInfo.source, ptrAuxAux->machineInfo.status, ptrAuxAux->machineInfo.arrivalTime, ptrAuxAux->machineInfo.departureTime,
//					ptrAuxAux->machineInfo.usagePrice, ptrAuxAux->machineInfo.reservationPrice);
//		}
//		ptrAuxAux = ptrAuxAux->nextEvent;
//	}
//	printf("a lista tem %d eventos\n", count1);
//	printf("\n");

	// starting a machine list
	if ( (machineList = malloc(sizeof(machine))) ) {
		machineList->machineID = 0;		// 0 means code for an empty machine list
		machineList->nextMachine = NULL;
	}
	else {
		printf("ERROR: merdou o malloc!!!\n");
	}

	// starting a task list
	if ( (taskList = malloc(sizeof(task))) ) {
		taskList->taskID = 0;			// 0 means code for an empty task list
		taskList->nextTask = NULL;
	}
	else {
		printf("ERROR: merdou o malloc!!!\n");
	}

	// starting a job list
	if ( (jobList = malloc(sizeof(job))) ) {
		jobList->jobID = 0;			// 0 means code for an empty job list
		jobList->nextJob = NULL;
	}
	else {
		printf("ERROR: merdou o malloc!!!\n");
	}

	// starting an account list to register the execution of tasks
	if ( (taskAccountInfoList = malloc(sizeof(taskAccountInfo))) ) {
		taskAccountInfoList->taskAccountID = 0;			// 0 means code for an empty task list
		taskAccountInfoList->status = ACCOUNTUNFINNISHED;
		taskAccountInfoList->nextTaskAccountInfo = NULL;
	}
	else {
		printf("ERROR: merdou o malloc!!!\n");
	}

	// starting an account list to register the execution of jobs
	if ( (jobAccountInfoList = malloc(sizeof(jobAccountInfo))) ) {
		jobAccountInfoList->jobAccountID = 0;			// 0 means code for an empty task list
		jobAccountInfoList->nextJobAccountInfo = NULL;
	}
	else {
		printf("ERROR: merdou o malloc!!!\n");
	}

	// starting an account list to register the donations to the grid
	if ( (gridInfoList = malloc(sizeof(gridAccountInfo))) ) {
			gridInfoList->gridAccountID = 0;			// 0 means code for an empty task list
			gridInfoList->nextGridAccountInfo = NULL;
	}
	else {
			printf("ERROR: merdou o malloc!!!\n");
	}

	// starting a balance account list
	if ( (balanceAccountInfoList = malloc(sizeof(balanceAccountInfo))) ) {
			balanceAccountInfoList->balanceAccountID = 0;
			balanceAccountInfoList->time = 0;
			balanceAccountInfoList->value = 0;
			balanceAccountInfoList->nextBalanceAccountInfo = NULL;
	}
	else {
			printf("ERROR: merdou o malloc!!!\n");
	}

	ptrAuxList = eventList;
	int count = 0;
	while(ptrAuxList) {
		count++;
		printf("event# %d ", ptrAuxList->eventNumber);

		switch (ptrAuxList->eventID) {
			case 0:
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 1:				// Machine Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 2:				// Machine Departure
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 3:				// Grid Donating
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 4:				// Grid Preempted
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 5:				// Task Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 6:				// Task Schedule
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 7:				// Task Preempted
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 8:				// Task Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 9:				// Job Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 10:			// Job Started
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 11:			// Job Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			case 12:				// Simulation Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList, jobAccountInfoList,
						balanceAccountInfoList);
				break;
			default:
				printf("unkwonk event!!!\n");
				break;
		}

		ptrAuxList = ptrAuxList->nextEvent;
	}
	printf("a lista tem %d eventos\n", count);
	printf("\n");

	ptrAuxMachine = machineList;
	count = 0;
	while(ptrAuxMachine) {
		count++;
		printf("machineID %d source %d status %d AT %d DT %d UP %f RP %f\n", ptrAuxMachine->machineID,
				ptrAuxMachine->source, ptrAuxMachine->status, ptrAuxMachine->arrivalTime, ptrAuxMachine->departureTime,
				ptrAuxMachine->usagePrice, ptrAuxMachine->reservationPrice);
		ptrAuxMachine = ptrAuxMachine->nextMachine;
	}
	printf("a lista tem %d maquinas\n", count);
	printf("\n");

	ptrAuxTaskAccountInfo = taskAccountInfoList;
	count = 0;
	while(ptrAuxTaskAccountInfo) {
		count++;
		printf("taskAccountID %d machineID %d source %d taskID %d jobID %d runtime %d ST %d FT %d\n", ptrAuxTaskAccountInfo->taskAccountID,
				ptrAuxTaskAccountInfo->machineID, ptrAuxTaskAccountInfo->source, ptrAuxTaskAccountInfo->taskID, ptrAuxTaskAccountInfo->jobID,
				(ptrAuxTaskAccountInfo->finnishTime-ptrAuxTaskAccountInfo->startTime), ptrAuxTaskAccountInfo->startTime, ptrAuxTaskAccountInfo->finnishTime);
		ptrAuxTaskAccountInfo = ptrAuxTaskAccountInfo->nextTaskAccountInfo;
	}
	printf("a lista tem %d tasks\n", count);
	printf("\n");

	ptrAuxGridInfo = gridInfoList;
	count = 0;
	unsigned int credit = 0;
	while(ptrAuxGridInfo) {
		count++;
		credit += (ptrAuxGridInfo->finnishTime - ptrAuxGridInfo->startTime);
		printf("gridAccountID %d machineID %d source %d ST %d FT %d\n", ptrAuxGridInfo->gridAccountID,
				ptrAuxGridInfo->machineID, ptrAuxGridInfo->source, ptrAuxGridInfo->startTime,
				ptrAuxGridInfo->finnishTime);
		ptrAuxGridInfo = ptrAuxGridInfo->nextGridAccountInfo;
	}
	printf("a lista tem %d balances, e o total de creditos doados e %d\n", count, credit);
	printf("\n");

	ptrAuxJobAccountInfo = jobAccountInfoList;
	count = 0;
	while(ptrAuxJobAccountInfo) {
		count++;
		printf("jobAccountID %d jobID %d ST %d FT %d\n", ptrAuxJobAccountInfo->jobAccountID, ptrAuxJobAccountInfo->jobID, ptrAuxJobAccountInfo->startTime, ptrAuxJobAccountInfo->finnishTime);
		ptrAuxJobAccountInfo = ptrAuxJobAccountInfo->nextJobAccountInfo;
	}
	printf("a lista de jobs tem %d registros\n", count);
	printf("\n");

	ptrAuxBalanceAccountInfo = balanceAccountInfoList;
	count = 0;
	while(ptrAuxBalanceAccountInfo) {
		count++;
		printf("balanceAccountID %d time %d value %d\n", ptrAuxBalanceAccountInfo->balanceAccountID, ptrAuxBalanceAccountInfo->time, ptrAuxBalanceAccountInfo->value);
		ptrAuxBalanceAccountInfo = ptrAuxBalanceAccountInfo->nextBalanceAccountInfo;
	}
	printf("a lista de balances tem %d registros\n", count);
	printf("\n");

	ptrAuxJob = jobList;
	count = 0;
	while(ptrAuxJob) {
		count++;
		printf("jobID %d AR %d FT %d LT %d DL %d\n", ptrAuxJob->jobID, ptrAuxJob->arrivalTime, ptrAuxJob->finnishTime, ptrAuxJob->longestTask,
				ptrAuxJob->deadline);
		ptrAuxJob = ptrAuxJob->nextJob;
	}
	printf("a lista de jobs tem %d registros\n", count);
	printf("\n");

	ptrAuxTask = taskList;
	count = 0;
	while(ptrAuxTask) {
		count++;
		printf("taskID %d jobID %d jobSize %d AR %d RT %d status %d UF %f\n", ptrAuxTask->taskID, ptrAuxTask->jobID, ptrAuxTask->jobSize,
				ptrAuxTask->arrivalTime, ptrAuxTask->runtime, ptrAuxTask->status, ptrAuxTask->utilityFunction);
		ptrAuxTask = ptrAuxTask->nextTask;
	}
	printf("a lista de tasks tem %d registros\n", count);
	printf("\n");

	return EXIT_SUCCESS;
}
