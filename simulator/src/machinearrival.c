/*
 * machinearrival.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void MachineArrival(event *ptrCurrentEvent, event *ptrEventList, machine *ptrMachineList, task *ptrTaskList) {

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

				// insert a new grid donation into the event list, if there is no waiting task
				if ( ptrMachineList->source == LOCAL ) {  // cloud machines may be inserted here as well

					task *ptrAuxTask;
					ptrAuxTask = ptrTaskList;
					unsigned short int isThereTaskWaiting = 0;

					while(ptrAuxTask) {

						if (ptrAuxTask->taskID != 0 && ptrAuxTask->arrivalTime <= ptrCurrentEvent->time && ptrAuxTask->status == QUEUED) {
							isThereTaskWaiting = 1;
							break;
						}

						ptrAuxTask = ptrAuxTask->nextTask;
					}

					if (isThereTaskWaiting == 0) {

						event *ptrNewDonation;//, *ptrTargetEvent;
//						ptrTargetEvent = ptrCurrentEvent;

						if( (ptrNewDonation = malloc(sizeof(event))) ) {
							ptrNewDonation->eventNumber = 0;
							ptrNewDonation->eventID = GRIDDONATING;
							ptrNewDonation->time = (ptrCurrentEvent->time+1); // one second after machine's arrival
							ptrNewDonation->machineInfo.machineID = ptrCurrentEvent->machineInfo.machineID;
							ptrNewDonation->machineInfo.source = ptrCurrentEvent->machineInfo.source;
							ptrNewDonation->machineInfo.status = ptrCurrentEvent->machineInfo.status;
							ptrNewDonation->machineInfo.arrivalTime = ptrCurrentEvent->machineInfo.arrivalTime;
							ptrNewDonation->machineInfo.departureTime = ptrCurrentEvent->machineInfo.departureTime;
							ptrNewDonation->machineInfo.usagePrice = ptrCurrentEvent->machineInfo.usagePrice;
							ptrNewDonation->machineInfo.reservationPrice = ptrCurrentEvent->machineInfo.reservationPrice;
							ptrNewDonation->machineInfo.nextMachine = ptrCurrentEvent->machineInfo.nextMachine;
							ptrNewDonation->nextEvent = NULL;

//							InsertAfterEvent(ptrEventList, ptrNewDonation, ptrTargetEvent);
							InsertEvent(ptrEventList, ptrNewDonation);
						}
						else printf("ERROR (machine arrival): merdou o malloc!!!\n");

					}

				} // ending donation if

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

				// insert a new grid donation into the event list, if there is no waiting task
				if ( ptrNewMachine->source == LOCAL ) {  // cloud machines may be inserted here as well

					task *ptrAuxTask;
					ptrAuxTask = ptrTaskList;
					unsigned short int isThereTaskWaiting = 0;

					while(ptrAuxTask) {

						if (ptrAuxTask->taskID != 0 && ptrAuxTask->arrivalTime <= ptrCurrentEvent->time && ptrAuxTask->status == QUEUED) {
							isThereTaskWaiting = 1;
							break;
						}

						ptrAuxTask = ptrAuxTask->nextTask;
					}

					if (isThereTaskWaiting == 0) {

						event *ptrNewDonation;//, *ptrTargetEvent;
//						ptrTargetEvent = ptrCurrentEvent;

						if( (ptrNewDonation = malloc(sizeof(event))) ) {
							ptrNewDonation->eventNumber = 0;
							ptrNewDonation->eventID = GRIDDONATING;
							ptrNewDonation->time = (ptrCurrentEvent->time+1); // one second after machine's arrival
							ptrNewDonation->machineInfo.machineID = ptrCurrentEvent->machineInfo.machineID;
							ptrNewDonation->machineInfo.source = ptrCurrentEvent->machineInfo.source;
							ptrNewDonation->machineInfo.status = ptrCurrentEvent->machineInfo.status;
							ptrNewDonation->machineInfo.arrivalTime = ptrCurrentEvent->machineInfo.arrivalTime;
							ptrNewDonation->machineInfo.departureTime = ptrCurrentEvent->machineInfo.departureTime;
							ptrNewDonation->machineInfo.usagePrice = ptrCurrentEvent->machineInfo.usagePrice;
							ptrNewDonation->machineInfo.reservationPrice = ptrCurrentEvent->machineInfo.reservationPrice;
							ptrNewDonation->machineInfo.nextMachine = ptrCurrentEvent->machineInfo.nextMachine;
							ptrNewDonation->nextEvent = NULL;

//							InsertAfterEvent(ptrEventList, ptrNewDonation, ptrTargetEvent);
							InsertEvent(ptrEventList, ptrNewDonation);
						}
						else printf("ERROR (machine arrival): merdou o malloc!!!\n");
					}

				} // ending donation if

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
			event *ptrNewSchedule, *ptrTargetEvent;
			ptrTargetEvent = ptrCurrentEvent;

			if( (ptrNewSchedule = malloc(sizeof(event))) ) {
				ptrNewSchedule->eventNumber = 0;
				ptrNewSchedule->eventID = TASKSCHEDULE;
				ptrNewSchedule->time = ptrCurrentEvent->time;
				ptrNewSchedule->flag = 0;
				ptrNewSchedule->nextEvent = NULL;

				InsertAfterEvent(ptrEventList, ptrNewSchedule, ptrTargetEvent);
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
