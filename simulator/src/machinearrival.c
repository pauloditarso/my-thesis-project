/*
 * machinearrival.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void MachineArrival(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList) {

	if (ptrCurrentEvent->eventID == MACHARRIVAL) {

		if (ptrMachineList) {

			if (ptrMachineList->machineID == 0) {		// code for an empty machine list
				ptrMachineList->machineID = ptrCurrentEvent->machineInfo.machineID;
				ptrMachineList->source = ptrCurrentEvent->machineInfo.source;
				ptrMachineList->status = ptrCurrentEvent->machineInfo.status;
				ptrMachineList->arrivalTime = ptrCurrentEvent->machineInfo.arrivalTime;
				ptrMachineList->departureTime = ptrCurrentEvent->machineInfo.departureTime;
				ptrMachineList->usagePrice = ptrCurrentEvent->machineInfo.usagePrice;
				ptrMachineList->reservationPrice = ptrCurrentEvent->machineInfo.reservationPrice;
				ptrMachineList->nextMachine = NULL;

				// insert a new grid donation into the event list
				if ( ptrMachineList->source == 0 ) {

					event *ptrNewDonation;

					if( (ptrNewDonation = malloc(sizeof(event))) ) {
						ptrNewDonation->eventID = GRIDDONATING;
						ptrNewDonation->time = (ptrCurrentEvent->time+1); // one second after the machine's arrival
						ptrNewDonation->flag = 0;
						ptrNewDonation->nextEvent = NULL;

						InsertEvent(ptrEventList, ptrNewDonation);
					}
					else printf("ERROR (machine arrival): merdou o malloc!!!\n");

				}
			}
			else {

				machine *ptrNewMachine;

				if ( !(ptrNewMachine = malloc(sizeof(machine))) ) {
					printf("ERROR (machine arrival): merdou o malloc!!!\n");
				}

				ptrNewMachine->machineID = ptrCurrentEvent->machineInfo.machineID;
				ptrNewMachine->source = ptrCurrentEvent->machineInfo.source;
				ptrNewMachine->status = ptrCurrentEvent->machineInfo.status;
				ptrNewMachine->arrivalTime = ptrCurrentEvent->machineInfo.arrivalTime;
				ptrNewMachine->departureTime = ptrCurrentEvent->machineInfo.departureTime;
				ptrNewMachine->usagePrice = ptrCurrentEvent->machineInfo.usagePrice;
				ptrNewMachine->reservationPrice = ptrCurrentEvent->machineInfo.reservationPrice;
				ptrNewMachine->nextMachine = NULL;

				// insert a new grid donation into the event list
				if ( ptrNewMachine->source == 0 ) {

					event *ptrNewDonation;

					if( (ptrNewDonation = malloc(sizeof(event))) ) {
						ptrNewDonation->eventID = GRIDDONATING;
						ptrNewDonation->time = (ptrCurrentEvent->time+1); // one second after the machine's arrival
						ptrNewDonation->flag = 0;
						ptrNewDonation->nextEvent = NULL;

						InsertEvent(ptrEventList, ptrNewDonation);
					}
					else printf("ERROR (machine arrival): merdou o malloc!!!\n");

				}

				machine *ptrAux, *ptrActualMachine, *ptrLastMachine;
				ptrAux = ptrActualMachine = ptrLastMachine = ptrMachineList;

				while(ptrAux) {
					if(ptrActualMachine->source <= ptrNewMachine->source) {
						ptrLastMachine = ptrActualMachine;
						ptrActualMachine = ptrActualMachine->nextMachine;
					}
					ptrAux = ptrAux->nextMachine;
				}

				if (ptrActualMachine) {
					ptrNewMachine->nextMachine = ptrActualMachine;
					ptrLastMachine->nextMachine = ptrNewMachine;
				}
				else {			// resolves the last-node problem
					ptrNewMachine->nextMachine = NULL;
					ptrLastMachine->nextMachine = ptrNewMachine;
				}

			} // end else

			// insert a new schedule into the event list
			event *ptrNewSchedule;

			if( (ptrNewSchedule = malloc(sizeof(event))) ) {
				ptrNewSchedule->eventID = TASKSCHEDULE;
				ptrNewSchedule->time = ptrCurrentEvent->time;
				ptrNewSchedule->flag = 0;
				ptrNewSchedule->nextEvent = NULL;

				InsertEvent(ptrEventList, ptrNewSchedule);
			}
			else printf("ERROR (machine arrival): merdou o malloc!!!\n");

			printf("eventID %d (Machine Arrival) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			printf("machineID %d source %d status %d AT %d DT %d UP %f RP %f\n",
					ptrCurrentEvent->machineInfo.machineID, ptrCurrentEvent->machineInfo.source,
					ptrCurrentEvent->machineInfo.status, ptrCurrentEvent->machineInfo.arrivalTime,
					ptrCurrentEvent->machineInfo.departureTime, ptrCurrentEvent->machineInfo.usagePrice,
					ptrCurrentEvent->machineInfo.reservationPrice);

		} else printf("ERROR (machine arrival): there is no machine list!!!\n");

	} else printf("ERROR (machine arrival): wrong eventID!!!\n");

}
