/*
 * jobfinnishedopt.c
 *
 *  Created on: Aug 16, 2012
 *      Author: PauloDitarso
 */

#include "simulation.h"

void JobFinnishedOpt(event *ptrCurrentEvent, jobAccountInfo *ptrJobAccountInfo, job *ptrJobList) {

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
		if (ptrAuxJobAccount == NULL) printf("ERROR (job finnishedOpt): job account not found!!!\n");

		while(ptrAuxJobList) {
			if ( ptrAuxJobList->jobID == ptrCurrentEvent->jobInfo.jobID ) {
				ptrAuxJobList->finnishTime = ptrCurrentEvent->time;
				break;
			}
			ptrAuxJobList = ptrAuxJobList->nextJob;
		}
		if (ptrAuxJobList == NULL) printf("ERROR (job finnishedOpt): job not found!!!\n");

		int utility = 0;
		switch(utilityFunction) {
			case CONSTANT:
				if ( (ptrAuxJobList->finnishTime - ptrAuxJobList->arrivalTime) <= ptrAuxJobList->deadline ) {
					utility = ptrAuxJobList->maxUtility;
				}
				else {
					utility = 0;
				}
				break;
			case LINEAR:
				if ( (ptrAuxJobList->finnishTime - ptrAuxJobList->arrivalTime) < ptrAuxJobList->deadline ) {
					utility = ( (-1)*(ptrAuxJobList->maxUtility/(ptrCurrentEvent->jobInfo.deadline-ptrCurrentEvent->jobInfo.arrivalTime))*(ptrAuxJobList->finnishTime - ptrAuxJobList->arrivalTime) + ptrAuxJobList->maxUtility );
				}
				else {
					utility = 0;
				}
				break;
			case STEP:
				if ( (ptrAuxJobList->finnishTime - ptrAuxJobList->arrivalTime) <= (ptrAuxJobList->deadline/3) ) {
					utility = ptrAuxJobList->maxUtility;
				}
				else {
					if ( (ptrAuxJobList->finnishTime - ptrAuxJobList->arrivalTime) <= (2*ptrAuxJobList->deadline/3) ) {
						utility = (2*ptrAuxJobList->maxUtility/3);
					}
					else {
						if ( (ptrAuxJobList->finnishTime - ptrAuxJobList->arrivalTime) <= ptrAuxJobList->deadline ) {
							utility = (ptrAuxJobList->maxUtility/3);
						}
						else {
							if ( (ptrAuxJobList->finnishTime - ptrAuxJobList->arrivalTime) > ptrAuxJobList->deadline ) {
								utility = 0;
							}
							else {
								printf("ERROR (job finnishedOpt): switch STEP!!!\n");
							}
						}
					}
				}
				break;
			default:
				break;
		} // end of switch(utilityFunction)

		if (ptrAuxJobList->utility != utility) printf("ERROR (job finnishedOpt): different utility values!!!\n");

		printf("eventID %d (Job Finnished) time %ld ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
		printf("JobID %d AR %ld FT %ld LT %d Deadline %ld MU %ld Utility %ld Cost %.2f Profit %.2f\n", ptrAuxJobList->jobID, ptrAuxJobList->arrivalTime, ptrAuxJobList->finnishTime,
				ptrAuxJobList->longestTask, ptrAuxJobList->deadline, ptrAuxJobList->maxUtility, ptrAuxJobList->utility, ptrAuxJobList->cost,
				(ptrAuxJobList->utility - ptrAuxJobList->cost));

	} else printf("ERROR (job finnished): wrong eventID!!!\n");
}
