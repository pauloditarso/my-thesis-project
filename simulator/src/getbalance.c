/*
 * getbalance.c
 *
 *  Created on: Apr 12, 2012
 *      Author: PauloDitarso
 */

#include "simulation.h"

unsigned int GetBalance(balanceAccountInfo *ptrBalanceAccountInfo, unsigned int time) {

	balanceAccountInfo *ptrAuxBalanceAccount;
	ptrAuxBalanceAccount = ptrBalanceAccountInfo;

	while(ptrAuxBalanceAccount->nextBalanceAccountInfo != NULL) {
		if (ptrAuxBalanceAccount->time > time) {
			break;
		}
		ptrAuxBalanceAccount = ptrAuxBalanceAccount->nextBalanceAccountInfo;
	}

	return ptrAuxBalanceAccount->value;
}
