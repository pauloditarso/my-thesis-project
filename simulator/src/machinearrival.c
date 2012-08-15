/*
 * machinearrival.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 *
 *  What is done:
 *  	1. it tests if this is the correct event
 *  	2. it tests if there is a machine list
 *  	3. it tests if the machine list is empty or not
 *  	4.
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

					// insert a new grid donation into the event list, if it is a local machine
					// cloud machines may be inserted here as well, in a near future
					if ( ptrMachineList->source == LOCAL ) {

						event *ptrNewDonation;//, *ptrTargetEvent;
						//						ptrTargetEvent = ptrCurrentEvent;

						if( (ptrNewDonation = malloc(sizeof(event))) ) {
							ptrNewDonation->eventNumber = 0;
							ptrNewDonation->eventID = GRIDDONATING;
							ptrNewDonation->time = (ptrCurrentEvent->time); // one second after machine's arrival
							ptrNewDonation->machineInfo.machineID = ptrCurrentEvent->machineInfo.machineID;
							ptrNewDonation->machineInfo.source = ptrCurrentEvent->machineInfo.source;
							ptrNewDonation->machineInfo.status = DONATING;
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

					} // ending donation if
				}
				else {

					// if there is donating machines, it creates grid preempted events
					machine *ptrAuxMachine;
					ptrAuxMachine = ptrMachineList;
					while(ptrAuxMachine) {

						if (ptrAuxMachine->source == LOCAL && ptrAuxMachine->status == DONATING) {

							ptrAuxMachine->status = IDLE;
							event *ptrNewGridPreemption;

							if( (ptrNewGridPreemption = malloc(sizeof(event))) ) {
								ptrNewGridPreemption->eventNumber = 0;
								ptrNewGridPreemption->eventID = GRIDPREEMPTED;
								ptrNewGridPreemption->time = ptrCurrentEvent->time;
								ptrNewGridPreemption->machineInfo.machineID = ptrAuxMachine->machineID;
								ptrNewGridPreemption->machineInfo.source = ptrAuxMachine->source;
								ptrNewGridPreemption->machineInfo.status = QUEUED;
								ptrNewGridPreemption->machineInfo.arrivalTime = ptrAuxMachine->arrivalTime;
								ptrNewGridPreemption->machineInfo.departureTime = ptrAuxMachine->departureTime;
								ptrNewGridPreemption->machineInfo.reservationPrice = ptrAuxMachine->reservationPrice;
								ptrNewGridPreemption->machineInfo.usagePrice = ptrAuxMachine->usagePrice;
								ptrNewGridPreemption->machineInfo.nextMachine = ptrAuxMachine->nextMachine;
								ptrNewGridPreemption->nextEvent = NULL;

								InsertEvent(ptrEventList, ptrNewGridPreemption);
							}
							else printf("ERROR (job arrival): merdou o malloc!!!\n");

						}

						ptrAuxMachine = ptrAuxMachine->nextMachine;
					} // end while(ptrAuxMachine)
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

				task *ptrAuxTask;
				ptrAuxTask = ptrTaskList;
				unsigned short int isThereTaskWaiting = 0;

				while(ptrAuxTask) {

					// investigar se nao eh possivel uma situacao de erro com tasks que
					// podem chegar um segundo depois do evento atual
					if (ptrAuxTask->taskID != 0 && ptrAuxTask->arrivalTime <= ptrCurrentEvent->time && ptrAuxTask->status == QUEUED) {
						isThereTaskWaiting = 1;
						break;
					}

					ptrAuxTask = ptrAuxTask->nextTask;
				}

				if (isThereTaskWaiting == 0) {

					// insert a new grid donation into the event list, if there is no waiting task
					// cloud machines may be inserted here as well, in a near future
					if ( ptrNewMachine->source == LOCAL ) {

						event *ptrNewDonation;//, *ptrTargetEvent;
						//						ptrTargetEvent = ptrCurrentEvent;

						if( (ptrNewDonation = malloc(sizeof(event))) ) {
							ptrNewDonation->eventNumber = 0;
							ptrNewDonation->eventID = GRIDDONATING;
							ptrNewDonation->time = (ptrCurrentEvent->time); // one second after machine's arrival
							ptrNewDonation->machineInfo.machineID = ptrCurrentEvent->machineInfo.machineID;
							ptrNewDonation->machineInfo.source = ptrCurrentEvent->machineInfo.source;
							ptrNewDonation->machineInfo.status = DONATING;
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

					} // ending donation if

				}
				else {

					// if there is donating machines, it creates grid preempted events
					machine *ptrAuxMachine;
					ptrAuxMachine = ptrMachineList;
					while(ptrAuxMachine) {

						if (ptrAuxMachine->source == LOCAL && ptrAuxMachine->status == DONATING) {

							ptrAuxMachine->status = IDLE;
							event *ptrNewGridPreemption;

							if( (ptrNewGridPreemption = malloc(sizeof(event))) ) {
								ptrNewGridPreemption->eventNumber = 0;
								ptrNewGridPreemption->eventID = GRIDPREEMPTED;
								ptrNewGridPreemption->time = ptrCurrentEvent->time;
								ptrNewGridPreemption->machineInfo.machineID = ptrAuxMachine->machineID;
								ptrNewGridPreemption->machineInfo.source = ptrAuxMachine->source;
								ptrNewGridPreemption->machineInfo.status = QUEUED;
								ptrNewGridPreemption->machineInfo.arrivalTime = ptrAuxMachine->arrivalTime;
								ptrNewGridPreemption->machineInfo.departureTime = ptrAuxMachine->departureTime;
								ptrNewGridPreemption->machineInfo.reservationPrice = ptrAuxMachine->reservationPrice;
								ptrNewGridPreemption->machineInfo.usagePrice = ptrAuxMachine->usagePrice;
								ptrNewGridPreemption->machineInfo.nextMachine = ptrAuxMachine->nextMachine;
								ptrNewGridPreemption->nextEvent = NULL;

								InsertEvent(ptrEventList, ptrNewGridPreemption);
							}
							else printf("ERROR (job arrival): merdou o malloc!!!\n");

						}

						ptrAuxMachine = ptrAuxMachine->nextMachine;
					} // end while(ptrAuxMachine)
				}

			} // end else

			printf("eventID %d (Machine Arrival) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			printf("machineID %d source %d status %d AT %d DT %d UP %.2f RP %.2f\n",
					ptrCurrentEvent->machineInfo.machineID, ptrCurrentEvent->machineInfo.source,
					ptrCurrentEvent->machineInfo.status, ptrCurrentEvent->machineInfo.arrivalTime,
					ptrCurrentEvent->machineInfo.departureTime, ptrCurrentEvent->machineInfo.usagePrice,
					ptrCurrentEvent->machineInfo.reservationPrice);

		} else printf("ERROR (machine arrival): there is no machine list!!!\n");

	} else printf("ERROR (machine arrival): wrong eventID!!!\n");

}
