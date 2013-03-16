/*
 * taskunschedule.c
 *
 *  Created on: Sep 4, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void TaskUnSchedule(event *ptrCurrentEvent, event **ptrPtrEventList, machine *ptrMachineList, task *ptrTaskList, taskAccountInfo **ptrPtrTaskAccountInfoList,
		balanceAccountInfo *ptrBalanceAccountInfo, job *ptrJobList) {

	if (ptrCurrentEvent->eventID == TASKPREEMPTED) {

		taskAccountInfo *ptrAuxTaskAccountList;
		ptrAuxTaskAccountList = *ptrPtrTaskAccountInfoList;
		unsigned short int found = 0;

		while(ptrAuxTaskAccountList) {

			if(ptrAuxTaskAccountList->machineID == ptrCurrentEvent->machineInfo.machineID &&
					ptrAuxTaskAccountList->source == ptrCurrentEvent->machineInfo.source &&
					ptrAuxTaskAccountList->finnishTime == 0) {
				found = 1;
				break;
			}

			ptrAuxTaskAccountList = ptrAuxTaskAccountList->nextTaskAccountInfo;
		}

		if (found) {

			printf("eventID %d (Task Preempted) time %ld ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			printf("taskID %d jobID %d machineID %d source %d\n", ptrAuxTaskAccountList->taskID,
					ptrAuxTaskAccountList->jobID, ptrAuxTaskAccountList->machineID, ptrAuxTaskAccountList->source);

			// atualizar a task para queued;
			task *ptrAuxTask;
			ptrAuxTask = ptrTaskList;

			while(ptrAuxTask){

				if(ptrAuxTask->taskID == ptrAuxTaskAccountList->taskID &&
						ptrAuxTask->jobID == ptrAuxTaskAccountList->jobID) {
					ptrAuxTask->status = QUEUED;
					break;
				}

				ptrAuxTask = ptrAuxTask->nextTask;
			}

			// atualizar o finnishTime e o cost da task na account list;
			ptrAuxTaskAccountList->finnishTime = ptrCurrentEvent->time;
			ptrAuxTaskAccountList->cost = ceil( (float)(ptrAuxTaskAccountList->finnishTime - ptrAuxTaskAccountList->startTime) / 60.0 ) * ptrCurrentEvent->machineInfo.usagePrice;

			// decrement on the grid's balnace
			if (ptrAuxTaskAccountList->source == GRID) {
				DecrementBalance(ptrBalanceAccountInfo, ptrCurrentEvent->time, (ptrAuxTaskAccountList->finnishTime - ptrAuxTaskAccountList->startTime));
			}

			// remover o evento TASKFINNISHED da lista de eventos
			event *ptrOldEvent;
			ptrOldEvent = (*ptrPtrEventList);

			while(ptrOldEvent) {
				if (ptrOldEvent->eventID == TASKFINNISHED && ptrOldEvent->taskInfo.taskID == ptrAuxTaskAccountList->taskID &&
						ptrOldEvent->taskInfo.jobID == ptrAuxTaskAccountList->jobID) {
					break;
				}
				ptrOldEvent = ptrOldEvent->nextEvent;
			}

			RemoveEvent(ptrPtrEventList, ptrOldEvent);

			// remover a entrada na account list
//			RemoveTaskAccountList(ptrPtrTaskAccountInfoList, ptrAuxTaskAccountList);
//			comentado pq nao mais serao removidas essas entradas, para efeito do calculo de custo (8/3/12)

			if (ptrCurrentEvent->time < simulationTime) {

				// if there are donating machines, it creates grid preempted events
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

							InsertEvent(*ptrPtrEventList, ptrNewGridPreemption);
						}
						else printf("ERROR (job arrival): merdou o malloc!!!\n");

					}

					ptrAuxMachine = ptrAuxMachine->nextMachine;
				} // end while(ptrAuxMachine)

				// insert a new planning into the event list
				event *ptrNewEvent;
				job *ptrAuxJobList;
				ptrAuxJobList = ptrJobList;

				while(ptrAuxJobList) {
					if (ptrAuxJobList->jobID == ptrAuxTaskAccountList->jobID) break;
					ptrAuxJobList = ptrAuxJobList->nextJob;
				}

				if( (ptrNewEvent = malloc(sizeof(event))) ) {

					ptrNewEvent->eventNumber = 0;
					ptrNewEvent->eventID = ALLOCATIONPLANNING;
					ptrNewEvent->time = (ptrCurrentEvent->time + 1);
					ptrNewEvent->jobInfo.jobID = ptrAuxJobList->jobID;
					ptrNewEvent->jobInfo.jobSize = ptrAuxJobList->jobSize;
					ptrNewEvent->jobInfo.arrivalTime = ptrAuxJobList->arrivalTime;
					ptrNewEvent->jobInfo.finnishTime = ptrAuxJobList->finnishTime;
					ptrNewEvent->jobInfo.longestTask = ptrAuxJobList->longestTask;
					ptrNewEvent->jobInfo.deadline = ptrAuxJobList->deadline;
					ptrNewEvent->jobInfo.maxUtility = ptrAuxJobList->maxUtility;
					ptrNewEvent->jobInfo.utility = ptrAuxJobList->utility;
					ptrNewEvent->jobInfo.cost = ptrAuxJobList->cost;
					ptrNewEvent->jobInfo.nextJob = NULL;
					ptrNewEvent->nextEvent = NULL;

					InsertEvent(*ptrPtrEventList, ptrNewEvent);

				} else printf("ERROR (task unschedule): merdou o malloc!!!\n");

			}

		} else printf("ERROR (task unschedule): no account list information!!!\n");

	} else printf("ERROR (task unschedule): wrong eventID!!!\n");

}
