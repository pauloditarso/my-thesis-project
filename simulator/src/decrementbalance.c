/*
 * decrementbalance.c
 *
 *  Created on: Apr 15, 2012
 *      Author: PauloDitarso
 */

#include "simulation.h"

void DecrementBalance(balanceAccountInfo *ptrBalanceAccountInfo, unsigned long int time, unsigned long int value) {

	if (ptrBalanceAccountInfo) {

		balanceAccountInfo *ptrLast;
//		ptrAux = ptrLast = ptrBalanceAccountInfo;
//		unsigned long int count = 0;
		ptrLast = ptrLastBalance;

		// going to the end of the balance account info list
		while(ptrLast->nextBalanceAccountInfo != NULL) {
//			count++;
			ptrLast = ptrLast->nextBalanceAccountInfo;
		}

		balanceAccountInfo *ptrNewBalance;
		if ( (ptrNewBalance = malloc(sizeof(balanceAccountInfo))) ) {
			ptrNewBalance->balanceAccountID = (ptrLast->balanceAccountID + 1);
			ptrNewBalance->time = time;
			ptrNewBalance->consumed = value;
			ptrNewBalance->value = (ptrLast->value >= value) ? (ptrLast->value - value) : 0; // to treat the case of a runtime greater than the balance value
			ptrNewBalance->nextBalanceAccountInfo = NULL;

			ptrLast->nextBalanceAccountInfo = ptrNewBalance;

		} else printf("ERROR (decrement balance): merdou o malloc!!!\n");

	} else printf("ERROR (decrement balance): there is no balance list!!!\n");

}
