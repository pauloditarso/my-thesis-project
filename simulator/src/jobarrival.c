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

void JobArrival(event *ptrCurrentEvent, event *ptrEventList, job *ptrJobList, task *ptrTaskList, balanceAccountInfo *ptrBalanceAccountInfo) {

	if (ptrCurrentEvent->eventID == JOBARRIVAL) {

		// creation of a new node in the job list
		if (ptrJobList) {

//			task *ptrAuxTaskList;
//			ptrAuxTaskList = ptrTaskList;
//			unsigned int jobSize = 0;
//
//			while(ptrAuxTaskList) {
//
//				if (ptrAuxTaskList->jobID == ptrCurrentEvent->jobInfo.jobID) {
//					jobSize += 1;
//				}
//
//				ptrAuxTaskList = ptrAuxTaskList->nextTask;
//			}

			if (ptrJobList->jobID == 0) {	// code for an empty job list
				ptrJobList->jobID = ptrCurrentEvent->jobInfo.jobID;
				ptrJobList->jobSize = ptrCurrentEvent->jobInfo.jobSize;
				ptrJobList->arrivalTime = ptrCurrentEvent->jobInfo.arrivalTime;
				ptrJobList->finnishTime = ptrCurrentEvent->jobInfo.finnishTime;
				ptrJobList->longestTask = ptrCurrentEvent->jobInfo.longestTask;
				ptrJobList->deadline = ptrCurrentEvent->jobInfo.deadline;
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
			printf("jobID %d jobSize %d AT %d FT %d LT %d Deadline %d \n", ptrCurrentEvent->jobInfo.jobID, ptrCurrentEvent->jobInfo.jobSize,
					ptrCurrentEvent->jobInfo.arrivalTime, ptrCurrentEvent->jobInfo.finnishTime, ptrCurrentEvent->jobInfo.longestTask,
					ptrCurrentEvent->jobInfo.deadline);

		} else printf("ERROR (job arrival): there is no job list!!!\n");

		// creation of the grid machines



	} else printf("ERROR (job arrival): wrong eventID!!!\n");

}
