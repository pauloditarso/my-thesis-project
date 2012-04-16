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

						if ( ptrAuxTaskAccount->taskID == ptrAuxTask->taskID &&
								ptrAuxTaskAccount->finnishTime == 0 ) {

							ptrAuxTaskAccount->finnishTime = ptrCurrentEvent->time;
							ptrAuxTaskAccount->status = ACCOUNTFINNISHED;

							if (ptrAuxTaskAccount->source == GRID) {
								DecrementBalance(ptrBalanceAccountInfo, ptrCurrentEvent->time, (ptrAuxTaskAccount->finnishTime - ptrAuxTaskAccount->startTime));
							}

							while(ptrAuxMachine) {
								if(ptrAuxMachine->machineID == ptrAuxTaskAccount->machineID &&
										ptrAuxMachine->source == ptrAuxTaskAccount->source) {
									ptrAuxMachine->status = IDLE;
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

			}

		}
//		else printf("lista vazia!!!\n");

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

		if( isThereQueuedTask == 1) {
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
			else printf("ERROR (task finnished): merdou o malloc!!!\n");

		}

	} else printf("ERROR (task finnished): wrong eventID!!!\n");

}
