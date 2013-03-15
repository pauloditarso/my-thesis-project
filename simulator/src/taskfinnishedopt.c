/*
 * taskfinnishedopt.c
 *
 *  Created on: Aug 29, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void TaskFinnishedOpt(event *ptrCurrentEvent, event **ptrPtrEventList, task *ptrTaskList, taskAccountInfo *ptrTaskAccountInfoList, machine *ptrMachineList,
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
				if ( ptrAuxTask->taskID == ptrCurrentEvent->taskInfo.taskID && ptrAuxTask->jobID == ptrCurrentEvent->taskInfo.jobID ) break;
				ptrAuxTask = ptrAuxTask->nextTask;
			}

			while(ptrAuxJob) {
				if (ptrAuxJob->jobID == ptrCurrentEvent->taskInfo.jobID) break;
				ptrAuxJob = ptrAuxJob->nextJob;
			}

			if (ptrAuxTask != NULL) {

				if (ptrCurrentEvent->taskInfo.status == FINNISHED) ptrAuxTask->status = ptrCurrentEvent->taskInfo.status; // it must be FINNISHED
				else printf("ERROR (task finishedOpt): task status is not FINNISHED!!!\n");

				printf("eventID %d (Task Finnished) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
				printf("taskID %d jobID %d AT %d jobSize %d runtime %d status %d submissions %d\n",
						ptrCurrentEvent->taskInfo.taskID, ptrCurrentEvent->taskInfo.jobID,
						ptrCurrentEvent->taskInfo.arrivalTime, ptrCurrentEvent->taskInfo.jobSize,
						ptrCurrentEvent->taskInfo.runtime, ptrCurrentEvent->taskInfo.status, ptrCurrentEvent->taskInfo.numberOfSubmissions);


				unsigned short int countAccountFinnished = 0;
//				float usagePrice = 0.00, totalUsagePrice = 0.00;
				while(ptrAuxTaskAccount != NULL) {

					if ( ptrAuxTaskAccount->jobID == ptrAuxTask->jobID && ptrAuxTaskAccount->taskID == ptrAuxTask->taskID &&
							ptrAuxTaskAccount->finnishTime == 0 ) {

						ptrAuxTaskAccount->finnishTime = ptrCurrentEvent->time;
						ptrAuxTaskAccount->status = ACCOUNTFINNISHED;

						while(ptrAuxMachine) {

							if ( ptrAuxMachine->machineID == ptrAuxTaskAccount->machineID && ptrAuxMachine->source == ptrAuxTaskAccount->source ) {
								ptrAuxMachine->status = IDLE;
								break;
							}
							ptrAuxMachine = ptrAuxMachine->nextMachine;

						} // end of while(ptrAuxMachine)

						if (ptrAuxTaskAccount->source == GRID) {
							DecrementBalance(ptrBalanceAccountInfo, ptrCurrentEvent->time, (ptrAuxTaskAccount->finnishTime - ptrAuxTaskAccount->startTime));
						}

//						if (ptrAuxMachine) {
//							usagePrice = ptrAuxMachine->usagePrice;
//						}
//						else {
//							if (ptrAuxTaskAccount->source != GRID) {
//								printf("ERROR (task finished): source nao eh igual a GRID!!!\n");
//							}
//							usagePrice = 0.00;
//						}
//
//						ptrAuxTaskAccount->cost = ceil( (float)(ptrAuxTaskAccount->finnishTime - ptrAuxTaskAccount->startTime) / 60.0 ) * usagePrice;

					} // end of if (ptrAuxTaskAccount->jobID == ptrAuxTask->jobID && ptrAuxTaskAccount->taskID == ptrAuxTask->taskID &&	ptrAuxTaskAccount->finnishTime == 0 )

					if ( ptrAuxTaskAccount->jobID == ptrAuxTask->jobID && ptrAuxTaskAccount->status == ACCOUNTFINNISHED ) {
						countAccountFinnished++;
//						totalUsagePrice += ptrAuxTaskAccount->cost;
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
						ptrNewEvent->jobInfo.cost = ptrAuxJob->cost;
//						ptrNewEvent->jobInfo.cost = totalUsagePrice;
						ptrNewEvent->jobInfo.nextJob = NULL;
						ptrNewEvent->nextEvent = NULL;

						InsertAfterEvent(*ptrPtrEventList, ptrNewEvent, ptrTargetEvent);
					}
					else {
						printf("ERROR (task finnishedOpt): merdou o malloc!!!\n");
					}
				}

			} else printf("ERROR (task finnishedOpt) task not found!!!\n");

		} else printf("ERROR (task finnishedOpt) empty list!!!\n");

		// insert a grid donation into the event list, if there isn't a task schedule planned
		event *ptrAuxEventList;
		ptrAuxEventList = (*ptrPtrEventList);
		unsigned short int thereIsASchedule = 0, thereIsAMachine = 0;

		if (ptrAuxMachine != NULL) {

			thereIsAMachine = 1;

			while(ptrAuxEventList) {

				if ( ptrAuxEventList->time == ptrCurrentEvent->time && ptrAuxEventList->eventID == TASKSCHEDULE &&
						ptrAuxEventList->scheduleInfo.machineID == ptrAuxMachine->machineID &&
						ptrAuxEventList->scheduleInfo.source == ptrAuxMachine->source ) {
					thereIsASchedule = 1;
					break;
				}

				ptrAuxEventList = ptrAuxEventList->nextEvent;
			}

		}

		if (thereIsAMachine && !thereIsASchedule) {

			if ( ptrAuxMachine != NULL && ptrAuxMachine->source == LOCAL ) {  // cloud machines may be inserted as well

				// debug mode
//				if ( ptrCurrentEvent->time == 1072 && ptrAuxMachine->machineID == 3 ) passou();

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

		} // end of if (thereIsAMachine && thereIsntASchedule)

	} else printf("ERROR (task finnished): wrong eventID!!!\n");

}
