/*
 * gridpreempted.c
 *
 *  Created on: Nov 3, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void GridPreempted(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList) {

	if (ptrCurrentEvent->eventID == GRIDPREEMPTED) {

		printf("eventID %d (Grid Preempted) time %d \n", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
		//	printf("machineID %d source %d status %d AT %d DT %d UP %f RP %f\n",
		//			ptrCurrentEvent->machineInfo.machineID, ptrCurrentEvent->machineInfo.source,
		//			ptrCurrentEvent->machineInfo.status, ptrCurrentEvent->machineInfo.arrivalTime,
		//			ptrCurrentEvent->machineInfo.departureTime, ptrCurrentEvent->machineInfo.usagePrice,
		//			ptrCurrentEvent->machineInfo.reservationPrice);

	} else printf("ERROR (grid preempted): wrong eventID!!!\n");
}
