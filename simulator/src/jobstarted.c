/*
 * jobstarted.c
 *
 *  Created on: Jan 23, 2012
 *      Author: PauloDitarso
 */

#include "simulation.h"

void JobStarted(event *ptrCurrentEvent, event *ptrEventList, job *ptrJobList) {

	printf("eventID %d (Job Started) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
	printf("JobID %d\n", ptrCurrentEvent->jobInfo.jobID);

}
