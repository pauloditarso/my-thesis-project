/*
 * insertaccountlist.c
 *
 *  Created on: Sep 5, 2011
 *      Author: PauloDitarso
 */

# include "simulation.h"

void InsertAccountList(event *ptrCurrentEvent, machine *ptrAuxMachine,  task *ptrAuxTask, accountInfo *ptrAccountInfoList) {

	if (ptrAccountInfoList) {

		if (ptrAccountInfoList->accountID == 0) { // account list empty
			ptrAccountInfoList->accountID = 1;
			ptrAccountInfoList->machineID = ptrAuxMachine->machineID;
			ptrAccountInfoList->source = ptrAuxMachine->source;
			ptrAccountInfoList->taskID = ptrAuxTask->taskID;
			ptrAccountInfoList->jobID = ptrAuxTask->jobID;
			ptrAccountInfoList->startTime = ptrCurrentEvent->time;
			ptrAccountInfoList->finnishTime = 0; // it must be updated later
			ptrAccountInfoList->nextAccountInfo = NULL;

//			printf("accountID %d machineID %d source %d taskID %d jobID %d ST %d FT %d\n", ptrAccountInfoList->accountID,
//					ptrAccountInfoList->machineID, ptrAccountInfoList->source, ptrAccountInfoList->taskID, ptrAccountInfoList->jobID,
//					ptrAccountInfoList->startTime, ptrAccountInfoList->finnishTime);

		} else {

			accountInfo *ptrAuxAccount;
			ptrAuxAccount = ptrAccountInfoList;
			unsigned int count = 1;

			while(ptrAuxAccount->nextAccountInfo != NULL) {
				count += 1;
				ptrAuxAccount = ptrAuxAccount->nextAccountInfo;
			}

			accountInfo *ptrNewAccount;

			if ( (ptrNewAccount = malloc(sizeof(accountInfo))) ) {
				ptrNewAccount->accountID = (count + 1);
				ptrNewAccount->machineID = ptrAuxMachine->machineID;
				ptrNewAccount->source = ptrAuxMachine->source;
				ptrNewAccount->taskID = ptrAuxTask->taskID;
				ptrNewAccount->jobID = ptrAuxTask->jobID;
				ptrNewAccount->startTime = ptrCurrentEvent->time;
				ptrNewAccount->finnishTime = 0; // it must be updated later
				ptrNewAccount->nextAccountInfo = NULL;

				ptrAuxAccount->nextAccountInfo = ptrNewAccount;

//				printf("accountID %d machineID %d source %d taskID %d jobID %d ST %d FT %d\n", ptrNewAccount->accountID,
//									ptrNewAccount->machineID, ptrNewAccount->source, ptrNewAccount->taskID, ptrNewAccount->jobID,
//									ptrNewAccount->startTime, ptrNewAccount->finnishTime);

			} else printf("ERROR (insert account): merdou o malloc!!!\n");
		}

	} else printf("ERROR (insert account): there is no account list!!!\n");
}
