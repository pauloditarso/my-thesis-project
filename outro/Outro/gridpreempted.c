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

			if ( ptrAuxGrid->machineID == ptrCurrentEvent->machineInfo.machineID && ptrAuxGrid->source == ptrCurrentEvent->machineInfo.source &&
					ptrAuxGrid->finnishTime == 0) {

				found = 1;
				ptrAuxGrid->finnishTime = ptrCurrentEvent->time;

				IncrementBalance(ptrBalanceAccountInfo, ptrCurrentEvent->time, (ptrAuxGrid->finnishTime - ptrAuxGrid->startTime));

				// debug mode
//				if (ptrCurrentEvent->time == 25016143 || ptrAuxGrid->gridAccountID == 2944) {
//					printf("###################################################\n");
//					printf("FT %d ST %d Vl %d\n", ptrAuxGrid->finnishTime, ptrAuxGrid->startTime, (ptrAuxGrid->finnishTime - ptrAuxGrid->startTime));
//				}
				printf("eventID %d (Grid Preempted) time %ld ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
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
		else { // remover da gridAccountList o noh que jah foi finalizado

			if ( ptrAuxGrid != NULL && ptrAuxGrid != ptrAuxLast) {

				gridAccountInfo *ptrRemove;
				ptrRemove = ptrAuxGrid;
				ptrAuxLast->nextGridAccountInfo = ptrAuxGrid->nextGridAccountInfo;
				ptrAuxGrid = ptrAuxGrid->nextGridAccountInfo;
				free(ptrRemove);
				ptrRemove = NULL;

			}

		}

//		printf("eventID %d (Grid Preempted) time %d \n", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
		//	printf("machineID %d source %d status %d AT %d DT %d UP %.2f RP %.2f\n",
		//			ptrCurrentEvent->machineInfo.machineID, ptrCurrentEvent->machineInfo.source,
		//			ptrCurrentEvent->machineInfo.status, ptrCurrentEvent->machineInfo.arrivalTime,
		//			ptrCurrentEvent->machineInfo.departureTime, ptrCurrentEvent->machineInfo.usagePrice,
		//			ptrCurrentEvent->machineInfo.reservationPrice);

	} else printf("ERROR (grid preempted): wrong eventID!!!\n");
}
