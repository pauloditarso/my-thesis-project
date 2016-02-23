/*
 * insertaccountlist.c
 *
 *  Created on: Sep 5, 2011
 *      Author: PauloDitarso
 */

# include "simulation.h"

void InsertJobAccountList(event *ptrCurrentEvent, jobAccountInfo *ptrJobAccountInfoList) {

	if (ptrJobAccountInfoList) {

		if (ptrJobAccountInfoList->jobAccountID == 0) { // account list empty
			ptrJobAccountInfoList->jobAccountID = 1;
			ptrJobAccountInfoList->jobID = ptrCurrentEvent->jobInfo.jobID;
			ptrJobAccountInfoList->startTime = ptrCurrentEvent->time;
			ptrJobAccountInfoList->finnishTime = 0; // it must be updated later
			ptrJobAccountInfoList->nextJobAccountInfo = NULL;

//			printf("accountID %d machineID %d source %d taskID %d jobID %d ST %d FT %d\n", ptrJobAccountInfoList->accountID,
//					ptrJobAccountInfoList->machineID, ptrJobAccountInfoList->source, ptrJobAccountInfoList->taskID, ptrJobAccountInfoList->jobID,
//					ptrJobAccountInfoList->startTime, ptrJobAccountInfoList->finnishTime);

		} else {

			jobAccountInfo *ptrAuxJobAccount;
			ptrAuxJobAccount = ptrJobAccountInfoList;
			unsigned int count = 1;

			while(ptrAuxJobAccount->nextJobAccountInfo != NULL) {
				count += 1;
				ptrAuxJobAccount = ptrAuxJobAccount->nextJobAccountInfo;
			}

			jobAccountInfo *ptrNewJobAccount;

			if ( (ptrNewJobAccount = malloc(sizeof(jobAccountInfo))) ) {
				ptrNewJobAccount->jobAccountID = (count + 1);
				ptrNewJobAccount->jobID = ptrCurrentEvent->jobInfo.jobID;
				ptrNewJobAccount->startTime = ptrCurrentEvent->time;
				ptrNewJobAccount->finnishTime = 0; // it must be updated later
				ptrNewJobAccount->nextJobAccountInfo = NULL;

//				printf("accountID %d machineID %d source %d taskID %d jobID %d ST %d FT %d\n", ptrNewTaskAccount->accountID,
//									ptrNewTaskAccount->machineID, ptrNewTaskAccount->source, ptrNewTaskAccount->taskID, ptrNewTaskAccount->jobID,
//									ptrNewTaskAccount->startTime, ptrNewTaskAccount->finnishTime);

			} else printf("ERROR (insert jobaccount): merdou o malloc!!!\n");

			ptrAuxJobAccount->nextJobAccountInfo = ptrNewJobAccount;

		}

	} else printf("ERROR (insert jobaccount): there is no account list!!!\n");
}
