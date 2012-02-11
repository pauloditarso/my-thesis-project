/*
 * insertaccountlist.c
 *
 *  Created on: Sep 5, 2011
 *      Author: PauloDitarso
 */

# include "simulation.h"

void InsertAccountList(event *ptrCurrentEvent, machine *ptrAuxMachine,  task *ptrAuxTask, taskAccountInfo *ptrTaskAccountInfoList) {

	if (ptrTaskAccountInfoList) {

		if (ptrTaskAccountInfoList->taskAccountID == 0) { // account list empty
			ptrTaskAccountInfoList->taskAccountID = 1;
			ptrTaskAccountInfoList->machineID = ptrAuxMachine->machineID;
			ptrTaskAccountInfoList->source = ptrAuxMachine->source;
			ptrTaskAccountInfoList->taskID = ptrAuxTask->taskID;
			ptrTaskAccountInfoList->jobID = ptrAuxTask->jobID;
			ptrTaskAccountInfoList->startTime = ptrCurrentEvent->time;
			ptrTaskAccountInfoList->finnishTime = 0; // it must be updated later
			ptrTaskAccountInfoList->nextTaskAccountInfo = NULL;

//			printf("accountID %d machineID %d source %d taskID %d jobID %d ST %d FT %d\n", ptrTaskAccountInfoList->accountID,
//					ptrTaskAccountInfoList->machineID, ptrTaskAccountInfoList->source, ptrTaskAccountInfoList->taskID, ptrTaskAccountInfoList->jobID,
//					ptrTaskAccountInfoList->startTime, ptrTaskAccountInfoList->finnishTime);

		} else {

			taskAccountInfo *ptrAuxTaskAccount;
			ptrAuxTaskAccount = ptrTaskAccountInfoList;
			unsigned int count = 1;

			while(ptrAuxTaskAccount->nextTaskAccountInfo != NULL) {
				count += 1;
				ptrAuxTaskAccount = ptrAuxTaskAccount->nextTaskAccountInfo;
			}

			taskAccountInfo *ptrNewTaskAccount;

			if ( (ptrNewTaskAccount = malloc(sizeof(taskAccountInfo))) ) {
				ptrNewTaskAccount->taskAccountID = (count + 1);
				ptrNewTaskAccount->machineID = ptrAuxMachine->machineID;
				ptrNewTaskAccount->source = ptrAuxMachine->source;
				ptrNewTaskAccount->taskID = ptrAuxTask->taskID;
				ptrNewTaskAccount->jobID = ptrAuxTask->jobID;
				ptrNewTaskAccount->startTime = ptrCurrentEvent->time;
				ptrNewTaskAccount->finnishTime = 0; // it must be updated later
				ptrNewTaskAccount->nextTaskAccountInfo = NULL;

				ptrAuxTaskAccount->nextTaskAccountInfo = ptrNewTaskAccount;

//				printf("accountID %d machineID %d source %d taskID %d jobID %d ST %d FT %d\n", ptrNewTaskAccount->accountID,
//									ptrNewTaskAccount->machineID, ptrNewTaskAccount->source, ptrNewTaskAccount->taskID, ptrNewTaskAccount->jobID,
//									ptrNewTaskAccount->startTime, ptrNewTaskAccount->finnishTime);

			} else printf("ERROR (insert account): merdou o malloc!!!\n");
		}

	} else printf("ERROR (insert account): there is no account list!!!\n");
}
