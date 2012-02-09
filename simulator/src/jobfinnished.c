/*
 * jobfinnished.c
 *
 *  Created on: Jan 23, 2012
 *      Author: PauloDitarso
 */

#include "simulation.h"

void JobFinnished(event *ptrCurrentEvent, event *ptrEventList, job *ptrJobList) {

	printf("eventID %d (Job Finnished) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
	printf("JobID %d\n", ptrCurrentEvent->jobInfo.jobID);

}
