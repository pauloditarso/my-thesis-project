/*
 * fillemptyeventlist.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void FillEmptyEventList(event *ptrEventList) {

	event *auxPtrEvent1, *auxPtrEvent2;

	FILE *localMachines;
	FILE *gridMachines;
	FILE *workloadTasks;

	localMachines = fopen("./src/local-machines.txt", "r");
	if (!feof(localMachines)) {
//		printf("blz!!!\n");
	}
	else {
		printf("ERROR (fill): merdou abrir arquivo!!!");
		exit(1);
	}

	gridMachines = fopen("./src/grid-machines.txt", "r");
	if (!feof(gridMachines)) {
//		printf("blz!!!\n");
	}
	else {
		printf("ERROR (fill): merdou abrir arquivo!!!");
		exit(1);
	}

	workloadTasks = fopen("./src/workload-tasks.txt", "r");
	if (!feof(workloadTasks)) {
//		printf("blz!!!\n");
	}
	else {
		printf("ERROR (fill): merdou abrir arquivo!!!");
		exit(1);
	}

	while(!feof(localMachines)) {

		int aux1, aux2, aux3;
		fscanf(localMachines, "%d %d %d", &aux1, &aux2, &aux3);

		if( (auxPtrEvent1 = malloc(sizeof(event))) ) {
			auxPtrEvent1->eventID = MACHARRIVAL;
			auxPtrEvent1->time = aux2;
			auxPtrEvent1->machineInfo.machineID = aux1;
			auxPtrEvent1->machineInfo.source = LOCAL;
			auxPtrEvent1->machineInfo.status = IDLE;
			auxPtrEvent1->machineInfo.arrivalTime = aux2;
			auxPtrEvent1->machineInfo.departureTime = aux3;
			auxPtrEvent1->machineInfo.usagePrice = 0.0;
			auxPtrEvent1->machineInfo.reservationPrice = 0.0;
			auxPtrEvent1->machineInfo.nextMachine = NULL;
			auxPtrEvent1->nextEvent = NULL;

			InsertEvent(ptrEventList, auxPtrEvent1);
		}
		else {
			printf("ERROR (fill): merdou o malloc!!!\n");
		}

		if( (auxPtrEvent2 = malloc(sizeof(event))) ) {
			auxPtrEvent2->eventID = MACHDEPARTURE;
			auxPtrEvent2->time = aux3;
			auxPtrEvent2->machineInfo.machineID = aux1;
			auxPtrEvent2->machineInfo.source = LOCAL;
			auxPtrEvent2->machineInfo.status = IDLE;
			auxPtrEvent2->machineInfo.arrivalTime = aux2;
			auxPtrEvent2->machineInfo.departureTime = aux3;
			auxPtrEvent2->machineInfo.usagePrice = 0.0;
			auxPtrEvent2->machineInfo.reservationPrice = 0.0;
			auxPtrEvent2->machineInfo.nextMachine = NULL;
			auxPtrEvent2->nextEvent = NULL;

			InsertEvent(ptrEventList, auxPtrEvent2);
		}
		else {
			printf("ERROR (fill): merdou o malloc!!!\n");
		}
	}

	while(!feof(gridMachines)) {

		int aux1, aux2, aux3;
		fscanf(gridMachines, "%d %d %d", &aux1, &aux2, &aux3);

		if( (auxPtrEvent1 = malloc(sizeof(event))) ) {
			auxPtrEvent1->eventID = MACHARRIVAL;
			auxPtrEvent1->time = aux2;
			auxPtrEvent1->machineInfo.machineID = aux1;
			auxPtrEvent1->machineInfo.source = GRID;
			auxPtrEvent1->machineInfo.status = IDLE;
			auxPtrEvent1->machineInfo.arrivalTime = aux2;
			auxPtrEvent1->machineInfo.departureTime = aux3;
			auxPtrEvent1->machineInfo.usagePrice = 0.0;
			auxPtrEvent1->machineInfo.reservationPrice = 0.0;
			auxPtrEvent1->machineInfo.nextMachine = NULL;
			auxPtrEvent1->nextEvent = NULL;

			InsertEvent(ptrEventList, auxPtrEvent1);
		}
		else {
			printf("ERROR (fill): merdou o malloc!!!\n");
		}

		if( (auxPtrEvent2 = malloc(sizeof(event))) ) {
			auxPtrEvent2->eventID = MACHDEPARTURE;
			auxPtrEvent2->time = aux3;
			auxPtrEvent2->machineInfo.machineID = aux1;
			auxPtrEvent2->machineInfo.source = GRID;
			auxPtrEvent2->machineInfo.status = IDLE;
			auxPtrEvent2->machineInfo.arrivalTime = aux2;
			auxPtrEvent2->machineInfo.departureTime = aux3;
			auxPtrEvent2->machineInfo.usagePrice = 0.0;
			auxPtrEvent2->machineInfo.reservationPrice = 0.0;
			auxPtrEvent2->machineInfo.nextMachine = NULL;
			auxPtrEvent2->nextEvent = NULL;

			InsertEvent(ptrEventList, auxPtrEvent2);
		}
		else {
			printf("ERROR (fill): merdou o malloc!!!\n");
		}
	}

	while(!feof(workloadTasks)) {

		int auxTaskID, auxTime, auxJobID, auxJobSize, auxRuntime;
		fscanf(workloadTasks, "%d %d %d %d %d", &auxTaskID, &auxTime, &auxJobID, &auxJobSize, &auxRuntime);

		if( (auxPtrEvent1 = malloc(sizeof(event))) ) {
			auxPtrEvent1->eventID = TASKARRIVAL;
			auxPtrEvent1->time = auxTime;
			auxPtrEvent1->taskInfo.taskID = auxTaskID;
			auxPtrEvent1->taskInfo.arrivalTime = auxTime;
			auxPtrEvent1->taskInfo.jobID = auxJobID;
			auxPtrEvent1->taskInfo.jobSize = auxJobSize;
			auxPtrEvent1->taskInfo.runtime = auxRuntime;
			auxPtrEvent1->taskInfo.status = QUEUED;
			auxPtrEvent1->taskInfo.utilityFunction = 0.0;
			auxPtrEvent1->taskInfo.nextTask = NULL;
			auxPtrEvent1->nextEvent = NULL;

			InsertEvent(ptrEventList, auxPtrEvent1);
		}
		else {
			printf("ERROR (fill): merdou o malloc!!!\n");
		}

	}

	fclose(localMachines);
	fclose(gridMachines);
	fclose(workloadTasks);
}
