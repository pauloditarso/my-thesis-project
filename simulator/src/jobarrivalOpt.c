/*
 * jobarrivalOpt.c
 *
 *  Created on: Jun 12, 2012
 *      Author: PauloDitarso
 *
 *  What is done:
 *  	1. it tests if it is the correct event;
 *  	2. it tests if there are job, machine and task lists;
 *  	3. it creates a new node in the job list;
 *  	4. it creates the grid machines; (???)
 *  	. it plans the schedule map:
 *  		.1. it creates an ordered task list (from the longest to the shortest);
 *  		.2. it creates an ordered (by hours) machine list (in-house, cloud and grid machines);
 *  		.3.
 *
 *  	2. it looks for a task tagged as queued;
 *  	3. it looks for an idle or a donating machine;
 *  	4. if the machine is local and it is donating, it creates a grid preemption event;
 *      5. it changes the machine status to RUNNING and the task status to STARTED;
 *      6. it creates a task finish event;
 *      7. it creates a job start event if the task ID is 1 (first task);
 */

#include "simulation.h"

void JobArrivalOpt(event *ptrCurrentEvent, event *ptrEventList, job *ptrJobList, task *ptrTaskList, balanceAccountInfo *ptrBalanceAccountInfo) {

	if (ptrCurrentEvent->eventID == JOBARRIVAL) {

		if (ptrJobList) {

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

	} else printf("ERROR (job arrival): wrong eventID!!!\n");

}
