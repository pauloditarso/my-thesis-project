/*
 * machinedeparture.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void MachineDeparture(event *ptrCurrentEvent, event *ptrEventList, machine **ptrPtrMachineList) {

	if (ptrCurrentEvent->eventID == MACHDEPARTURE) {

		if (ptrPtrMachineList) {

			machine *ptrAux, *ptrLastMachine, *ptrActualMachine;
			unsigned short int found = 0;

			ptrAux = ptrLastMachine = ptrActualMachine = (*ptrPtrMachineList);

			while(ptrAux) {

				if ( ptrCurrentEvent->machineInfo.machineID == ptrAux->machineID &&
						ptrCurrentEvent->machineInfo.source == ptrAux->source  &&
						ptrCurrentEvent->machineInfo.departureTime == ptrAux->departureTime ) {

					found = 1;
					break;
				}
				else {
					ptrLastMachine = ptrActualMachine;
					ptrActualMachine = ptrActualMachine->nextMachine;
				}
				ptrAux = ptrAux->nextMachine;
			}

			if (found) {

				if ( ptrActualMachine->status == RUNNING ) {

					// insert a new event into the event list
					event *ptrNewEvent;

					if( (ptrNewEvent = malloc(sizeof(event))) ) {
						ptrNewEvent->eventID = TASKPREEMPTED;
						ptrNewEvent->time = ptrCurrentEvent->time;
						ptrNewEvent->machineInfo.machineID = ptrActualMachine->machineID;
						ptrNewEvent->machineInfo.source = ptrActualMachine->source;
						ptrNewEvent->machineInfo.arrivalTime = ptrActualMachine->arrivalTime;
						ptrNewEvent->machineInfo.departureTime = ptrActualMachine->departureTime;
						ptrNewEvent->machineInfo.status = ptrActualMachine->status;
						ptrNewEvent->machineInfo.reservationPrice = ptrActualMachine->reservationPrice;
						ptrNewEvent->machineInfo.usagePrice = ptrActualMachine->usagePrice;
						ptrNewEvent->machineInfo.nextMachine = NULL;
						ptrNewEvent->nextEvent = NULL;

						InsertEvent(ptrEventList, ptrNewEvent);
					}
					else printf("ERROR (machine departure): merdou o malloc!!!\n");

				}

				if ( ptrActualMachine->source == LOCAL && ptrActualMachine->status == IDLE ) {

					// insert a new grid preemption into the event list
					event *ptrNewGridPreemption;

					if( (ptrNewGridPreemption = malloc(sizeof(event))) ) {
						ptrNewGridPreemption->eventID = GRIDPREEMPTED;
						ptrNewGridPreemption->time = ptrCurrentEvent->time;
						ptrNewGridPreemption->flag = 0;
						ptrNewGridPreemption->nextEvent = NULL;

						InsertEvent(ptrEventList, ptrNewGridPreemption);
					}
					else printf("ERROR (machine arrival): merdou o malloc!!!\n");

				}

				if (ptrLastMachine != ptrActualMachine) {
					ptrLastMachine->nextMachine = ptrActualMachine->nextMachine;
					ptrActualMachine->nextMachine = NULL;
				}
				else {
					*ptrPtrMachineList = (*ptrPtrMachineList)->nextMachine;
				}

				printf("eventID %d (Machine Departure) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
				printf("machineID %d source %d status %d AT %d DT %d UP %f RP %f\n",
						ptrCurrentEvent->machineInfo.machineID, ptrCurrentEvent->machineInfo.source,
						ptrCurrentEvent->machineInfo.status, ptrCurrentEvent->machineInfo.arrivalTime,
						ptrCurrentEvent->machineInfo.departureTime, ptrCurrentEvent->machineInfo.usagePrice,
						ptrCurrentEvent->machineInfo.reservationPrice);

			} else printf("ERROR (machine departure): machine not found!!!\n");

		} else printf("ERROR (machine departure): there is no machine list!!!\n");

	} else printf("ERROR (machine departure): wrong eventID!!!\n");

}
