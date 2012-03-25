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

	event *eventList, *ptrAuxList, *ptrLastNode;
	machine *machineList, *ptrAuxMachine;
	task *taskList;
	job *jobList;
//	task *taskList, *ptrAuxTask;
	taskAccountInfo *taskAccountInfoList, *ptrAuxTaskAccountInfo;
//	gridInfo *gridInfoList;
	gridAccountInfo *gridInfoList, *ptrAuxGridInfo;

	if ( (eventList = malloc(sizeof(event))) ) {
		// start simulation event
		eventList->eventNumber = 0; eventList->eventID = 0; eventList->time = 0; eventList->flag = -1; eventList->nextEvent = NULL;
	} else printf("ERROR: merdou o malloc!!!\n");

	// finish simulation event
	if ( (ptrLastNode = malloc(sizeof(event))) ) {
		ptrLastNode->eventNumber = 1; ptrLastNode->eventID = SIMFINNISHED; ptrLastNode->time = 108001; ptrLastNode->flag = -1; ptrLastNode->nextEvent = NULL;
	} else printf("ERROR: merdou o malloc!!!\n");

	eventList->nextEvent = ptrLastNode;

	FillEmptyEventList(eventList);

	if ( (machineList = malloc(sizeof(machine))) ) {
		machineList->machineID = 0;		// 0 means code for an empty machine list
		machineList->nextMachine = NULL;
	}
	else {
		printf("ERROR: merdou o malloc!!!\n");
	}

	if ( (taskList = malloc(sizeof(task))) ) {
		taskList->taskID = 0;			// 0 means code for an empty task list
		taskList->nextTask = NULL;
	}
	else {
		printf("ERROR: merdou o malloc!!!\n");
	}

	if ( (jobList = malloc(sizeof(job))) ) {
		jobList->jobID = 0;			// 0 means code for an empty job list
		jobList->nextJob = NULL;
	}
	else {
		printf("ERROR: merdou o malloc!!!\n");
	}

	if ( (taskAccountInfoList = malloc(sizeof(taskAccountInfo))) ) {
		taskAccountInfoList->taskAccountID = 0;			// 0 means code for an empty task list
		taskAccountInfoList->status = ACCOUNTUNFINNISHED;
		taskAccountInfoList->nextTaskAccountInfo = NULL;
	}
	else {
		printf("ERROR: merdou o malloc!!!\n");
	}

	if ( (gridInfoList = malloc(sizeof(gridAccountInfo))) ) {
			gridInfoList->gridAccountID = 0;			// 0 means code for an empty task list
			gridInfoList->nextGridAccountInfo = NULL;
	}
	else {
			printf("ERROR: merdou o malloc!!!\n");
	}

	ptrAuxList = eventList;
	int count = 0;
	while(ptrAuxList) {
		count++;
//		printf("eventNumber %d eventID %d\n", ptrAuxList->eventNumber, ptrAuxList->eventID);

		switch (ptrAuxList->eventID) {
			case 0:
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList);
				break;
			case 1:				// Machine Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList);
				break;
			case 2:				// Machine Departure
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList);
				break;
			case 3:				// Grid Donating
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList);
				break;
			case 4:				// Grid Preempted
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList);
				break;
			case 5:				// Task Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList);
				break;
			case 6:				// Task Schedule
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList);
				break;
			case 7:				// Task Preempted
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList);
				break;
			case 8:				// Task Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList);
				break;
			case 9:				// Job Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList);
				break;
			case 10:			// Job Started
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList);
				break;
			case 11:			// Job Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList);
				break;
			case 12:				// Simulation Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &taskAccountInfoList, gridInfoList, jobList);
				break;
			default:
				printf("unkwonk event!!!\n");
				break;
		}

		ptrAuxList = ptrAuxList->nextEvent;
	}

	printf("a lista tem %d eventos\n", count);

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

	ptrAuxTaskAccountInfo = taskAccountInfoList;
	count = 0;
	while(ptrAuxTaskAccountInfo) {
		count++;
		printf("taskAccountID %d machineID %d source %d taskID %d jobID %d runtime %d ST %d FT %d\n", ptrAuxTaskAccountInfo->taskAccountID,
				ptrAuxTaskAccountInfo->machineID, ptrAuxTaskAccountInfo->source, ptrAuxTaskAccountInfo->taskID, ptrAuxTaskAccountInfo->jobID,
				(ptrAuxTaskAccountInfo->finnishTime-ptrAuxTaskAccountInfo->startTime), ptrAuxTaskAccountInfo->startTime, ptrAuxTaskAccountInfo->finnishTime);
		ptrAuxTaskAccountInfo = ptrAuxTaskAccountInfo->nextTaskAccountInfo;
	}

	printf("a lista tem %d registros\n", count);

	ptrAuxGridInfo = gridInfoList;
	count = 0;
	while(ptrAuxGridInfo) {
		count++;
		printf("gridAccountID %d machineID %d source %d ST %d FT %d\n", ptrAuxGridInfo->gridAccountID,
				ptrAuxGridInfo->machineID, ptrAuxGridInfo->source, ptrAuxGridInfo->startTime,
				ptrAuxGridInfo->finnishTime);
		ptrAuxGridInfo = ptrAuxGridInfo->nextGridAccountInfo;
	}

	printf("a lista tem %d balances\n", count);


	return EXIT_SUCCESS;
}
