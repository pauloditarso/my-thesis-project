/*
 * insertaccountlist.c
 *
 *  Created on: Sep 5, 2011
 *      Author: PauloDitarso
 */

# include "simulation.h"

void InsertJobAccountList(event *ptrCurrentEvent, job *ptrAuxJob, jobAccountInfo *ptrJobAccountInfoList) {

	if (ptrJobAccountInfoList) {

		if (ptrJobAccountInfoList->jobAccountID == 0) { // account list empty
			ptrJobAccountInfoList->jobAccountID = 1;
//			ptrJobAccountInfoList->machineID = ptrAuxMachine->machineID;
//			ptrJobAccountInfoList->source = ptrAuxMachine->source;
//			ptrJobAccountInfoList->taskID = ptrAuxJob->taskID;
//			ptrJobAccountInfoList->jobID = ptrAuxJob->jobID;
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

			jobAccountInfo *ptrNewTaskAccount;

			if ( (ptrNewTaskAccount = malloc(sizeof(jobAccountInfo))) ) {
				ptrNewTaskAccount->jobAccountID = (count + 1);
//				ptrNewTaskAccount->machineID = ptrAuxMachine->machineID;
//				ptrNewTaskAccount->source = ptrAuxMachine->source;
//				ptrNewTaskAccount->taskID = ptrAuxJob->taskID;
//				ptrNewTaskAccount->jobID = ptrAuxJob->jobID;
				ptrNewTaskAccount->startTime = ptrCurrentEvent->time;
				ptrNewTaskAccount->finnishTime = 0; // it must be updated later
				ptrNewTaskAccount->nextJobAccountInfo = NULL;

				ptrAuxJobAccount->nextJobAccountInfo = ptrNewTaskAccount;

//				printf("accountID %d machineID %d source %d taskID %d jobID %d ST %d FT %d\n", ptrNewTaskAccount->accountID,
//									ptrNewTaskAccount->machineID, ptrNewTaskAccount->source, ptrNewTaskAccount->taskID, ptrNewTaskAccount->jobID,
//									ptrNewTaskAccount->startTime, ptrNewTaskAccount->finnishTime);

			} else printf("ERROR (insert jobaccount): merdou o malloc!!!\n");
		}

	} else printf("ERROR (insert jobaccount): there is no account list!!!\n");
}
