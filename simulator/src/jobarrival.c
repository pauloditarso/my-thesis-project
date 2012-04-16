/*
 * jobarrival.c
 *
 *  Created on: Jan 23, 2012
 *      Author: PauloDitarso
 */

#include "simulation.h"

void JobArrival(event *ptrCurrentEvent, event *ptrEventList, job *ptrJobList, balanceAccountInfo *ptrBalanceAccountInfo) {

	if (ptrCurrentEvent->eventID == JOBARRIVAL) {

		if (ptrJobList) {

			if (ptrJobList->jobID == 0) {	// code for an empty job list
				ptrJobList->jobID = ptrCurrentEvent->jobInfo.jobID;
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
			printf("jobID %d AT %d FT %d LT %d Deadline %d \n", ptrCurrentEvent->jobInfo.jobID, ptrCurrentEvent->jobInfo.arrivalTime, ptrCurrentEvent->jobInfo.finnishTime,
					ptrCurrentEvent->jobInfo.longestTask, ptrCurrentEvent->jobInfo.deadline);

		} else printf("ERROR (job arrival): there is no job list!!!\n");

	} else printf("ERROR (job arrival): wrong eventID!!!\n");

}
