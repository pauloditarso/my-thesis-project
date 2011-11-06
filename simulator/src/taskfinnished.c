/*
 * taskfinnished.c
 *
 *  Created on: Aug 29, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void TaskFinnished(event *ptrCurrentEvent, event *ptrEventList, task *ptrTaskList, accountInfo *ptrAccountInfoList, machine *ptrMachineList) {

	if (ptrCurrentEvent->eventID == TASKFINNISHED) {

		task *ptrAuxTask;
		ptrAuxTask = ptrTaskList;
		accountInfo *ptrAuxAccount;
		ptrAuxAccount = ptrAccountInfoList;
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

				while(ptrAuxAccount){

					if (ptrAuxAccount->taskID == ptrAuxTask->taskID && ptrAuxAccount->jobID == ptrAuxTask->jobID) {

						ptrAuxAccount->finnishTime = ptrCurrentEvent->time;

						while(ptrAuxMachine) {

							if(ptrAuxMachine->machineID == ptrAuxAccount->machineID &&
									ptrAuxMachine->source == ptrAuxAccount->source) {
								ptrAuxMachine->status = IDLE;
								break;
							}

							ptrAuxMachine = ptrAuxMachine->nextMachine;
						}

						break;
					}
					ptrAuxAccount = ptrAuxAccount->nextAccountInfo;
				}

			}
		}
//		else printf("lista vazia!!!\n");

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

		// insert a new donation into the event list
		event *ptrNewDonation;

		if( (ptrNewDonation = malloc(sizeof(event))) ) {
			ptrNewDonation->eventID = GRIDDONATING;
			ptrNewDonation->time = (ptrCurrentEvent->time+1); // one second after the machine's arrival
			ptrNewDonation->flag = 0;
			ptrNewDonation->nextEvent = NULL;

			InsertEvent(ptrEventList, ptrNewDonation);
		}
		else printf("ERROR (machine arrival): merdou o malloc!!!\n");

	} else printf("ERROR (task finnished): wrong eventID!!!\n");

}
