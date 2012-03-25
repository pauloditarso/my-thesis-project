/*
 * taskunschedule.c
 *
 *  Created on: Sep 4, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void TaskUnSchedule(event *ptrCurrentEvent, event **ptrPtrEventList, machine *ptrMachineList, task *ptrTaskList, taskAccountInfo **ptrPtrTaskAccountInfoList) {

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

			printf("eventID %d (Task Preempted) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
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

			// atualizar o finnishTime da task na account list;
			ptrAuxTaskAccountList->finnishTime = ptrCurrentEvent->time;

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
//			comentado pq n‹o mais ser‹o removidas essas entradas, para efeito do calculo de custo (8/3/12)

			if (ptrCurrentEvent->time < 108000) {
				// insert a new schedule into the event list
				event *ptrNewEvent, *ptrTargetEvent;
				ptrTargetEvent = ptrCurrentEvent;

				if( (ptrNewEvent = malloc(sizeof(event))) ) {
					ptrNewEvent->eventID = TASKSCHEDULE;
					ptrNewEvent->time = ptrCurrentEvent->time;
					ptrNewEvent->flag = 0;
					ptrNewEvent->nextEvent = NULL;

					InsertAfterEvent(*ptrPtrEventList, ptrNewEvent, ptrTargetEvent);
				}
				else printf("ERROR (machine arrival): merdou o malloc!!!\n");
			}

		} else printf("ERROR (task unschedule): no account list information!!!\n");

	} else printf("ERROR (task unschedule): wrong eventID!!!\n");

}
