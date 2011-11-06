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
//	task *taskList, *ptrAuxTask;
	accountInfo *accountInfoList, *ptrAuxAccountInfo;

	if ( (eventList = malloc(sizeof(event))) ) {
		// start simulation event
		eventList->eventID = 0; eventList->time = 0; eventList->flag = -1; eventList->nextEvent = NULL;
	} else printf("ERROR: merdou o malloc!!!\n");

	// finish simulation event
	if ( (ptrLastNode = malloc(sizeof(event))) ) {
		ptrLastNode->eventID = SIMFINNISHED; ptrLastNode->time = 108001; ptrLastNode->flag = -1; ptrLastNode->nextEvent = NULL;
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

	if ( (accountInfoList = malloc(sizeof(accountInfo))) ) {
		accountInfoList->accountID = 0;			// 0 means code for an empty task list
		accountInfoList->nextAccountInfo = NULL;
	}
	else {
		printf("ERROR: merdou o malloc!!!\n");
	}

	ptrAuxList = eventList;
	int count = 0;
	while(ptrAuxList) {
		count++;

		switch (ptrAuxList->eventID) {
			case 0:
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &accountInfoList);
				break;
			case 1:				// Machine Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &accountInfoList);
				break;
			case 2:				// Machine Departure
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &accountInfoList);
				break;
			case 3:				// Grid Donating
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &accountInfoList);
				break;
			case 4:				// Grid Preempted
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &accountInfoList);
				break;
			case 5:				// Task Arrival
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &accountInfoList);
				break;
			case 6:				// Task Schedule
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &accountInfoList);
				break;
			case 7:				// Task Preempted
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &accountInfoList);
				break;
			case 8:				// Task Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &accountInfoList);
				break;
			case 9:				// Simulation Finnished
				EventHandler(ptrAuxList, &eventList, &machineList, taskList, &accountInfoList);
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

	ptrAuxAccountInfo = accountInfoList;
	count = 0;
	while(ptrAuxAccountInfo) {
		count++;
		printf("accountID %d machineID %d source %d taskID %d jobID %d ST %d FT %d\n", ptrAuxAccountInfo->accountID,
				ptrAuxAccountInfo->machineID, ptrAuxAccountInfo->source, ptrAuxAccountInfo->taskID, ptrAuxAccountInfo->jobID,
				ptrAuxAccountInfo->startTime, ptrAuxAccountInfo->finnishTime);
		ptrAuxAccountInfo = ptrAuxAccountInfo->nextAccountInfo;
	}

	printf("a lista tem %d registros\n", count);

	return EXIT_SUCCESS;
}
