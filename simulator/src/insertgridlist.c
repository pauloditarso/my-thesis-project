/*
 * insertgridlist.c
 *
 *  Created on: Nov 17, 2011
 *      Author: PauloDitarso
 */

# include "simulation.h"

void InsertGridList(event *ptrCurrentEvent, machine *ptrAuxMachine, gridAccountInfo *ptrGridInfoList) {

	if (ptrGridInfoList) {

		if (ptrGridInfoList->gridAccountID == 0) { // account list empty
			ptrGridInfoList->gridAccountID = 1;
			ptrGridInfoList->machineID = ptrAuxMachine->machineID;
			ptrGridInfoList->source = ptrAuxMachine->source;
			ptrGridInfoList->startTime = ptrCurrentEvent->time;
			ptrGridInfoList->finnishTime = 0; // it must be updated later
			ptrGridInfoList->nextGridAccountInfo = NULL;

		} else {

			gridAccountInfo *ptrAuxGrid;
			ptrAuxGrid = ptrGridInfoList;
			unsigned int count = 1;

			while(ptrAuxGrid->nextGridAccountInfo != NULL) {
				count += 1;
				ptrAuxGrid = ptrAuxGrid->nextGridAccountInfo;
			}

			gridAccountInfo *ptrNewGrid;

			if ( (ptrNewGrid = malloc(sizeof(gridAccountInfo))) ) {
				ptrNewGrid->gridAccountID = (count + 1);
				ptrNewGrid->machineID = ptrAuxMachine->machineID;
				ptrNewGrid->source = ptrAuxMachine->source;
				ptrNewGrid->startTime = ptrCurrentEvent->time;
				ptrNewGrid->finnishTime = 0; // it must be updated later
				ptrNewGrid->nextGridAccountInfo = NULL;

				ptrAuxGrid->nextGridAccountInfo = ptrNewGrid;

			} else printf("ERROR (insert grid): merdou o malloc!!!\n");
		}

	} else printf("ERROR (insert grid): there is no grid list!!!\n");
}
