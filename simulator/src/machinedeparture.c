/*
 * machinedeparture.c
 *
 *  Created on: Aug 26, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void MachineDeparture(event *ptrCurrentEvent, event *ptrEventList, machine **ptrPtrMachineList, taskAccountInfo *ptrTaskAccountInfoList) {

	if (ptrCurrentEvent->eventID == MACHDEPARTURE) {

		if (ptrPtrMachineList) {

			// INVARIANTE para o tempo de partida diferente do tempo atual
			if (ptrCurrentEvent->time == ptrCurrentEvent->machineInfo.departureTime) {

				machine *ptrActualMachine, *ptrLastMachine;
				unsigned short int machineFound = 0;

				ptrActualMachine = ptrLastMachine = (*ptrPtrMachineList);

				while(ptrActualMachine) {

					if ( ptrCurrentEvent->machineInfo.machineID == ptrActualMachine->machineID && ptrCurrentEvent->machineInfo.source == ptrActualMachine->source ) {
						machineFound = 1;
						break;
					}
					ptrLastMachine = ptrActualMachine;
					ptrActualMachine = ptrActualMachine->nextMachine;

				}

				if (machineFound) {

					if ( ptrActualMachine->status == RUNNING ) {

						// TRATAR DO CASO EM QUE A TASK FINALIZA NO MESMO TEMPO DE PARTIDA DA MAQUINA, MAS O EVENTO DE PARTIDA VEM ANTES NA LISTA
						// PROCURAR PELA TASK NA ACCOUNT LIST, ACHAR EVENTO TASKFINNISHED PARA TEMPO ATUAL E NAO PREEMPTAR A TASK

						taskAccountInfo *ptrAuxTaskAccount;
						ptrAuxTaskAccount = ptrTaskAccountInfoList;
						event *ptrAuxEventList;
						ptrAuxEventList = ptrEventList;
						unsigned short int taskFound = 0;
						unsigned short int eventFound = 0;

						while(ptrAuxTaskAccount) {

							if ( ptrAuxTaskAccount->machineID == ptrActualMachine->machineID && ptrAuxTaskAccount->source == ptrActualMachine->source
									&& ptrAuxTaskAccount->finnishTime == 0 && ptrAuxTaskAccount->status == ACCOUNTUNFINNISHED ) {
								taskFound = 1;
								break;
							}

							ptrAuxTaskAccount = ptrAuxTaskAccount->nextTaskAccountInfo;
						}

						if (taskFound) {

							while(ptrAuxEventList) {

								if ( ptrAuxEventList->time == ptrCurrentEvent->time && ptrAuxEventList->eventID == TASKFINNISHED &&
										ptrAuxEventList->taskInfo.taskID == ptrAuxTaskAccount->taskID && ptrAuxEventList->taskInfo.jobID == ptrAuxTaskAccount->jobID ) {
									eventFound = 1;
									break;
								}

								ptrAuxEventList = ptrAuxEventList->nextEvent;
							}

						} else printf("ERROR (machine departure) task not found!!!\n");

						// insert a new event into the event list
						if (!eventFound) {

							event *ptrNewEvent, *ptrTargetEvent;
							ptrTargetEvent = ptrCurrentEvent;

							if( (ptrNewEvent = malloc(sizeof(event))) ) {
								ptrNewEvent->eventNumber = 0;
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

								InsertAfterEvent(ptrEventList, ptrNewEvent, ptrTargetEvent);
							}
							else printf("ERROR (machine departure): merdou o malloc!!!\n");

						}

					} // end if ( ptrActualMachine->status == RUNNING )

					if ( ptrActualMachine->source == LOCAL && ptrActualMachine->status == DONATING) {

						// insert a new grid preemption into the event list
						event *ptrNewGridPreemption, *ptrTargetEvent;
						ptrTargetEvent = ptrCurrentEvent;

						if( (ptrNewGridPreemption = malloc(sizeof(event))) ) {
							ptrNewGridPreemption->eventNumber = 0;
							ptrNewGridPreemption->eventID = GRIDPREEMPTED;
							ptrNewGridPreemption->time = ptrCurrentEvent->time;
							ptrNewGridPreemption->machineInfo.machineID = ptrActualMachine->machineID;
							ptrNewGridPreemption->machineInfo.source = ptrActualMachine->source;
							ptrNewGridPreemption->machineInfo.status = ptrActualMachine->status;
							ptrNewGridPreemption->machineInfo.arrivalTime = ptrActualMachine->arrivalTime;
							ptrNewGridPreemption->machineInfo.departureTime = ptrActualMachine->departureTime;
							ptrNewGridPreemption->machineInfo.reservationPrice = ptrActualMachine->reservationPrice;
							ptrNewGridPreemption->machineInfo.usagePrice = ptrActualMachine->usagePrice;
							ptrNewGridPreemption->machineInfo.nextMachine = ptrActualMachine->nextMachine;
							ptrNewGridPreemption->nextEvent = NULL;

							InsertAfterEvent(ptrEventList, ptrNewGridPreemption, ptrTargetEvent);
						}
						else printf("ERROR (task schedule): merdou o malloc!!!\n");

					}

					printf("eventID %d (Machine Departure) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
					printf("machineID %d source %d status %d AT %d DT %d UP %.2f RP %.2f\n", ptrActualMachine->machineID, ptrActualMachine->source,
							ptrActualMachine->status, ptrActualMachine->arrivalTime, ptrActualMachine->departureTime, ptrActualMachine->usagePrice,
							ptrActualMachine->reservationPrice);

					if (ptrLastMachine != ptrActualMachine) {
						ptrLastMachine->nextMachine = ptrActualMachine->nextMachine;
						ptrActualMachine->nextMachine = NULL;
						free(ptrActualMachine);
						ptrActualMachine = NULL;
					}
					else {
						machine *ptrAux;
						ptrAux = *ptrPtrMachineList;
						*ptrPtrMachineList = (*ptrPtrMachineList)->nextMachine;
						free(ptrAux);
						ptrAux = NULL;
					}

				} else printf("ERROR (machine departure): machine not found!!!\n");

			} else printf("ERROR (machine departure): departure time different from current time!!!\n");

		} else printf("ERROR (machine departure): there is no machine list!!!\n");

	} else printf("ERROR (machine departure): wrong eventID!!!\n");

}
