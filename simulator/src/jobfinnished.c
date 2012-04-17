/*
 * jobfinnished.c
 *
 *  Created on: Jan 23, 2012
 *      Author: PauloDitarso
 */

#include "simulation.h"

void JobFinnished(event *ptrCurrentEvent, jobAccountInfo *ptrJobAccountInfo, job *ptrJobList) {

	if ( ptrCurrentEvent->eventID == JOBFINNISHED ) {

		jobAccountInfo *ptrAuxJobAccount;
		ptrAuxJobAccount = ptrJobAccountInfo;
		job *ptrAuxJobList;
		ptrAuxJobList = ptrJobList;

		while(ptrAuxJobAccount) {
			if ( ptrAuxJobAccount->jobID == ptrCurrentEvent->jobInfo.jobID ) {
				ptrAuxJobAccount->finnishTime = ptrCurrentEvent->time;
				break;
			}
			ptrAuxJobAccount = ptrAuxJobAccount->nextJobAccountInfo;
		}

		while(ptrAuxJobList) {
			if ( ptrAuxJobList->jobID == ptrCurrentEvent->jobInfo.jobID ) {
				ptrAuxJobList->finnishTime = ptrCurrentEvent->time;
				break;
			}
			ptrAuxJobList = ptrAuxJobList->nextJob;
		}

		printf("eventID %d (Job Finnished) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
		printf("JobID %d AR %d FT %d LT %d DL %d\n", ptrAuxJobList->jobID, ptrAuxJobList->arrivalTime, ptrAuxJobList->finnishTime,
				ptrAuxJobList->longestTask, ptrAuxJobList->deadline);

	} else printf("ERROR (job finnished): wrong eventID!!!\n");
}
