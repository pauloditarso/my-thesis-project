/*
 * insertgridlist.c
 *
 *  Created on: Nov 17, 2011
 *      Author: PauloDitarso
 */

# include "simulation.h"

void InsertGridList(event *ptrCurrentEvent, machine *ptrAuxMachine, gridInfo *ptrGridInfoList) {

	if (ptrGridInfoList) {

		if (ptrGridInfoList->accountID == 0) { // account list empty
			ptrGridInfoList->accountID = 1;
			ptrGridInfoList->machineID = ptrAuxMachine->machineID;
			ptrGridInfoList->source = ptrAuxMachine->source;
			ptrGridInfoList->startTime = ptrCurrentEvent->time;
			ptrGridInfoList->finnishTime = 0; // it must be updated later
			ptrGridInfoList->nextGridInfo = NULL;

		} else {

			gridInfo *ptrAuxGrid;
			ptrAuxGrid = ptrGridInfoList;
			unsigned int count = 1;

			while(ptrAuxGrid->nextGridInfo != NULL) {
				count += 1;
				ptrAuxGrid = ptrAuxGrid->nextGridInfo;
			}

			gridInfo *ptrNewGrid;

			if ( (ptrNewGrid = malloc(sizeof(gridInfo))) ) {
				ptrNewGrid->accountID = (count + 1);
				ptrNewGrid->machineID = ptrAuxMachine->machineID;
				ptrNewGrid->source = ptrAuxMachine->source;
				ptrNewGrid->startTime = ptrCurrentEvent->time;
				ptrNewGrid->finnishTime = 0; // it must be updated later
				ptrNewGrid->nextGridInfo = NULL;

				ptrAuxGrid->nextGridInfo = ptrNewGrid;

			} else printf("ERROR (insert grid): merdou o malloc!!!\n");
		}

	} else printf("ERROR (insert grid): there is no grid list!!!\n");
}
