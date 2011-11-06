/*
 * griddonating.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void GridDonating(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList) {

	if (ptrCurrentEvent->eventID == GRIDDONATING) {

		machine *ptrAuxMachine;
		ptrAuxMachine = ptrMachineList;

		while (ptrAuxMachine) {

			if ( ptrAuxMachine->source == LOCAL && ptrAuxMachine->status == IDLE ) {

				printf("eventID %d (Grid Donating) time %d \n", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
				//	printf("machineID %d source %d status %d AT %d DT %d UP %f RP %f\n",
				//			ptrCurrentEvent->machineInfo.machineID, ptrCurrentEvent->machineInfo.source,
				//			ptrCurrentEvent->machineInfo.status, ptrCurrentEvent->machineInfo.arrivalTime,
				//			ptrCurrentEvent->machineInfo.departureTime, ptrCurrentEvent->machineInfo.usagePrice,
				//			ptrCurrentEvent->machineInfo.reservationPrice);

				break;
			}

			ptrAuxMachine = ptrAuxMachine->nextMachine;
		}

	} else printf("ERROR (grid donating): wrong eventID!!!\n");
}
