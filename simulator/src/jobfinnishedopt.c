/*
 * jobfinnishedopt.c
 *
 *  Created on: Aug 16, 2012
 *      Author: PauloDitarso
 */

#include "simulation.h"

void JobFinnishedOpt(event *ptrCurrentEvent, jobAccountInfo *ptrJobAccountInfo, job *ptrJobList, task **ptrPtrTaskList, task **ptrPtrOrderedTaskList) {

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

		float utility = 0.0;
		switch(utilityFunction) {
			case CONSTANT:
				if (ptrAuxJobList->finnishTime <= ptrAuxJobList->deadline) {
					utility = ptrAuxJobList->maxUtility;
				}
				else {
					utility = 0;
				}
				break;
			case LINEAR:
				if (ptrAuxJobList->finnishTime < ptrAuxJobList->deadline) {
//					utility = ( (-1)*(ptrAuxJobList->maxUtility/(float)(ptrCurrentEvent->jobInfo.deadline - ptrCurrentEvent->jobInfo.arrivalTime))*(float)(ptrAuxJobList->finnishTime - ptrAuxJobList->arrivalTime) + ptrAuxJobList->maxUtility );
					utility = ( ptrAuxJobList->maxUtility * ( 1 -
							( (float)(ptrAuxJobList->finnishTime - ptrAuxJobList->arrivalTime)/(float)(ptrCurrentEvent->jobInfo.deadline - ptrCurrentEvent->jobInfo.arrivalTime) ) ) );
				}
				else {
					utility = 0;
				}
				break;
			case STEP:
				if ( ptrAuxJobList->finnishTime <= (ptrAuxJobList->deadline/3) ) {
					utility = ptrAuxJobList->maxUtility;
				}
				else {
					if ( ptrAuxJobList->finnishTime <= (2*ptrAuxJobList->deadline/3) ) {
						utility = (2*ptrAuxJobList->maxUtility/3);
					}
					else {
						if ( ptrAuxJobList->finnishTime <= ptrAuxJobList->deadline ) {
							utility = (ptrAuxJobList->maxUtility/3);
						}
						else {
							if ( ptrAuxJobList->finnishTime > ptrAuxJobList->deadline ) {
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

//		debug mode
//		printf("ptr->utility %.2f utility %.2f\n", ptrAuxJobList->utility, utility);
		int diffUtility = (int) truncf(fabs(ptrAuxJobList->utility - utility));
		if (diffUtility != 0) printf("ERROR (job finnishedOpt): different utility values!!!\n");
//		if ((int)ptrAuxJobList->utility != (int)utility) printf("ERROR (job finnishedOpt): different utility values!!!\n");
//		float roundedAux1, roundedAux2;
//		roundedAux1 = ceilf(ptrAuxJobList->utility * 100) / 100;
//		roundedAux2 = ceilf(utility * 100) / 100;
//		if (roundedAux1 != roundedAux2) printf("ERROR (job finnishedOpt): different utility values!!!\n");

		// desalocando memoria da lista de tasks
		task *ptrAuxTaskList;
		ptrAuxTaskList = ptrPtrTaskList;
		while(ptrAuxTaskList->status == 3) { // 3 -> FINNISHED

			task *ptrRemove;
			ptrRemove = ptrAuxTaskList;
			ptrAuxTaskList = ptrAuxTaskList->nextTask;
			(*ptrPtrTaskList) = (*ptrPtrTaskList)->nextTask;
			free(ptrRemove);
			ptrRemove = NULL;

		}

		// desalocando memoria da lista ordenada de tasks
		task *ptrAuxOrderedTaskList;
		ptrAuxOrderedTaskList = ptrPtrOrderedTaskList;
		while(ptrAuxOrderedTaskList->status == 3) { // 3 -> FINNISHED

			task *ptrRemove;
			ptrRemove = ptrAuxOrderedTaskList;
			ptrAuxOrderedTaskList = ptrAuxOrderedTaskList->nextTask;
			(*ptrPtrOrderedTaskList) = (*ptrPtrOrderedTaskList)->nextTask;
			free(ptrRemove);
			ptrRemove = NULL;

		}

		// calculating the aggregated utility, cost and profit
		aggregatedUtility += ptrAuxJobList->utility; aggregatedCost += ptrAuxJobList->cost; aggregatedProfit += (ptrAuxJobList->utility - ptrAuxJobList->cost);

		printf("eventID %d (Job Finnished) time %ld ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
		printf("JobID %d AR %ld FT %ld LT %d Deadline %ld MU %.2f Utility %.2f Cost %.2f Profit %.2f\n", ptrAuxJobList->jobID, ptrAuxJobList->arrivalTime, ptrAuxJobList->finnishTime,
				ptrAuxJobList->longestTask, ptrAuxJobList->deadline, ptrAuxJobList->maxUtility, ptrAuxJobList->utility, ptrAuxJobList->cost,
				(ptrAuxJobList->utility - ptrAuxJobList->cost));

	} else printf("ERROR (job finnished): wrong eventID!!!\n");
}
