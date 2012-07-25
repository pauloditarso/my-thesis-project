/*
 * taskfinnished.c
 *
 *  Created on: Aug 29, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void TaskFinnished(event *ptrCurrentEvent, event **ptrPtrEventList, task *ptrTaskList, taskAccountInfo *ptrTaskAccountInfoList, machine *ptrMachineList,
		balanceAccountInfo *ptrBalanceAccountInfo, job *ptrJobList) {

	if (ptrCurrentEvent->eventID == TASKFINNISHED) {

		task *ptrAuxTask;
		ptrAuxTask = ptrTaskList;
		taskAccountInfo *ptrAuxTaskAccount;
		ptrAuxTaskAccount = ptrTaskAccountInfoList;
		machine *ptrAuxMachine;
		ptrAuxMachine = ptrMachineList;
		job *ptrAuxJob;
		ptrAuxJob = ptrJobList;

		if (ptrAuxTask->taskID > 0) { // 0 means code for an empty task list

			while(ptrAuxTask){
				if (ptrAuxTask->taskID == ptrCurrentEvent->taskInfo.taskID &&	ptrAuxTask->jobID == ptrCurrentEvent->taskInfo.jobID) break;
				ptrAuxTask = ptrAuxTask->nextTask;
			}

			while(ptrAuxJob) {
				if (ptrAuxJob->jobID == ptrCurrentEvent->taskInfo.jobID) break;
				ptrAuxJob = ptrAuxJob->nextJob;
			}

			if (ptrAuxTask != NULL) {

				if (ptrCurrentEvent->taskInfo.status == FINNISHED) {
					ptrAuxTask->status = ptrCurrentEvent->taskInfo.status; // it must be FINNISHED
				}
				else {
					printf("ERROR (task finished): task status is not FINNISHED!!!\n");
				}

				printf("eventID %d (Task Finnished) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
				printf("taskID %d jobID %d AT %d jobSize %d runtime %d status %d submissions %d\n",
						ptrCurrentEvent->taskInfo.taskID, ptrCurrentEvent->taskInfo.jobID,
						ptrCurrentEvent->taskInfo.arrivalTime, ptrCurrentEvent->taskInfo.jobSize,
						ptrCurrentEvent->taskInfo.runtime, ptrCurrentEvent->taskInfo.status, ptrCurrentEvent->taskInfo.numberOfSubmissions);


				unsigned short int countAccountFinnished = 0;
				float usagePrice = 0.00, totalUsagePrice = 0.00;
				while(ptrAuxTaskAccount != NULL){

					if (ptrAuxTaskAccount->jobID == ptrAuxTask->jobID && ptrAuxTaskAccount->taskID == ptrAuxTask->taskID && ptrAuxTaskAccount->finnishTime == 0 ) {

						ptrAuxTaskAccount->finnishTime = ptrCurrentEvent->time;
						ptrAuxTaskAccount->status = ACCOUNTFINNISHED;

						while(ptrAuxMachine) {

							if (ptrAuxMachine->machineID == ptrAuxTaskAccount->machineID && ptrAuxMachine->source == ptrAuxTaskAccount->source) {

								ptrAuxMachine->status = IDLE;

								if (ptrAuxMachine->source == GRID) {

									// remover o evento futuro de partida desta maquina na lista de eventos
									event *ptrOldEvent;
									ptrOldEvent = (*ptrPtrEventList);

									while(ptrOldEvent) {
										if ( ptrOldEvent->time >= ptrCurrentEvent->time && ptrOldEvent->eventID == MACHDEPARTURE &&
												ptrOldEvent->machineInfo.machineID == ptrAuxMachine->machineID &&
												ptrOldEvent->machineInfo.source == ptrAuxMachine->source ) {
											break;
										}
										ptrOldEvent = ptrOldEvent->nextEvent;
									}

									if (ptrOldEvent) {
										RemoveEvent(ptrPtrEventList, ptrOldEvent);
									}
									else printf("ERROR (task finished): event not found!!!\n");

									// chamar machine departure para este tempo ou para 1seg a frente
									event *ptrOutGridMachine;
									if( (ptrOutGridMachine = malloc(sizeof(event))) ) {
										ptrOutGridMachine->eventNumber = 0;
										ptrOutGridMachine->eventID = MACHDEPARTURE;
										ptrOutGridMachine->time = ptrCurrentEvent->time;
										ptrOutGridMachine->machineInfo.machineID = ptrAuxMachine->machineID;
										ptrOutGridMachine->machineInfo.source = ptrAuxMachine->source;
										ptrOutGridMachine->machineInfo.status = ptrAuxMachine->status;
										ptrOutGridMachine->machineInfo.arrivalTime = ptrAuxMachine->arrivalTime;
										ptrOutGridMachine->machineInfo.departureTime = ptrCurrentEvent->time;
										ptrOutGridMachine->machineInfo.usagePrice = 0.0;
										ptrOutGridMachine->machineInfo.reservationPrice = 0.0;
										ptrOutGridMachine->machineInfo.nextMachine = NULL;
										ptrOutGridMachine->nextEvent = NULL;

										InsertEvent(*ptrPtrEventList, ptrOutGridMachine);
									}
									else {
										printf("ERROR (task finished): merdou o malloc!!!\n");
									}

								} // end of if (ptrAuxMachine->source == GRID)

								break;

							} // end of if (ptrAuxMachine->machineID == ptrAuxTaskAccount->machineID && ptrAuxMachine->source == ptrAuxTaskAccount->source)

							ptrAuxMachine = ptrAuxMachine->nextMachine;

						} // end of while(ptrAuxMachine)

						if (ptrAuxTaskAccount->source == GRID) {
							DecrementBalance(ptrBalanceAccountInfo, ptrCurrentEvent->time, (ptrAuxTaskAccount->finnishTime - ptrAuxTaskAccount->startTime));
						}

						if (ptrAuxMachine) {
							usagePrice = ptrAuxMachine->usagePrice;
						}
						else {
							if (ptrAuxTaskAccount->source != GRID) {
								printf("ERROR (task finished): source nao eh igual a GRID!!!\n");
							}
							usagePrice = 0.00;
						}

						ptrAuxTaskAccount->cost = ceil( (float)(ptrAuxTaskAccount->finnishTime - ptrAuxTaskAccount->startTime) / 60.0 ) * usagePrice;

					} // end of if (ptrAuxTaskAccount->jobID == ptrAuxTask->jobID && ptrAuxTaskAccount->taskID == ptrAuxTask->taskID &&	ptrAuxTaskAccount->finnishTime == 0 )

					if (ptrAuxTaskAccount->jobID == ptrAuxTask->jobID && ptrAuxTaskAccount->status == ACCOUNTFINNISHED) {
						countAccountFinnished++;
						totalUsagePrice += ptrAuxTaskAccount->cost;
					}

					ptrAuxTaskAccount = ptrAuxTaskAccount->nextTaskAccountInfo;

				}  // end of while(ptrAuxTaskAccount != NULL) (varendo a taskAccountList)

				if(countAccountFinnished == ptrAuxTask->jobSize) {

					event *ptrNewEvent, *ptrTargetEvent;
					ptrTargetEvent = ptrCurrentEvent;

					if( (ptrNewEvent = malloc(sizeof(event))) ) {
						ptrNewEvent->eventNumber = 0;
						ptrNewEvent->eventID = JOBFINNISHED;
						ptrNewEvent->time = ptrCurrentEvent->time;
						ptrNewEvent->jobInfo.jobID = ptrAuxJob->jobID;
						ptrNewEvent->jobInfo.jobSize = ptrAuxJob->jobSize;
						ptrNewEvent->jobInfo.arrivalTime = ptrAuxJob->arrivalTime;
						ptrNewEvent->jobInfo.finnishTime = ptrCurrentEvent->time;
						ptrNewEvent->jobInfo.longestTask = ptrAuxJob->longestTask;
						ptrNewEvent->jobInfo.deadline = ptrAuxJob->deadline;
						ptrNewEvent->jobInfo.maxUtility = ptrAuxJob->maxUtility;
						ptrNewEvent->jobInfo.utility = ptrAuxJob->utility;
						ptrNewEvent->jobInfo.cost = totalUsagePrice;
						ptrNewEvent->jobInfo.nextJob = NULL;
						ptrNewEvent->nextEvent = NULL;

						InsertAfterEvent(*ptrPtrEventList, ptrNewEvent, ptrTargetEvent);
					}
					else {
						printf("ERROR (task finnished): merdou o malloc!!!\n");
					}
				}

			} else printf("ERROR (task finnished) task not found!!!\n");

		} else printf("ERROR (task finnished) empty list!!!\n");

		// code to decide if it inserts a schedule or a donation event

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
				event *ptrNewDonation, *ptrTargetEvent;
				ptrTargetEvent = ptrCurrentEvent;

				if( (ptrNewDonation = malloc(sizeof(event))) ) {
					ptrNewDonation->eventNumber = 0;
					ptrNewDonation->eventID = GRIDDONATING;
					ptrNewDonation->time = (ptrCurrentEvent->time); // one second after the machine's arrival
					ptrNewDonation->machineInfo.machineID = ptrAuxMachine->machineID;
					ptrNewDonation->machineInfo.source = ptrAuxMachine->source;
					ptrNewDonation->machineInfo.status = DONATING;
					ptrNewDonation->machineInfo.arrivalTime = ptrAuxMachine->arrivalTime;
					ptrNewDonation->machineInfo.departureTime = ptrAuxMachine->departureTime;
					ptrNewDonation->machineInfo.usagePrice = ptrAuxMachine->usagePrice;
					ptrNewDonation->machineInfo.reservationPrice = ptrAuxMachine->reservationPrice;
					ptrNewDonation->machineInfo.nextMachine = ptrAuxMachine->nextMachine;
					ptrNewDonation->nextEvent = NULL;

					InsertAfterEvent(*ptrPtrEventList, ptrNewDonation, ptrTargetEvent);
//					InsertEvent(*ptrPtrEventList, ptrNewDonation);
				}
				else printf("ERROR (task finnished): merdou o malloc!!!\n");
			}
		}

		// insert a new allocation planning into the event list, if there isn't one already planned
		if (isThereQueuedTask == 1) {

			event *ptrAuxEventList;
			ptrAuxEventList = (*ptrPtrEventList);
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
							ptrNewGridPreemption->machineInfo.status = IDLE;
							ptrNewGridPreemption->machineInfo.arrivalTime = ptrAuxMach->arrivalTime;
							ptrNewGridPreemption->machineInfo.departureTime = ptrAuxMach->departureTime;
							ptrNewGridPreemption->machineInfo.reservationPrice = ptrAuxMach->reservationPrice;
							ptrNewGridPreemption->machineInfo.usagePrice = ptrAuxMach->usagePrice;
							ptrNewGridPreemption->machineInfo.nextMachine = ptrAuxMach->nextMachine;
							ptrNewGridPreemption->nextEvent = NULL;

							InsertEvent(*ptrPtrEventList, ptrNewGridPreemption);
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
					ptrNewEvent->jobInfo.jobID = ptrAuxJob->jobID;
					ptrNewEvent->jobInfo.jobSize = ptrAuxJob->jobSize;
					ptrNewEvent->jobInfo.arrivalTime = ptrAuxJob->arrivalTime;
					ptrNewEvent->jobInfo.finnishTime = ptrAuxJob->finnishTime;
					ptrNewEvent->jobInfo.longestTask = ptrAuxJob->longestTask;
					ptrNewEvent->jobInfo.deadline = ptrAuxJob->deadline;
					ptrNewEvent->jobInfo.maxUtility = ptrAuxJob->maxUtility;
					ptrNewEvent->jobInfo.utility = ptrAuxJob->utility;
					ptrNewEvent->jobInfo.cost = ptrAuxJob->cost;
					ptrNewEvent->jobInfo.nextJob = NULL;
					ptrNewEvent->nextEvent = NULL;

					InsertEvent(*ptrPtrEventList, ptrNewEvent);

				} else printf("ERROR (task finnished): merdou o malloc!!!\n");

			} // end if (thereIsAPlanning)

		} // end if (isThereQueuedTask)

	} else printf("ERROR (task finnished): wrong eventID!!!\n");

}
