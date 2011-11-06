/*
 * removeaccountlist.c
 *
 *  Created on: Nov 3, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void RemoveAccountList(accountInfo **ptrPtrAccountInfoList, accountInfo *ptrOldAccount) {

	accountInfo *ptrAux, *ptrActual, *ptrLast;
	ptrAux = ptrActual = ptrLast = (*ptrPtrAccountInfoList);
	unsigned short int found = 0;

	while (ptrAux) {

		if(ptrAux->machineID == ptrOldAccount->machineID &&
				ptrAux->source == ptrOldAccount->source &&
				ptrAux->finnishTime == 0) {

			found = 1;
			break;
		}
		else {
			ptrLast = ptrActual;
			ptrActual = ptrActual->nextAccountInfo;
		}
		ptrAux = ptrAux->nextAccountInfo;
	}

	if (found) {

		if (ptrLast != ptrActual) {
			ptrLast->nextAccountInfo = ptrActual->nextAccountInfo;
			ptrActual->nextAccountInfo = NULL;
		}
		else {
			*ptrPtrAccountInfoList = (*ptrPtrAccountInfoList)->nextAccountInfo;
		}

	} else printf("ERROR (remove account info): account info not found!!!\n");
}
