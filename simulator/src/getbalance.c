/*
 * getbalance.c
 *
 *  Created on: Apr 12, 2012
 *      Author: PauloDitarso
 */

#include "simulation.h"

unsigned int GetBalance(balanceAccountInfo **ptrPtrBalanceAccountInfo, unsigned long int time) {

	balanceAccountInfo *ptrAuxBalanceAccount;
//	ptrAuxBalanceAccount = ptrBalanceAccountInfo;
	ptrAuxBalanceAccount = ptrLastBalance;

	// looking for the time point in the balance account info list
	while(ptrAuxBalanceAccount->nextBalanceAccountInfo != NULL) {
		if (ptrAuxBalanceAccount->time > time) {
			break;
		}
		ptrLastBalance = ptrAuxBalanceAccount;
		ptrAuxBalanceAccount = ptrAuxBalanceAccount->nextBalanceAccountInfo;
	}

	if ( (ptrLastBalance->balanceAccountID != 0) && (ptrLastBalance != (*ptrPtrBalanceAccountInfo)) ) {

		while ( (*ptrPtrBalanceAccountInfo)->balanceAccountID < ptrLastBalance->balanceAccountID ) {

			balanceAccountInfo *ptrRemove;
			ptrRemove = (*ptrPtrBalanceAccountInfo);
			(*ptrPtrBalanceAccountInfo) = (*ptrPtrBalanceAccountInfo)->nextBalanceAccountInfo;
			free(ptrRemove);
			ptrRemove = NULL;

		}

	}

	// returning the accumulated balance at that point
	return ptrLastBalance->value;
}
