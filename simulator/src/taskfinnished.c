/*
 * taskfinnished.c
 *
 *  Created on: Aug 29, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void TaskFinnished(event *ptrCurrentEvent, event *ptrEventList, task *ptrTaskList, taskAccountInfo *ptrTaskAccountInfoList, machine *ptrMachineList) {

	if (ptrCurrentEvent->eventID == TASKFINNISHED) {

		task *ptrAuxTask;
		ptrAuxTask = ptrTaskList;
		taskAccountInfo *ptrAuxTaskAccount;
		ptrAuxTaskAccount = ptrTaskAccountInfoList;
		machine *ptrAuxMachine;
		ptrAuxMachine = ptrMachineList;

		if (ptrAuxTask->taskID > 0) { // 0 means code for an empty task list

			while(ptrAuxTask){

				if (ptrAuxTask->taskID == ptrCurrentEvent->taskInfo.taskID && ptrAuxTask->jobID == ptrCurrentEvent->taskInfo.jobID) {
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

						count += 1;

						if (ptrAuxTaskAccount->taskID == ptrAuxTask->taskID) {

							ptrAuxTaskAccount->finnishTime = ptrCurrentEvent->time;

							while(ptrAuxMachine) {

								if(ptrAuxMachine->machineID == ptrAuxTaskAccount->machineID &&
										ptrAuxMachine->source == ptrAuxTaskAccount->source) {
									ptrAuxMachine->status = IDLE;
									break;
								}

								ptrAuxMachine = ptrAuxMachine->nextMachine;
							}

							if(count == ptrAuxTask->jobSize) {

								event *ptrNewEvent;

								if( (ptrNewEvent = malloc(sizeof(event))) ) {
									ptrNewEvent->eventID = JOBFINNISHED;
									ptrNewEvent->time = ptrCurrentEvent->time;
									ptrNewEvent->jobInfo.jobID = ptrAuxTask->jobID;
									ptrNewEvent->nextEvent = NULL;

									InsertEvent(ptrEventList, ptrNewEvent);
								}
								else {
									printf("ERROR (task schedule): merdou o malloc!!!\n");
								}

							}

							break;
						}
					}

					ptrAuxTaskAccount = ptrAuxTaskAccount->nextTaskAccountInfo;

				}

			}
		}
//		else printf("lista vazia!!!\n");

		// code to decide if to insert a schedule or a donation event

		ptrAuxTask = ptrTaskList;
		unsigned short int isThereQueuedTask = 0;

		while(ptrAuxTask) {

			if(ptrAuxTask->taskID != 0 && ptrAuxTask->arrivalTime <= (ptrCurrentEvent->time+1) && ptrAuxTask->status == QUEUED) {
				isThereQueuedTask = 1;
				break;
			}

			ptrAuxTask = ptrAuxTask->nextTask;
		}

		if( isThereQueuedTask == 0 && ptrAuxMachine->source == LOCAL ) {  // cloud machines may be inserted as well
			// insert a new donation into the event list, if there is no waiting tasks

			event *ptrNewDonation;

			if( (ptrNewDonation = malloc(sizeof(event))) ) {
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

				InsertEvent(ptrEventList, ptrNewDonation);
			}
			else printf("ERROR (task finnished): merdou o malloc!!!\n");

		}
		else {
			// insert a new schedule into the event list

			event *ptrNewSchedule;

			if( (ptrNewSchedule = malloc(sizeof(event))) ) {
				ptrNewSchedule->eventID = TASKSCHEDULE;
				ptrNewSchedule->time = ptrCurrentEvent->time;
				ptrNewSchedule->flag = 0;
				ptrNewSchedule->nextEvent = NULL;

				InsertEvent(ptrEventList, ptrNewSchedule);
			}
			else printf("ERROR (task finnished): merdou o malloc!!!\n");

		}

	} else printf("ERROR (task finnished): wrong eventID!!!\n");

}
