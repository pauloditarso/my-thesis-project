/*
 * taskfinnished.c
 *
 *  Created on: Aug 29, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void TaskFinnished(event *ptrCurrentEvent, event *ptrEventList, task *ptrTaskList, taskAccountInfo *ptrTaskAccountInfoList, machine *ptrMachineList,
		balanceAccountInfo *ptrBalanceAccountInfo) {

	if (ptrCurrentEvent->eventID == TASKFINNISHED) {

		task *ptrAuxTask;
		ptrAuxTask = ptrTaskList;
		taskAccountInfo *ptrAuxTaskAccount;
		ptrAuxTaskAccount = ptrTaskAccountInfoList;
		machine *ptrAuxMachine;
		ptrAuxMachine = ptrMachineList;

		if (ptrAuxTask->taskID > 0) { // 0 means code for an empty task list

			while(ptrAuxTask){

				if (ptrAuxTask->taskID == ptrCurrentEvent->taskInfo.taskID &&
						ptrAuxTask->jobID == ptrCurrentEvent->taskInfo.jobID) {
					break;
				}
				ptrAuxTask = ptrAuxTask->nextTask;
			}

			if (ptrAuxTask != NULL) {

				ptrAuxTask->status = ptrCurrentEvent->taskInfo.status; // it must be FINNISHED

				printf("eventID %d (Task Finnished) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
				printf("taskID %d jobID %d AT %d jobSize %d runtime %d status %d utility %f\n",
						ptrCurrentEvent->taskInfo.taskID, ptrCurrentEvent->taskInfo.jobID,
						ptrCurrentEvent->taskInfo.arrivalTime, ptrCurrentEvent->taskInfo.jobSize,
						ptrCurrentEvent->taskInfo.runtime, ptrCurrentEvent->taskInfo.status,
						ptrCurrentEvent->taskInfo.utilityFunction);


				unsigned short int count = 0;
				while(ptrAuxTaskAccount != NULL){

					if (ptrAuxTaskAccount->jobID == ptrAuxTask->jobID) {

						if ( ptrAuxTaskAccount->taskID == ptrAuxTask->taskID &&	ptrAuxTaskAccount->finnishTime == 0 ) {

							ptrAuxTaskAccount->finnishTime = ptrCurrentEvent->time;
							ptrAuxTaskAccount->status = ACCOUNTFINNISHED;

							if (ptrAuxTaskAccount->source == GRID) {
								DecrementBalance(ptrBalanceAccountInfo, ptrCurrentEvent->time, (ptrAuxTaskAccount->finnishTime - ptrAuxTaskAccount->startTime));
							}

							while(ptrAuxMachine) {

								if(ptrAuxMachine->machineID == ptrAuxTaskAccount->machineID && ptrAuxMachine->source == ptrAuxTaskAccount->source) {

									ptrAuxMachine->status = IDLE;

									if (ptrAuxMachine->source == GRID) {
										// chamar machine departure para este tempo ou para 1seg a frente
										// remover o evento futuro de partida desta maquina na lista de eventos
									}

									break;

								}

								ptrAuxMachine = ptrAuxMachine->nextMachine;
							}

						}

						if (ptrAuxTaskAccount->status == ACCOUNTFINNISHED) {
							count += 1;
						}
					}

					ptrAuxTaskAccount = ptrAuxTaskAccount->nextTaskAccountInfo;

				}

				if(count == ptrAuxTask->jobSize) {

					event *ptrNewEvent, *ptrTargetEvent;
					ptrTargetEvent = ptrCurrentEvent;

					if( (ptrNewEvent = malloc(sizeof(event))) ) {
						ptrNewEvent->eventNumber = 0;
						ptrNewEvent->eventID = JOBFINNISHED;
						ptrNewEvent->time = ptrCurrentEvent->time;
						ptrNewEvent->jobInfo.jobID = ptrAuxTask->jobID;
						ptrNewEvent->nextEvent = NULL;

						InsertAfterEvent(ptrEventList, ptrNewEvent, ptrTargetEvent);
					}
					else {
						printf("ERROR (task schedule): merdou o malloc!!!\n");
					}
				}

			} else printf("(task finnished) task not found!!!\n");

		} else printf("(task finnished) empty list!!!\n");

		// code to decide if to insert a schedule or a donation event

		ptrAuxTask = ptrTaskList;
		unsigned short int isThereQueuedTask = 0;

		while(ptrAuxTask != NULL) {

			if(ptrAuxTask->taskID != 0 && ptrAuxTask->arrivalTime <= (ptrCurrentEvent->time+1) && ptrAuxTask->status == QUEUED) {
				isThereQueuedTask = 1;
				break;
			}

			ptrAuxTask = ptrAuxTask->nextTask;
		}

//		if (ptrAuxMachine == NULL) printf("NULL \n"); else printf("no-NULL\n"); // debug mode

//		printf("isThereQueuedTask == %d\n", isThereQueuedTask);  // debug mode

		if( isThereQueuedTask == 0 ) {

			if ( ptrAuxMachine != NULL && ptrAuxMachine->source == LOCAL ) {  // cloud machines may be inserted as well

				// insert a new donation into the event list, if there is no waiting tasks
				event *ptrNewDonation;//, *ptrTargetEvent;
//				ptrTargetEvent = ptrCurrentEvent;

				if( (ptrNewDonation = malloc(sizeof(event))) ) {
					ptrNewDonation->eventNumber = 0;
					ptrNewDonation->eventID = GRIDDONATING;
					ptrNewDonation->time = (ptrCurrentEvent->time+1); // one second after the machine's arrival
					ptrNewDonation->machineInfo.machineID = ptrAuxMachine->machineID;
					ptrNewDonation->machineInfo.source = ptrAuxMachine->source;
					ptrNewDonation->machineInfo.status = DONATING;
					ptrNewDonation->machineInfo.arrivalTime = ptrAuxMachine->arrivalTime;
					ptrNewDonation->machineInfo.departureTime = ptrAuxMachine->departureTime;
					ptrNewDonation->machineInfo.usagePrice = ptrAuxMachine->usagePrice;
					ptrNewDonation->machineInfo.reservationPrice = ptrAuxMachine->reservationPrice;
					ptrNewDonation->machineInfo.nextMachine = ptrAuxMachine->nextMachine;
					ptrNewDonation->nextEvent = NULL;

//					InsertAfterEvent(ptrEventList, ptrNewDonation, ptrTargetEvent);
					InsertEvent(ptrEventList, ptrNewDonation);
				}
				else printf("ERROR (task finnished): merdou o malloc!!!\n");
			}
		}

		// insert a new allocation planning into the event list, if there isn't one already planned
		if (isThereQueuedTask == 1) {

			event *ptrAuxEventList;
			ptrAuxEventList = ptrEventList;
			unsigned short int thereIsAPlanning = 0;

			while(ptrAuxEventList) {

				if ( ptrAuxEventList->time == (ptrCurrentEvent->time + 1) && ptrAuxEventList->eventID == ALLOCATIONPLANNING ) {
					thereIsAPlanning = 1;
					break;
				}

				ptrAuxEventList = ptrAuxEventList->nextEvent;
			}

			if(!thereIsAPlanning) {

				// if there is donating machines, it creates grid preempted events
				machine *ptrAuxMach;
				ptrAuxMach = ptrMachineList;
				while(ptrAuxMach) {

					if (ptrAuxMach->source == LOCAL && ptrAuxMach->status == DONATING) {

						ptrAuxMach->status = IDLE;
						event *ptrNewGridPreemption;

						if( (ptrNewGridPreemption = malloc(sizeof(event))) ) {
							ptrNewGridPreemption->eventNumber = 0;
							ptrNewGridPreemption->eventID = GRIDPREEMPTED;
							ptrNewGridPreemption->time = ptrCurrentEvent->time;
							ptrNewGridPreemption->machineInfo.machineID = ptrAuxMach->machineID;
							ptrNewGridPreemption->machineInfo.source = ptrAuxMach->source;
							ptrNewGridPreemption->machineInfo.status = QUEUED;
							ptrNewGridPreemption->machineInfo.arrivalTime = ptrAuxMach->arrivalTime;
							ptrNewGridPreemption->machineInfo.departureTime = ptrAuxMach->departureTime;
							ptrNewGridPreemption->machineInfo.reservationPrice = ptrAuxMach->reservationPrice;
							ptrNewGridPreemption->machineInfo.usagePrice = ptrAuxMach->usagePrice;
							ptrNewGridPreemption->machineInfo.nextMachine = ptrAuxMach->nextMachine;
							ptrNewGridPreemption->nextEvent = NULL;

							InsertEvent(ptrEventList, ptrNewGridPreemption);
						}
						else printf("ERROR (job arrival): merdou o malloc!!!\n");

					}

					ptrAuxMach = ptrAuxMach->nextMachine;
				} // end while(ptrAuxMach)

				event *ptrNewEvent;
				if( (ptrNewEvent = malloc(sizeof(event))) ) {

					ptrNewEvent->eventNumber = 0;
					ptrNewEvent->eventID = ALLOCATIONPLANNING;
					ptrNewEvent->time = (ptrCurrentEvent->time + 1);
					ptrNewEvent->flag = 1;
//					ptrNewEvent->taskInfo.taskID = ptrCurrentEvent->taskInfo.taskID;
//					ptrNewEvent->taskInfo.jobID = ptrCurrentEvent->taskInfo.jobID;
//					ptrNewEvent->taskInfo.jobSize = ptrCurrentEvent->taskInfo.jobSize;
//					ptrNewEvent->taskInfo.arrivalTime = ptrCurrentEvent->taskInfo.arrivalTime;
//					ptrNewEvent->taskInfo.runtime = ptrCurrentEvent->taskInfo.runtime;
//					ptrNewEvent->taskInfo.status = ptrCurrentEvent->taskInfo.status;
//					ptrNewEvent->taskInfo.utilityFunction = ptrCurrentEvent->taskInfo.utilityFunction;
//					ptrNewEvent->taskInfo.nextTask = NULL;
					ptrNewEvent->nextEvent = NULL;

					InsertEvent(ptrEventList, ptrNewEvent);

				} else printf("ERROR (task finnished): merdou o malloc!!!\n");

			} // end if (thereIsAPlanning)

		} // end if (isThereQueuedTask)

	} else printf("ERROR (task finnished): wrong eventID!!!\n");

}
