/*
 * incrementbalance.c
 *
 *  Created on: Apr 12, 2012
 *      Author: PauloDitarso
 */

#include "simulation.h"

void IncrementBalance(balanceAccountInfo *ptrBalanceAccountInfo, unsigned long int time, unsigned long int value) {

	if (ptrBalanceAccountInfo) {

		balanceAccountInfo *ptrLast;
//		ptrLast = ptrBalanceAccountInfo;
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
			ptrNewBalance->consumed = 0;
			ptrNewBalance->value = (value + ptrLast->value);
			ptrNewBalance->nextBalanceAccountInfo = NULL;

			ptrLast->nextBalanceAccountInfo = ptrNewBalance;

		} else printf("ERROR (increment balance): merdou o malloc!!!\n");

	} else printf("ERROR (increment balance): there is no balance list!!!\n");
}
