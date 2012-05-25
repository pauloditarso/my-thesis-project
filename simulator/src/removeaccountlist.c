/*
 * removeaccountlist.c
 *
 *  Created on: Nov 3, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void RemoveTaskAccountList(taskAccountInfo **ptrPtrTaskAccountInfoList, taskAccountInfo *ptrOldTaskAccount) {

	taskAccountInfo *ptrAux, *ptrActual, *ptrLast;
	ptrAux = ptrActual = ptrLast = (*ptrPtrTaskAccountInfoList);
	unsigned short int found = 0;

	while (ptrAux) {

		if(ptrAux->machineID == ptrOldTaskAccount->machineID &&
				ptrAux->source == ptrOldTaskAccount->source &&
				ptrAux->finnishTime == 0) {

			found = 1;
			break;
		}
		else {
			ptrLast = ptrActual;
			ptrActual = ptrActual->nextTaskAccountInfo;
		}
		ptrAux = ptrAux->nextTaskAccountInfo;
	}

	if (found) {

		if (ptrLast != ptrActual) {
			ptrLast->nextTaskAccountInfo = ptrActual->nextTaskAccountInfo;
			ptrActual->nextTaskAccountInfo = NULL;
			free(ptrActual);
			ptrActual = NULL;
		}
		else {
			taskAccountInfo *ptrPtrAux;
			ptrPtrAux = *ptrPtrTaskAccountInfoList;
			*ptrPtrTaskAccountInfoList = (*ptrPtrTaskAccountInfoList)->nextTaskAccountInfo;
			free(ptrPtrAux);
			ptrPtrAux = NULL;
		}

	} else printf("ERROR (remove account info): account info not found!!!\n");
}
