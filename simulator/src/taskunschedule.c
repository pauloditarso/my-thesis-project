/*
 * taskunschedule.c
 *
 *  Created on: Sep 4, 2011
 *      Author: PauloDitarso
 */

#include "simulation.h"

void TaskUnSchedule(event *ptrCurrentEvent, event **ptrPtrEventList, machine *ptrMachineList, task *ptrTaskList, accountInfo **ptrPtrAccountInfoList) {

	if (ptrCurrentEvent->eventID == TASKPREEMPTED) {

		accountInfo *ptrAuxAccountList;
		ptrAuxAccountList = *ptrPtrAccountInfoList;
		unsigned short int found = 0;

		while(ptrAuxAccountList) {

			if(ptrAuxAccountList->machineID == ptrCurrentEvent->machineInfo.machineID &&
					ptrAuxAccountList->source == ptrCurrentEvent->machineInfo.source &&
					ptrAuxAccountList->finnishTime == 0) {
				found = 1;
				break;
			}

			ptrAuxAccountList = ptrAuxAccountList->nextAccountInfo;
		}

		if (found) {

			printf("eventID %d (Task Preempted) time %d ", ptrCurrentEvent->eventID, ptrCurrentEvent->time);
			printf("taskID %d jobID %d machineID %d source %d\n", ptrAuxAccountList->taskID,
					ptrAuxAccountList->jobID, ptrAuxAccountList->machineID, ptrAuxAccountList->source);

			// atualizar a task para queued;
			task *ptrAuxTask;
			ptrAuxTask = ptrTaskList;

			while(ptrAuxTask){

				if(ptrAuxTask->taskID == ptrAuxAccountList->taskID &&
						ptrAuxTask->jobID == ptrAuxAccountList->jobID) {
					ptrAuxTask->status = QUEUED;
					break;
				}

				ptrAuxTask = ptrAuxTask->nextTask;
			}

			// atualizar o finnishTime da task na account list;
//			ptrAuxAccountList->finnishTime = ptrCurrentEvent->time; ACHO QUE DEVE SER ZERO

			// remover o evento TASKFINNISHED da lista de eventos
			event *ptrOldEvent;
			ptrOldEvent = (*ptrPtrEventList);

			while(ptrOldEvent) {
				if (ptrOldEvent->eventID == TASKFINNISHED && ptrOldEvent->taskInfo.taskID == ptrAuxAccountList->taskID &&
						ptrOldEvent->taskInfo.jobID == ptrAuxAccountList->jobID) {
					break;
				}
				ptrOldEvent = ptrOldEvent->nextEvent;
			}

			RemoveEvent(ptrPtrEventList, ptrOldEvent);

			// remover a entrada na account list
			RemoveAccountList(ptrPtrAccountInfoList, ptrAuxAccountList);

			if (ptrCurrentEvent->time < 108000) {
				// insert a new schedule into the event list
				event *ptrNewEvent;

				if( (ptrNewEvent = malloc(sizeof(event))) ) {
					ptrNewEvent->eventID = TASKSCHEDULE;
					ptrNewEvent->time = ptrCurrentEvent->time;
					ptrNewEvent->flag = 0;
					ptrNewEvent->nextEvent = NULL;

					InsertEvent(*ptrPtrEventList, ptrNewEvent);
				}
				else printf("ERROR (machine arrival): merdou o malloc!!!\n");
			}

		} else printf("ERROR (task unschedule): no account list information!!!\n");

	} else printf("ERROR (task unschedule): wrong eventID!!!\n");

}
