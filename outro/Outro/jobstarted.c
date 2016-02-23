/*
 * jobstarted.c
 *
 *  Created on: Jan 23, 2012
 *      Author: PauloDitarso
 */

#include "simulation.h"

void JobStarted(event *ptrCurrentEvent, jobAccountInfo *ptrJobAccountInfo, job *ptrJobList) {

	if ( ptrCurrentEvent->eventID == JOBSTARTED ) {

		InsertJobAccountList(ptrCurrentEvent, ptrJobAccountInfo);

		printf("eventID %d (Job Started) time %ld ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
		printf("JobID %d\n", ptrCurrentEvent->jobInfo.jobID);
//		printf("eventID %d (Job Started) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
//		printf("JobID %d AT %d FT %d LT %d DL %d\n", ptrCurrentEvent->jobInfo.jobID, ptrCurrentEvent->jobInfo.arrivalTime, ptrCurrentEvent->jobInfo.finnishTime,
//				ptrCurrentEvent->jobInfo.longestTask, ptrCurrentEvent->jobInfo.deadline);

	} else printf("ERROR (job started): wrong eventID!!!\n");
}
