/*
 * gridpreempted.c
 *
 *  Created on: Nov 3, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void GridPreempted(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, gridAccountInfo *ptrGridInfoList,
		balanceAccountInfo *ptrBalanceAccountInfo) {

	if (ptrCurrentEvent->eventID == GRIDPREEMPTED) {

		gridAccountInfo *ptrAuxGrid, *ptrAuxLast;
		ptrAuxGrid = ptrAuxLast = ptrGridInfoList;
		unsigned short int found = 0;

		while (ptrAuxGrid) {

			if ( ptrAuxGrid->machineID == ptrCurrentEvent->machineInfo.machineID &&
					ptrAuxGrid->source == ptrCurrentEvent->machineInfo.source &&
					ptrAuxGrid->finnishTime == 0) {

				found = 1;
				ptrAuxGrid->finnishTime = ptrCurrentEvent->time;

				IncrementBalance(ptrBalanceAccountInfo, ptrCurrentEvent->time, (ptrAuxGrid->finnishTime - ptrAuxGrid->startTime));

				printf("eventID %d (Grid Preempted) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
				printf("machineID %d source %d\n", ptrAuxGrid->machineID, ptrAuxGrid->source);

				break;

			}

			if (ptrAuxGrid->gridAccountID > 1) {
				ptrAuxLast = ptrAuxGrid;
			}
			ptrAuxGrid = ptrAuxGrid->nextGridAccountInfo;

		}

		if (!found) {
			printf("ERROR (grid preempted): didn't find the grid entry!!!\n");
		}


//		printf("eventID %d (Grid Preempted) time %d \n", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
		//	printf("machineID %d source %d status %d AT %d DT %d UP %f RP %f\n",
		//			ptrCurrentEvent->machineInfo.machineID, ptrCurrentEvent->machineInfo.source,
		//			ptrCurrentEvent->machineInfo.status, ptrCurrentEvent->machineInfo.arrivalTime,
		//			ptrCurrentEvent->machineInfo.departureTime, ptrCurrentEvent->machineInfo.usagePrice,
		//			ptrCurrentEvent->machineInfo.reservationPrice);

	} else printf("ERROR (grid preempted): wrong eventID!!!\n");
}
