/*
 * jobarrival.c
 *
 *  Created on: Jan 23, 2012
 *      Author: PauloDitarso
 */

#include "simulation.h"

void JobArrival(event *ptrCurrentEvent, event *ptrEventList, job *ptrJobList) {

	printf("eventID %d (Job Arrival) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
	printf("jobID %d AT %d FT %d LT %d Deadline %d \n", ptrCurrentEvent->jobInfo.jobID, ptrCurrentEvent->jobInfo.arrivalTime, ptrCurrentEvent->jobInfo.finnishTime,
			ptrCurrentEvent->jobInfo.longestTask, ptrCurrentEvent->jobInfo.deadline);

}
