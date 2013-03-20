/*
 * getbalance.c
 *
 *  Created on: Apr 12, 2012
 *      Author: PauloDitarso
 */

#include "simulation.h"

unsigned int GetBalance(balanceAccountInfo *ptrBalanceAccountInfo, unsigned long int time) {

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

	// returning the accumulated balance at that point
	return ptrLastBalance->value;
}
