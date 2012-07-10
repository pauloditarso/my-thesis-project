/*
 * jobarrival.c
 *
 *  Created on: Jan 23, 2012
 *      Author: PauloDitarso
 *
 *  What is done:
 *  	1. it tests if it is the correct event;
 *  	2. it tests if thereis a job list;
 *  	3. it creates a new node in the job list;
 *  	4. it calls AllocationPlanning(); (???)
 */

#include "simulation.h"

void JobArrival(event *ptrCurrentEvent, event *ptrEventList, job *ptrJobList, task *ptrTaskList, machine *ptrMachineList, balanceAccountInfo *ptrBalanceAccountInfo) {

	if (ptrCurrentEvent->eventID == JOBARRIVAL) {

		// creation of a new node in the job list
		if (ptrJobList) {

			if (ptrJobList->jobID == 0) {	// code for an empty job list
				ptrJobList->jobID = ptrCurrentEvent->jobInfo.jobID;
				ptrJobList->jobSize = ptrCurrentEvent->jobInfo.jobSize;
				ptrJobList->arrivalTime = ptrCurrentEvent->jobInfo.arrivalTime;
				ptrJobList->finnishTime = ptrCurrentEvent->jobInfo.finnishTime;
				ptrJobList->longestTask = ptrCurrentEvent->jobInfo.longestTask;
				ptrJobList->deadline = ptrCurrentEvent->jobInfo.deadline;
				ptrJobList->maxUtility = ptrCurrentEvent->jobInfo.maxUtility;
				ptrJobList->utility = ptrCurrentEvent->jobInfo.utility;
				ptrJobList->nextJob = NULL;
			}
			else {

				job *ptrNewJob;

				if ( !(ptrNewJob = malloc(sizeof(job))) ) {
					printf("ERROR (job arrival): merdou o malloc!!!\n");
				}

				ptrNewJob->jobID = ptrCurrentEvent->jobInfo.jobID;
				ptrNewJob->jobSize = ptrCurrentEvent->jobInfo.jobSize;
				ptrNewJob->arrivalTime = ptrCurrentEvent->jobInfo.arrivalTime;
				ptrNewJob->finnishTime = ptrCurrentEvent->jobInfo.finnishTime;
				ptrNewJob->longestTask = ptrCurrentEvent->jobInfo.longestTask;
				ptrNewJob->deadline = ptrCurrentEvent->jobInfo.deadline;
				ptrNewJob->maxUtility = ptrCurrentEvent->jobInfo.maxUtility;
				ptrNewJob->utility = ptrCurrentEvent->jobInfo.utility;
				ptrNewJob->nextJob = NULL;

				job *ptrAux;

				ptrAux = ptrJobList;

				while(ptrAux->nextJob != NULL) {
					ptrAux = ptrAux->nextJob;
				}

				ptrAux->nextJob = ptrNewJob;

			}

			//	printf("o balance na grade agora e %d\n", GetBalance(ptrBalanceAccountInfo, ptrCurrentEvent->time));
			printf("eventID %d (Job Arrival) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			printf("jobID %d jobSize %d AT %d FT %d LT %d Deadline %d MU %d Utility %d\n", ptrCurrentEvent->jobInfo.jobID, ptrCurrentEvent->jobInfo.jobSize,
					ptrCurrentEvent->jobInfo.arrivalTime, ptrCurrentEvent->jobInfo.finnishTime, ptrCurrentEvent->jobInfo.longestTask,
					ptrCurrentEvent->jobInfo.deadline, ptrCurrentEvent->jobInfo.maxUtility, ptrCurrentEvent->jobInfo.utility);

			// if there is donating machines, it creates grid preempted events
			machine *ptrAuxMachine;
			ptrAuxMachine = ptrMachineList;
			while(ptrAuxMachine) {

				if (ptrAuxMachine->source == LOCAL && ptrAuxMachine->status == DONATING) {

					ptrAuxMachine->status = IDLE;
					event *ptrNewGridPreemption;

					if( (ptrNewGridPreemption = malloc(sizeof(event))) ) {
						ptrNewGridPreemption->eventNumber = 0;
						ptrNewGridPreemption->eventID = GRIDPREEMPTED;
						ptrNewGridPreemption->time = ptrCurrentEvent->time;
						ptrNewGridPreemption->machineInfo.machineID = ptrAuxMachine->machineID;
						ptrNewGridPreemption->machineInfo.source = ptrAuxMachine->source;
						ptrNewGridPreemption->machineInfo.status = QUEUED;
						ptrNewGridPreemption->machineInfo.arrivalTime = ptrAuxMachine->arrivalTime;
						ptrNewGridPreemption->machineInfo.departureTime = ptrAuxMachine->departureTime;
						ptrNewGridPreemption->machineInfo.reservationPrice = ptrAuxMachine->reservationPrice;
						ptrNewGridPreemption->machineInfo.usagePrice = ptrAuxMachine->usagePrice;
						ptrNewGridPreemption->machineInfo.nextMachine = ptrAuxMachine->nextMachine;
						ptrNewGridPreemption->nextEvent = NULL;

						InsertEvent(ptrEventList, ptrNewGridPreemption);
					}
					else printf("ERROR (job arrival): merdou o malloc!!!\n");

				}

				ptrAuxMachine = ptrAuxMachine->nextMachine;
			} // end while(ptrAuxMachine)

			// insert a new planning into the event list
			event *ptrNewEvent;
			if( (ptrNewEvent = malloc(sizeof(event))) ) {

				ptrNewEvent->eventNumber = 0;
				ptrNewEvent->eventID = ALLOCATIONPLANNING;
				ptrNewEvent->time = (ptrCurrentEvent->time + 1);
				ptrNewEvent->flag = 1;
//				ptrNewEvent->jobInfo.jobID = ptrCurrentEvent->jobInfo.jobID;
//				ptrNewEvent->jobInfo.jobSize = ptrCurrentEvent->jobInfo.jobSize;
//				ptrNewEvent->jobInfo.arrivalTime = ptrCurrentEvent->jobInfo.arrivalTime;
//				ptrNewEvent->jobInfo.finnishTime = ptrCurrentEvent->jobInfo.finnishTime;
//				ptrNewEvent->jobInfo.longestTask = ptrCurrentEvent->jobInfo.longestTask;
//				ptrNewEvent->jobInfo.deadline = ptrCurrentEvent->jobInfo.deadline;
//				ptrNewEvent->jobInfo.nextJob = NULL;
				ptrNewEvent->nextEvent = NULL;

				InsertEvent(ptrEventList, ptrNewEvent);

			} else printf("ERROR (job arrival): merdou o malloc!!!\n");

		} else printf("ERROR (job arrival): there is no job list!!!\n");

	} else printf("ERROR (job arrival): wrong eventID!!!\n");

}
