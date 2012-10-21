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

	// filling event list with local machines
	for (i = 1; i <= numberOfLocalMachines; i++) {

		if( (auxPtrEvent1 = malloc(sizeof(event))) ) {
			auxPtrEvent1->eventNumber = 0;
			auxPtrEvent1->eventID = MACHARRIVAL;
			auxPtrEvent1->time = 1;
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
	} // end for (filling event list with local machines)

	// filling event list with reserved machines
	for (i = 1; i <= numberOfReservedMachines; i++) {

		if( (auxPtrEvent1 = malloc(sizeof(event))) ) {
			auxPtrEvent1->eventNumber = 0;
			auxPtrEvent1->eventID = MACHARRIVAL;
			auxPtrEvent1->time = 1;
			auxPtrEvent1->machineInfo.machineID = i;
			auxPtrEvent1->machineInfo.source = RESERVED;
			auxPtrEvent1->machineInfo.status = IDLE;
			auxPtrEvent1->machineInfo.arrivalTime = 0;
			auxPtrEvent1->machineInfo.departureTime = simulationTime;
			auxPtrEvent1->machineInfo.usagePrice = reservedUsagePrice;
			auxPtrEvent1->machineInfo.reservationPrice = reservationPrice;
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
			auxPtrEvent2->machineInfo.source = RESERVED;
			auxPtrEvent2->machineInfo.status = IDLE;
			auxPtrEvent2->machineInfo.arrivalTime = 0;
			auxPtrEvent2->machineInfo.departureTime = simulationTime;
			auxPtrEvent2->machineInfo.usagePrice = reservedUsagePrice;
			auxPtrEvent2->machineInfo.reservationPrice = reservationPrice;
			auxPtrEvent2->machineInfo.nextMachine = NULL;
			auxPtrEvent2->nextEvent = NULL;

			InsertEvent(ptrEventList, auxPtrEvent2);
		}
		else {
			printf("ERROR (fill): merdou o malloc!!!\n");
		}

	} // end for (filling event list with reserved machines)

	// filling event list with on-demand machines
	for (i = 1; i <= numberOfOnDemandMachines; i++) {

		if( (auxPtrEvent1 = malloc(sizeof(event))) ) {
			auxPtrEvent1->eventNumber = 0;
			auxPtrEvent1->eventID = MACHARRIVAL;
			auxPtrEvent1->time = 1;
			auxPtrEvent1->machineInfo.machineID = i;
			auxPtrEvent1->machineInfo.source = ONDEMAND;
			auxPtrEvent1->machineInfo.status = IDLE;
			auxPtrEvent1->machineInfo.arrivalTime = 0;
			auxPtrEvent1->machineInfo.departureTime = simulationTime;
			auxPtrEvent1->machineInfo.usagePrice = ondemandUsagePrice;
			auxPtrEvent1->machineInfo.reservationPrice = 0.00;
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
			auxPtrEvent2->machineInfo.source = ONDEMAND;
			auxPtrEvent2->machineInfo.status = IDLE;
			auxPtrEvent2->machineInfo.arrivalTime = 0;
			auxPtrEvent2->machineInfo.departureTime = simulationTime;
			auxPtrEvent2->machineInfo.usagePrice = ondemandUsagePrice;
			auxPtrEvent2->machineInfo.reservationPrice = 0.00;
			auxPtrEvent2->machineInfo.nextMachine = NULL;
			auxPtrEvent2->nextEvent = NULL;

			InsertEvent(ptrEventList, auxPtrEvent2);
		}
		else {
			printf("ERROR (fill): merdou o malloc!!!\n");
		}

	} // end for (filling event list with on-demand machines)

	// task and job arrival events
	unsigned int jobArrivalTime = (int)Randn(720, 60); // mean 12 hours; sd 1 hour;
	unsigned int maximumNumberOfJobs = (int)floor(simulationTime/DAY_TIME); // maximum of one job a day
	unsigned int deadline, longestTask, jobLength, jobSize, taskSdvTime;
	taskSdvTime = (int)taskAvgTime*0.1;

	for (i = 0; i < maximumNumberOfJobs; i++) {

		deadline = 2; // taking into account the 2 minutes between job arrival and task scheduling (allocation process)
		longestTask = 0;
		jobLength = (int)Randn(JOB_AVG_LENGTH, JOB_SDV_LENGTH);
		jobSize = (int)(jobLength/taskAvgTime) + 1; // garantir pelo menos uma task (RETIRAR ESSA SOMA???)

		for (j = 0; j < jobSize; j++) {

			if( (auxPtrEvent1 = malloc(sizeof(event))) ) {

				auxPtrEvent1->eventNumber = 0;
				auxPtrEvent1->eventID = TASKARRIVAL;
				auxPtrEvent1->time = jobArrivalTime;
				auxPtrEvent1->taskInfo.taskID = (j + 1);
				auxPtrEvent1->taskInfo.arrivalTime = jobArrivalTime;
				auxPtrEvent1->taskInfo.jobID = (i + 1);
				auxPtrEvent1->taskInfo.jobSize = jobSize;
				auxPtrEvent1->taskInfo.runtime = (int)Randn(taskAvgTime, taskSdvTime);
				auxPtrEvent1->taskInfo.status = QUEUED;
				auxPtrEvent1->taskInfo.numberOfSubmissions = 0;
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
			auxPtrEvent2->jobInfo.utility = 0;
			auxPtrEvent2->jobInfo.cost = 0.00;
			auxPtrEvent2->jobInfo.nextJob = NULL;
			auxPtrEvent2->nextEvent = NULL;

			InsertEvent(ptrEventList, auxPtrEvent2);
		}
		else {
			printf("ERROR (fill): merdou o malloc!!!\n");
		}

		jobArrivalTime += (int)Randn(DAY_TIME, 60); if (jobArrivalTime > 524880) break; // no arrivals after noom of the 365th day

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
