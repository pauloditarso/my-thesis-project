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

		printf("eventID %d (Job Started) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
		printf("JobID %d\n", ptrCurrentEvent->jobInfo.jobID);

	} else printf("ERROR (job started): wrong eventID!!!\n");
}
