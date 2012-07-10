/*
 * fillemptyeventlist.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void FillEmptyEventList(event *ptrEventList) {

	event *auxPtrEvent1, *auxPtrEvent2;
	unsigned short int i, j;

	for (i = 1; i <= numberMachinesP1; i++) {

		if( (auxPtrEvent1 = malloc(sizeof(event))) ) {
			auxPtrEvent1->eventNumber = 0;
			auxPtrEvent1->eventID = MACHARRIVAL;
			auxPtrEvent1->time = 0;
			auxPtrEvent1->machineInfo.machineID = i;
			auxPtrEvent1->machineInfo.source = LOCAL;
			auxPtrEvent1->machineInfo.status = IDLE;
			auxPtrEvent1->machineInfo.arrivalTime = 0;
			auxPtrEvent1->machineInfo.departureTime = simulationTime;
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
			auxPtrEvent2->eventNumber = 0;
			auxPtrEvent2->eventID = MACHDEPARTURE;
			auxPtrEvent2->time = simulationTime;
			auxPtrEvent2->machineInfo.machineID = i;
			auxPtrEvent2->machineInfo.source = LOCAL;
			auxPtrEvent2->machineInfo.status = IDLE;
			auxPtrEvent2->machineInfo.arrivalTime = 0;
			auxPtrEvent2->machineInfo.departureTime = simulationTime;
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

	// filling event list with cloud machines
	for (i = 1; i <= numberMachinesP3; i++) {

		if( (auxPtrEvent1 = malloc(sizeof(event))) ) {
			auxPtrEvent1->eventNumber = 0;
			auxPtrEvent1->eventID = MACHARRIVAL;
			auxPtrEvent1->time = 0;
			auxPtrEvent1->machineInfo.machineID = i;
			auxPtrEvent1->machineInfo.source = CLOUD;
			auxPtrEvent1->machineInfo.status = IDLE;
			auxPtrEvent1->machineInfo.arrivalTime = 0;
			auxPtrEvent1->machineInfo.departureTime = simulationTime;
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
			auxPtrEvent2->eventNumber = 0;
			auxPtrEvent2->eventID = MACHDEPARTURE;
			auxPtrEvent2->time = simulationTime;
			auxPtrEvent2->machineInfo.machineID = i;
			auxPtrEvent2->machineInfo.source = CLOUD;
			auxPtrEvent2->machineInfo.status = IDLE;
			auxPtrEvent2->machineInfo.arrivalTime = 0;
			auxPtrEvent2->machineInfo.departureTime = simulationTime;
			auxPtrEvent2->machineInfo.usagePrice = 0.0;
			auxPtrEvent2->machineInfo.reservationPrice = 0.0;
			auxPtrEvent2->machineInfo.nextMachine = NULL;
			auxPtrEvent2->nextEvent = NULL;

			InsertEvent(ptrEventList, auxPtrEvent2);
		}
		else {
			printf("ERROR (fill): merdou o malloc!!!\n");
		}

	} // end for (filling event list with cloud machines)


	// task and job arrival events
	avgRunTime = 1800; jobSize = (JOB_AVG_LENGTH/avgRunTime);
	unsigned int jobArrivalTime = 43200;
	unsigned int numberOfJobs = (int)floor(simulationTime/DAY_TIME);
	unsigned int deadline, longestTask;

	for (i = 0; i < numberOfJobs; i++) {

		deadline = 0;
		longestTask = 0;

		for (j = 0; j < jobSize; j++) {

			if( (auxPtrEvent1 = malloc(sizeof(event))) ) {

				auxPtrEvent1->eventNumber = 0;
				auxPtrEvent1->eventID = TASKARRIVAL;
				auxPtrEvent1->time = jobArrivalTime;
				auxPtrEvent1->taskInfo.taskID = (j + 1);
				auxPtrEvent1->taskInfo.arrivalTime = jobArrivalTime;
				auxPtrEvent1->taskInfo.jobID = (i + 1);
				auxPtrEvent1->taskInfo.jobSize = jobSize;
				auxPtrEvent1->taskInfo.runtime = (int)Randn(avgRunTime, (avgRunTime * 0.1));
				auxPtrEvent1->taskInfo.status = QUEUED;
				auxPtrEvent1->taskInfo.nextTask = NULL;
				auxPtrEvent1->nextEvent = NULL;

				InsertEvent(ptrEventList, auxPtrEvent1);
				deadline += auxPtrEvent1->taskInfo.runtime;
				if (longestTask < auxPtrEvent1->taskInfo.runtime) longestTask = auxPtrEvent1->taskInfo.runtime;

			}
			else {
				printf("ERROR (fill): merdou o malloc!!!\n");
			}
		}

		if( (auxPtrEvent2 = malloc(sizeof(event))) ) {
			auxPtrEvent2->eventNumber = 0;
			auxPtrEvent2->eventID = JOBARRIVAL;
			auxPtrEvent2->time = jobArrivalTime;
			auxPtrEvent2->jobInfo.jobID = (i + 1);
			auxPtrEvent2->jobInfo.jobSize = jobSize;
			auxPtrEvent2->jobInfo.arrivalTime = jobArrivalTime;
			auxPtrEvent2->jobInfo.finnishTime = 0; // indicates a non-setted finnish time
			auxPtrEvent2->jobInfo.longestTask = longestTask;
			auxPtrEvent2->jobInfo.deadline = deadline;
			auxPtrEvent2->jobInfo.maxUtility = (10 * deadline);
			auxPtrEvent2->jobInfo.utility = 0.0;
			auxPtrEvent2->jobInfo.nextJob = NULL;
			auxPtrEvent2->nextEvent = NULL;

			InsertEvent(ptrEventList, auxPtrEvent2);
		}
		else {
			printf("ERROR (fill): merdou o malloc!!!\n");
		}

		jobArrivalTime += 86400;

	}

//	FILE *localMachines;
//	FILE *gridMachines;
//	FILE *workloadTasks;
//	FILE *workloadJobs;

//	localMachines = fopen("./src/local-machines.txt", "r");
//	localMachines = fopen(localMachinesTrace, "r");
//	if (!feof(localMachines)) {
////		printf("blz!!!\n");
//	}
//	else {
//		printf("ERROR (fill): merdou abrir arquivo!!!");
//		exit(1);
//	}

//	gridMachines = fopen("./src/grid-machines.txt", "r");
//	if (!feof(gridMachines)) {
////		printf("blz!!!\n");
//	}
//	else {
//		printf("ERROR (fill): merdou abrir arquivo!!!");
//		exit(1);
//	}

//	workloadTasks = fopen("./src/workload-tasks.txt", "r");
//	workloadTasks = fopen(workloadTasksTrace, "r");
//	if (!feof(workloadTasks)) {
//		printf("blz!!!\n");
//	}
//	else {
//		printf("ERROR (fill): merdou abrir arquivo!!!");
//		exit(1);
//	}

//	workloadJobs = fopen("./src/workload-jobs.txt", "r");
//	workloadJobs = fopen(workloadJobsTrace, "r");
//	if (!feof(workloadJobs)) {
//		printf("blz!!!\n");
//	}
//	else {
//		printf("ERROR (fill): merdou abrir arquivo!!!");
//		exit(1);
//	}

//	fclose(localMachines);
//	fclose(gridMachines);
//	fclose(workloadTasks);
//	fclose(workloadJobs);

}
